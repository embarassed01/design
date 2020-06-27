#ifndef ALGO200_H
#define ALGO200_H

#include "runcrack.h"

#define ALGO200_DESCRIPTION  "mysql323"
#define ALGO200_EXAMPLE_PWD   "hashcat"
#define ALGO200_EXAMPLE_HASH  "7196759210defdc0"
// "7196759210defdc0" -- hashcat

STATE_FLAG algo200_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo200_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo200_free_parse_hash(crack_t * crack);


#endif // ALGO200_H
