# Set policy to enable VERSION in project()
cmake_policy(SET CMP0048 NEW)

project(kminer VERSION 1.0.0)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Enable solvers here
option(USE_CPU_XENONCAT "USE CPU_XENONCAT" ON)
option(USE_CUDA_DJEZO "USE CUDA_DJEZO" OFF)

## Add solvers here
if (USE_CPU_XENONCAT)
    # Ensure assemble.sh is executable
    execute_process(
        COMMAND chmod +x ${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/assemble.sh
        RESULT_VARIABLE CHMOD_RESULT
    )
    if(NOT CHMOD_RESULT EQUAL 0)
        message(WARNING "Failed to make assemble.sh executable")
    endif()

    # Custom command to generate assembly object files
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/equihash_avx1.o
               ${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/equihash_avx2.o
        COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux && ./assemble.sh
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/equihash_avx1.asm
                ${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/equihash_avx2.asm
                ${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/assemble.sh
                ${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/fasm
        COMMENT "Generating CPU_XENONCAT assembly object files"
        VERBATIM
    )

    # Custom target to ensure assembly files are built
    add_custom_target(cpu_xenoncat_asm 
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/equihash_avx1.o
                ${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/equihash_avx2.o
    )

    add_definitions(-DUSE_CPU_XENONCAT)
    message("-- USE_CPU_XENONCAT DEFINED")
endif()
if (USE_CUDA_DJEZO)
    add_definitions(-DUSE_CUDA_DJEZO)
    message("-- USE_CUDA_DJEZO DEFINED")
endif()

# Suppress Boost bind deprecation warning
# add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)


########
# LINUX
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # use
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -msse2 -fPIC -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -msse2 -fPIC -Wall -Wextra")
    # optimizations
    add_definitions(-O2)
endif()

# Common
include_directories(${kminer_SOURCE_DIR}/kminer)

# BOOST
#find_package(Threads REQUIRED COMPONENTS)
# compile boost staticaly
set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#set(BUILD_SHARED_LIBRARIES OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
find_package(Boost REQUIRED COMPONENTS system log_setup log date_time filesystem thread)

if (Boost_FOUND)
  # From the offical documentation:
  # Add include directories to the build. [...] If the SYSTEM option is given,
  # the compiler will be told the directories are meant as system include
  # directories on some platforms (signalling this setting might achieve effects
  # such as the compiler skipping warnings [...])."
  include_directories (SYSTEM ${Boost_INCLUDE_DIR})

  # From the offical documentation:
  # "Specify directories in which the linker will look for libraries. [...] Note
  # that this command is rarely necessary. Library locations returned by
  # find_package() and find_library() are absolute paths. Pass these absolute
  # library file paths directly to the target_link_libraries() command. CMake
  # will ensure the linker finds them."
  link_directories (${Boost_LIBRARY_DIRS})
else()
    message("Boost_FOUND NOT FOUND")
endif ()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

# Fetch nlohmann::json (header-only modern JSON library)
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

set(SOURCE_FILES
    # sources
    kminer/amount.cpp
    kminer/api.cpp
    kminer/arith_uint256.cpp
    kminer/crypto/sha256.cpp
    kminer/libstratum/StratumClient.cpp
    kminer/libstratum/ZcashStratum.cpp
    kminer/main.cpp
    kminer/primitives/block.cpp
    kminer/speed.cpp
    kminer/uint256.cpp
    kminer/utilstrencodings.cpp
    # headers
    kminer/amount.h
    kminer/api.hpp
    kminer/arith_uint256.h
    kminer/crypto/sha256.h
    kminer/hash.h
    kminer/libstratum/StratumClient.h
    kminer/libstratum/ZcashStratum.h
    kminer/primitives/block.h
    kminer/primitives/transaction.h
    kminer/script/script.h
    kminer/serialize.h
    kminer/speed.hpp
    kminer/streams.h
    kminer/support/allocators/zeroafterfree.h
    kminer/tinyformat.h
    kminer/uint252.h
    kminer/uint256.h
    kminer/utilstrencodings.h
    kminer/version.h
    algos/zcash/JoinSplit.hpp
    algos/zcash/NoteEncryption.hpp
    algos/zcash/Proof.hpp
    algos/zcash/Zcash.h
    kminer/SolverStub.h
    kminer/AvailableSolvers.h
    kminer/ISolver.h
    kminer/Solver.h
    kminer/MinerFactory.h
    kminer/MinerFactory.cpp
    )

if (USE_CUDA_DJEZO)
    set(SOURCE_FILES ${SOURCE_FILES}
    # make same path on windows
    #blake shared
    # src
    algos/blake2/blake2bx.cpp
    # headers
    algos/blake2/blake2.h
    algos/blake2/blake2b-load-sse2.h
    algos/blake2/blake2b-load-sse41.h
    algos/blake2/blake2b-round.h
    algos/blake2/blake2-config.h
    algos/blake2/blake2-impl.h
    algos/blake2/blake2-round.h
    )
endif()

#set(LIBS ${LIBS} ${Threads_LIBRARIES} ${Boost_LIBRARIES})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

message("-- CXXFLAGS: ${CMAKE_CXX_FLAGS}")
message("-- LIBS: ${LIBS}")

if (USE_CPU_XENONCAT)
    add_subdirectory(solvers/cpu_xenoncat)
endif()
if (USE_CUDA_DJEZO)
    add_subdirectory(solvers/cuda_djezo)
endif()

#add_subdirectory(cpu_xenoncat)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

#target_link_libraries(${PROJECT_NAME} ${LIBS} ${CUDA_LIBRARIES} )
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${LIBS} )

# link libs
if (USE_CPU_XENONCAT)
    # Ensure assembly files are built before linking
    add_dependencies(${PROJECT_NAME} cpu_xenoncat_asm)
    
    add_library ( xenoncat_avx1 SHARED IMPORTED GLOBAL )
    set_target_properties ( xenoncat_avx1 PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/equihash_avx1.o" )
    add_library ( xenoncat_avx2 SHARED IMPORTED GLOBAL )
    set_target_properties ( xenoncat_avx2 PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/solvers/cpu_xenoncat/asm_linux/equihash_avx2.o" )
    target_link_libraries(${PROJECT_NAME} cpu_xenoncat xenoncat_avx1 xenoncat_avx2)
endif()
if (USE_CUDA_DJEZO)
    target_link_libraries(${PROJECT_NAME} cuda_djezo)
endif()

