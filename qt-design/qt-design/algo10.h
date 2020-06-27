#ifndef ALGO10_H
#define ALGO10_H

#include "runcrack.h"

#define ALGO10_DESCRIPTION   "md5($pass.$salt)"
#define ALGO10_EXAMPLE_PWD   "hashcat"
#define ALGO10_EXAMPLE_HASH  "3d83c8e717ff0e7ecfe187f088d69954:343141"
// "3d83c8e717ff0e7ecfe187f088d69954:343141" -- hashcat

STATE_FLAG algo10_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo10_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo10_free_parse_hash(crack_t * crack);

#endif // ALGO10_H
