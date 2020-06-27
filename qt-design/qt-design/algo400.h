#ifndef ALGO400_H
#define ALGO400_H

#include "runcrack.h"

#define ALGO400_DESCRIPTION  "phpass,WordPress(MD5),Joomla(MD5)/phpass,phpBB3(MD5)"
#define ALGO400_EXAMPLE_PWD   "hashcat"
#define ALGO400_EXAMPLE_HASH  "$P$984478476IagS59wHZvyQMArzfx58u."
// "$P$984478476IagS59wHZvyQMArzfx58u." -- hashcat
// "$H$984478476IagS59wHZvyQMArzfx58u." -- hashcat

STATE_FLAG algo400_parse_hash    (crack_t * crack, const char * hash,     int len_hash);

STATE_FLAG algo400_crack_for_test(crack_t * crack, const char * password, int len_pwd);

void       algo400_free_parse_hash(crack_t * crack);

#endif // ALGO400_H
