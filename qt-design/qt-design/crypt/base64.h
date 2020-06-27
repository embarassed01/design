#ifndef BASE64_H
#define BASE64_H

#include "my_qt_common.h"

int base64_encode(u8 * out, int * outlen, const u8 * in, int inlen);

int base64_decode(u8 * out, int * outlen, const u8 * in, int inlen);

#endif // BASE64_H
