/*
Copyright (c) 2009-2014 The Bitcoin Core developers
Copyright (c) 2025 ComputerGenieCo
Licensed under GPL v3.0
see LICENSE file for a full copy of the GNU General Public License
*/

#if defined(HAVE_CONFIG_H)
#include "config/bitcoin-config.h"
#endif

#include <cstring>

#if HAVE_DECL_STRNLEN == 0
size_t strnlen( const char *start, size_t max_len)
{
    const char *end = (const char *)memchr(start, '\0', max_len);

    return end ? (size_t)(end - start) : max_len;
}
#endif // HAVE_DECL_STRNLEN
