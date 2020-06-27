#ifndef ALGO50_H
#define ALGO50_H

#include "runcrack.h"

#define ALGO50_DESCRIPTION    "HMAC-MD5 (key = $pass)"
#define ALGO50_EXAMPLE_PWD   "hashcat"
#define ALGO50_EXAMPLE_HASH  "e28e4e37e972a945e464b5226053bac0:40"
// "e28e4e37e972a945e464b5226053bac0:40" -- hashcat

STATE_FLAG algo50_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo50_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo50_free_parse_hash(crack_t * crack);

#endif // ALGO50_H
