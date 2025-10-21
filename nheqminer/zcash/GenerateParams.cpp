/*
 * This file is part of nheqminer.
 *
Licensed under GPL v3.0
see LICENSE file for a full copy of the GNU General Public License
 *
 * Copyright (c) 2025 ComputerGenieCo
 */

#include "zcash/JoinSplit.hpp"

#include <iostream>
#include "crypto/common.h"

int main(int argc, char **argv)
{
    if (init_and_check_sodium() == -1) {
        return 1;
    }

    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " provingKeyFileName verificationKeyFileName" << std::endl;
        return 1;
    }

    std::string pkFile = argv[1];
    std::string vkFile = argv[2];

    auto p = ZCJoinSplit::Generate();

    p->saveProvingKey(pkFile);
    p->saveVerifyingKey(vkFile);

    delete p;

    return 0;
}
