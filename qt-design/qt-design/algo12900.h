#ifndef ALGO12900_H
#define ALGO12900_H

#include "runcrack.h"

#define ALGO12900_DESCRIPTION    "Android FDE (Samsung DEK)"
#define ALGO12900_EXAMPLE_PWD   "hashcat"
#define ALGO12900_EXAMPLE_HASH  "15738301074686823451275227041071157383010746868234512752270410712bc4be900bf96ccf43c9852fff49b5f5874a9f6e7bf301686fa6d98286de151f15738301074686823451275227041071"
// "15738301074686823451275227041071157383010746868234512752270410712bc4be900bf96ccf43c9852fff49b5f5874a9f6e7bf301686fa6d98286de151f15738301074686823451275227041071" -- hashcat

STATE_FLAG algo12900_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo12900_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo12900_free_parse_hash(crack_t * crack);

#endif // ALGO12900_H
