/*
 * This file is part of kminer.
 *
Licensed under GPL v3.0
see LICENSE file for a full copy of the GNU General Public License
 *
 * Copyright (c) 2025 ComputerGenieCo
 */

#ifndef _ZCCONSTANTS_H_
#define _ZCCONSTANTS_H_

#define ZC_NUM_JS_INPUTS 2
#define ZC_NUM_JS_OUTPUTS 2
#define INCREMENTAL_MERKLE_TREE_DEPTH 29
#define INCREMENTAL_MERKLE_TREE_DEPTH_TESTING 4

#define ZC_NOTEPLAINTEXT_LEADING 1
#define ZC_V_SIZE 8
#define ZC_RHO_SIZE 32
#define ZC_R_SIZE 32
#define ZC_MEMO_SIZE 512

#define ZC_NOTEPLAINTEXT_SIZE (ZC_NOTEPLAINTEXT_LEADING + ZC_V_SIZE + ZC_RHO_SIZE + ZC_R_SIZE + ZC_MEMO_SIZE)

#endif // _ZCCONSTANTS_H_
