#ifndef ALGO150_H
#define ALGO150_H

#include "runcrack.h"

#define ALGO150_DESCRIPTION    "HMAC-SHA1 (key = $pass)"
#define ALGO150_EXAMPLE_PWD   "hashcat"
#define ALGO150_EXAMPLE_HASH  "02b256705348a28b1d6c0f063907979f7e0c82f8:10323"
// "02b256705348a28b1d6c0f063907979f7e0c82f8:10323" -- hashcat

STATE_FLAG algo150_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo150_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo150_free_parse_hash(crack_t * crack);

#endif // ALGO150_H
