#include "char.h"

bool char_is_ws(char c) { return isspace(c); }

bool char_is_letter(char c) { return isalpha(c); }

bool char_is_print(char c) { return isprint(c); }

bool char_is_oct_dig(char c) { return '0' <= c && c <= '7'; }

bool char_is_dec_dig(char c) { return isdigit(c); }

bool char_is_hex_dig(char c) { return isxdigit(c);  }

bool char_is_dig_of_base(char c, unsigned b)
{
	if (b == 8) return char_is_oct_dig(c);
	if (b == 10) return char_is_dec_dig(c);
	if (b == 16) return char_is_hex_dig(c);
}

unsigned char_get_dig_val(char c)
{
	if ('0' <= c && c <= '9') return c - '0';
	if ('a' <= tolower(c) && tolower(c) <= 'f') return 10 + tolower(c) - 'a';
}

