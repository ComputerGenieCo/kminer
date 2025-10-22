# kminer - Equihash CPU & GPU Miner

A high-performance Equihash miner supporting efficient CPU and GPU solvers. This is a fork of the abandoned nheqminer project, optimized to include only the faster CPU_XENONCAT and CUDA_DJEZO solvers while removing slower alternatives.

## Solver Options

This project supports 2 different solvers:

### CPU Solvers
- **CPU_XENONCAT**: Optimized CPU solver with AVX2 assembly (faster, requires AVX2)

### GPU Solvers
- **CUDA_DJEZO**: Optimized CUDA GPU solver (faster)

## Quick Start

### Prerequisites
- Ubuntu/Debian-based Linux distribution
- GCC 13.3 (for CUDA compatibility, see notes below)
- CMake 3.22+
- Boost 1.74
- FASM assembler (for CPU_XENONCAT)

### Install Dependencies
```bash
sudo apt update
sudo apt install build-essential cmake libboost-all-dev fasm
```

### Clone and Build (Default: CPU_XENONCAT)
```bash
git clone https://github.com/ComputerGenieCo/kminer.git
cd kminer

# Build (assembly files are generated automatically)
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Test the Build
```bash
# Run benchmark to verify solvers work
./kminer -b

# Test mining with localhost pool
./kminer -l localhost:5332 -u test.worker1 -t 4
```

> **Note**: The CPU_XENONCAT solver requires assembly files that are automatically generated during the build process. CMake will automatically run the FASM assembler to create the necessary `equihash_avx1.o` and `equihash_avx2.o` files when building with CPU_XENONCAT enabled.

### Validation Results

**CUDA Compatibility Notes**:
- CUDA 12.6 supports GCC 7-13.x and resolves GCC 13.3.0 compatibility issues
- If using Ubuntu's nvidia-cuda-toolkit package, you may need to install proper CUDA 12.6 toolkit and update PATH
- Test with `./kminer -ci` to verify CUDA device detection

## Custom Build Options

### Build with Specific Solvers

To enable only specific solvers, modify the CMake options:

#### CPU Only (XENONCAT only)
```bash
cmake .. -DUSE_CPU_XENONCAT=ON -DUSE_CUDA_DJEZO=OFF
make -j$(nproc)
```
*Assembly files are automatically generated during build*

#### GPU Only (CUDA_DJEZO)
```bash
cmake .. -DUSE_CPU_XENONCAT=OFF -DUSE_CUDA_DJEZO=ON
make -j$(nproc)
```

#### CPU + GPU (All solvers)
```bash
cmake .. -DUSE_CPU_XENONCAT=ON -DUSE_CUDA_DJEZO=ON
make -j$(nproc)
```

### GPU Requirements

For CUDA solvers, you need:
- NVIDIA GPU with CUDA support
- CUDA Toolkit 11+
- NVIDIA drivers

Install CUDA:
```bash
sudo apt install nvidia-cuda-toolkit
```

## Usage

### Command Line Options
```
Parameters:
  -h    Print this help and quit
  -l [location]    Stratum server:port
  -u [username]    Username (bitcoinaddress)
  -a [port]    Local API port (default: 0 = do not bind)
  -d [level]    Debug print level (0 = print all, 5 = fatal only, default: 2)
  -b [hashes]    Run in benchmark mode (default: 200 iterations)

CPU settings
  -t [num_thrds]    Number of CPU threads
  -e [ext]    Force CPU ext (0 = SSE2, 1 = AVX, 2 = AVX2)

NVIDIA CUDA settings
  -ci    CUDA info
  -cd [devices]    Enable CUDA mining on spec. devices
  -cb [blocks]    Number of blocks
  -ct [tpb]    Number of threads per block
```

### Examples

#### Benchmark (test all enabled solvers)
```bash
./kminer -b
```

#### Mine with CPU (4 threads)
```bash
./kminer -l localhost:5332 -u test.worker1 -t 4
```

#### Mine with CPU (8 threads, AVX2 forced)
```bash
./kminer -l localhost:5332 -u test.worker1 -t 8 -e 2
```

#### Mine with GPU (CUDA device 0)
```bash
./kminer -l localhost:5332 -u test.worker1 -cd 0
```

#### Mine with CPU + GPU
```bash
./kminer -l localhost:5332 -u test.worker1 -t 4 -cd 0
```

## Troubleshooting

### CUDA Build Issues

If you encounter CUDA compilation errors:

1. **GCC/CUDA Version Compatibility**: CUDA 12.6 supports GCC 7-13.x
2. **CUDA Toolkit Installation**: Ensure you have a proper CUDA 12.6 installation, not just Ubuntu's nvidia-cuda-toolkit package
3. **PATH Configuration**: Update your PATH to prioritize CUDA 12.6: `export PATH=/usr/local/cuda-12.6/bin:$PATH`
4. **Device Detection**: Use `./kminer -ci` to verify CUDA devices are detected
5. **Build Fixes Applied**: This version includes fixes for CUDA 12.6 compatibility including:
   - Updated deprecated `__shfl` to `__shfl_sync`
   - Updated deprecated `__any` to `__any_sync`
   - Fixed C++11 function signatures
   - Resolved header inclusion conflicts
