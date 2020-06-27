#ifndef ALGO1400_H
#define ALGO1400_H

#include "runcrack.h"

#define ALGO1400_DESCRIPTION    "SHA2-256"
#define ALGO1400_EXAMPLE_PWD   "hashcat"
#define ALGO1400_EXAMPLE_HASH  "127e6fbfe24a750e72930c220a8e138275656b8e5d8f48a98c3c92df2caba935"
// "127e6fbfe24a750e72930c220a8e138275656b8e5d8f48a98c3c92df2caba935" -- hashcat

STATE_FLAG algo1400_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo1400_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo1400_free_parse_hash(crack_t * crack);

#endif // ALGO1400_H
