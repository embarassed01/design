#ifndef ALGO20_H
#define ALGO20_H

#include "runcrack.h"

#define ALGO20_DESCRIPTION  "md5($salt.$pass)"
#define ALGO20_EXAMPLE_PWD   "hashcat"
#define ALGO20_EXAMPLE_HASH  "57ab8499d08c59a7211c77f557bf9425:4247"
// "57ab8499d08c59a7211c77f557bf9425:4247" -- hashcat

STATE_FLAG algo20_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo20_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo20_free_parse_hash(crack_t * crack);

#endif // ALGO20_H
