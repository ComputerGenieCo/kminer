/*
Copyright (c) 2009-2010 Satoshi Nakamoto
Copyright (c) 2009-2014 The Bitcoin Core developers
Copyright (c) 2025 ComputerGenieCo
Licensed under GPL v3.0
see LICENSE file for a full copy of the GNU General Public License
*/

#ifndef BITCOIN_SCRIPT_SIGCACHE_H
#define BITCOIN_SCRIPT_SIGCACHE_H

#include "script/interpreter.h"

#include <vector>

class CPubKey;

class CachingTransactionSignatureChecker : public TransactionSignatureChecker
{
private:
    bool store;

public:
    CachingTransactionSignatureChecker(const CTransaction* txToIn, unsigned int nInIn, bool storeIn=true) : TransactionSignatureChecker(txToIn, nInIn), store(storeIn) {}

    bool VerifySignature(const std::vector<unsigned char>& vchSig, const CPubKey& vchPubKey, const uint256& sighash) const;
};

#endif // BITCOIN_SCRIPT_SIGCACHE_H
