#ifndef ALGO1450_H
#define ALGO1450_H

#include "runcrack.h"

#define ALGO1450_DESCRIPTION    "HMAC-SHA256 (key = $pass)"
#define ALGO1450_EXAMPLE_PWD   "hashcat"
#define ALGO1450_EXAMPLE_HASH  "b435ffbacea34d5eb0dbc4d69a92f0152f2cf4cd364d34c2ece322ca22d8b334:21217"
// "b435ffbacea34d5eb0dbc4d69a92f0152f2cf4cd364d34c2ece322ca22d8b334:21217" -- hashcat

STATE_FLAG algo1450_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo1450_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo1450_free_parse_hash(crack_t * crack);

#endif // ALGO1450_H
