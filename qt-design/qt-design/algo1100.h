#ifndef ALGO1100_H
#define ALGO1100_H

#include "runcrack.h"

#define ALGO1100_DESCRIPTION    "Domain Cached Credentials (DCC), MS Cache"
#define ALGO1100_EXAMPLE_PWD   "hashcat"
#define ALGO1100_EXAMPLE_HASH  "4dd8965d1d476fa0d026722989a6b772:3060147285011"
// "4dd8965d1d476fa0d026722989a6b772:3060147285011" -- hashcat

STATE_FLAG algo1100_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo1100_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo1100_free_parse_hash(crack_t * crack);

#endif // ALGO1100_H
