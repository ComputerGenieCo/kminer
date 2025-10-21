/*
 * This file is part of nheqminer.
 *
Licensed under GPL v3.0
see LICENSE file for a full copy of the GNU General Public License
 *
 * Copyright (c) 2025 ComputerGenieCo
 */

#pragma once

#include <string>
#include <functional>
#include <vector>
#include <cstdint>

#define CREATE_SOLVER_STUB(NAME, DESC) \
class NAME { \
public: \
    int blocks; \
    int use_opt; \
    int threadsperblock; \
    NAME() {} \
    NAME([[maybe_unused]] int platf_id, [[maybe_unused]] int dev_id) {} \
    std::string getdevinfo() { return ""; } \
    std::string getname() { return DESC; } \
    static int getcount() { return 0; } \
    static void getinfo([[maybe_unused]] int platf_id, [[maybe_unused]] int d_id, [[maybe_unused]] std::string& gpu_name, [[maybe_unused]] int& sm_count, [[maybe_unused]] std::string& version)  {} \
    static void start([[maybe_unused]] NAME& device_context)  {} \
    static void stop([[maybe_unused]] NAME& device_context)  {} \
    static void solve([[maybe_unused]] const char *tequihash_header, \
        [[maybe_unused]] unsigned int tequihash_header_len, \
        [[maybe_unused]] const char* nonce, \
        [[maybe_unused]] unsigned int nonce_len, \
        [[maybe_unused]] std::function<bool()> cancelf, \
        [[maybe_unused]] std::function<void(const std::vector<uint32_t>&, size_t, const unsigned char*)> solutionf, \
        [[maybe_unused]] std::function<void(void)> hashdonef, \
        [[maybe_unused]] NAME& device_context)  {} \
};
