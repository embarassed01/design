#ifndef ALGO1711_H
#define ALGO1711_H

#include "runcrack.h"

#define ALGO1711_DESCRIPTION    "SSHA-512(Base64), LDAP {SSHA512}"
#define ALGO1711_EXAMPLE_PWD   "hashcat"
#define ALGO1711_EXAMPLE_HASH  "{SSHA512}ALtwKGBdRgD+U0fPAy31C28RyKYx7+a8kmfksccsOeLknLHv2DBXYI7TDnTolQMBuPkWDISgZr2cHfnNPFjGZTEyNDU4OTkw"
// "{SSHA512}ALtwKGBdRgD+U0fPAy31C28RyKYx7+a8kmfksccsOeLknLHv2DBXYI7TDnTolQMBuPkWDISgZr2cHfnNPFjGZTEyNDU4OTkw" -- hashcat

STATE_FLAG algo1711_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo1711_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo1711_free_parse_hash(crack_t * crack);

#endif // ALGO1711_H
