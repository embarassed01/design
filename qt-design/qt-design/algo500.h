#ifndef ALGO500_H
#define ALGO500_H

#include "runcrack.h"

#define ALGO500_DESCRIPTION    "md5crypt, MD5 (Unix), Cisco-IOS $1$ (MD5)"
#define ALGO500_EXAMPLE_PWD   "hashcat"
#define ALGO500_EXAMPLE_HASH  "$1$28772684$iEwNOgGugqO9.bIz5sk8k/"
// "$1$28772684$iEwNOgGugqO9.bIz5sk8k/" -- hashcat

STATE_FLAG algo500_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo500_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo500_free_parse_hash(crack_t * crack);

#endif // ALGO500_H
