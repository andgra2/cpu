#ifndef CHAR_H
#define CHAR_H

#include "util.h"

extern bool char_is_ws(char c);
extern bool char_is_letter(char c);
extern bool char_is_pseudo_letter(char c);
extern bool char_is_print(char c);
extern bool char_is_bin_dig(char c);
extern bool char_is_oct_dig(char c);
extern bool char_is_dec_dig(char c);
extern bool char_is_hex_dig(char c);
extern bool char_is_dig(char c, unsigned b);
extern unsigned char_dig_val(char c);

#endif

