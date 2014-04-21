#include "char.h"

extern bool char_is_ws(char c) { return isspace(c); }

extern bool char_is_letter(char c) { return isalpha(c); }

extern bool char_is_pseudo_letter(char c) { return isalpha(c) || c == '_' || isdigit(c); }

extern bool char_is_print(char c) { return isprint(c); }

extern bool char_is_bin_dig(char c) { return '0' <= c && c <= '1'; }

extern bool char_is_oct_dig(char c) { return '0' <= c && c <= '7'; }

extern bool char_is_dec_dig(char c) { return isdigit(c); }

extern bool char_is_hex_dig(char c) { return isxdigit(c); }

extern bool char_is_dig(char c, unsigned b)
{
	if (b == 2) return char_is_bin_dig(c);
	if (b == 8) return char_is_oct_dig(c);
	if (b == 10) return char_is_dec_dig(c);
	if (b == 16) return char_is_hex_dig(c);

	assert(false);
}

extern unsigned char_dig_val(char c)
{
	if ('0' <= c && c <= '9') return c - '0';
	if ('a' <= tolower(c) && tolower(c) <= 'f') return 10 + tolower(c) - 'a';

	assert(false);
}

