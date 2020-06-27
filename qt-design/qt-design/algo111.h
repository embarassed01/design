#ifndef ALGO111_H
#define ALGO111_H

#include "runcrack.h"

#define ALGO111_DESCRIPTION   "nsldaps,SSHA-1(Base64),Netscape LDAP SSHA"
#define ALGO111_EXAMPLE_PWD   "hashcat"
#define ALGO111_EXAMPLE_HASH  "{SSHA}AZKja92fbuuB9SpRlHqaoXxbTc43Mzc2MDM1Ng=="
// "{SSHA}AZKja92fbuuB9SpRlHqaoXxbTc43Mzc2MDM1Ng==" -- hashcat

STATE_FLAG algo111_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo111_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo111_free_parse_hash(crack_t * crack);

#endif // ALGO111_H
