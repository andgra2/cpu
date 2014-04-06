#ifndef TOK_H
#define TOK_H

#include "char.h"
#include "vector.h"

enum tok_type {
	lit_numb,
	lit_ch,
	lit_str,
	ident,
	other,
};

struct tok_lit_numb {
	unsigned val;
};

struct tok_lit_ch {
	char c;
};

struct tok_lit_str {
	char *s_beg;
	char *s_end;
};

struct tok_other {
	char c;
};

struct tok {
	const char *beg, *end;
	enum tok_type type;
	union {
		struct tok_lit_numb lit_numb;
		struct tok_lit_ch lit_ch;
		struct tok_lit_str lit_str;
		struct tok_other other;
	};
};

struct tok_seq {
	struct tok *toks;
	size_t num_toks;
};

bool tok_seq_create(struct tok_seq *ts, const char *cs, bool *ok, const char **tok_err_pos);
void tok_seq_destroy(struct tok_seq *ts);

#endif

