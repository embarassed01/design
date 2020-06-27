#ifndef ALGO300_H
#define ALGO300_H

#include "runcrack.h"

#define ALGO300_DESCRIPTION  "mysql4.1/mysql5"
#define ALGO300_EXAMPLE_PWD   "hashcat"
#define ALGO300_EXAMPLE_HASH  "fcf7c1b8749cf99d88e5f34271d636178fb5d130"
// "fcf7c1b8749cf99d88e5f34271d636178fb5d130" -- hashcat

STATE_FLAG algo300_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo300_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo300_free_parse_hash(crack_t * crack);

#endif // ALGO300_H
