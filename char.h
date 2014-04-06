#ifndef CHAR_H
#define CHAR_H

#include "util.h"

bool char_is_ws(char c);
bool char_is_letter(char c);
bool char_is_print(char c);
bool char_is_oct_dig(char c);
bool char_is_dec_dig(char c);
bool char_is_hex_dig(char c);
bool char_is_dig_of_base(char c, unsigned b);
unsigned char_get_dig_val(char c);

#endif

