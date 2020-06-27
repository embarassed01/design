#ifndef PBKDF2_MD4_H
#define PBKDF2_MD4_H

#include "my_qt_common.h"

void pbkdf2_md4(const u8 * key, size_t key_len, const u8 * input, size_t inlen, u64 iters, u8 * output, size_t outlen);

#endif // PBKDF2_MD4_H
