#ifndef ALGO0_H
#define ALGO0_H

#include "runcrack.h"

#define ALGO0_DESCRIPTION   "MD5"
#define ALGO0_EXAMPLE_PWD   "hashcat"
#define ALGO0_EXAMPLE_HASH  "8743b52063cd84097a65d1633f5c74f5"
// "8743b52063cd84097a65d1633f5c74f5" -- hashcat

STATE_FLAG algo0_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo0_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo0_free_parse_hash(crack_t * crack);

#endif // ALGO0_H
