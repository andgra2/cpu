#ifndef TOK_H
#define TOK_H

#include "util.h"
#include "char.h"
#include "vector.h"

enum tok_type {
	lit_numb,
	lit_ch,
	lit_str,
	ident,
	punct_op,
	eot,
};

struct tok_lit_numb {
	unsigned val;
};

struct tok_lit_ch {
	char c;
};

struct tok_lit_str {
	char *s_beg, *s_end;
};

struct tok_ident {
	char *s;
};

struct tok_punct_op {
	char c;
};

struct tok {
	enum tok_type type;
	union {
		struct tok_lit_numb lit_numb;
		struct tok_lit_ch lit_ch;
		struct tok_lit_str lit_str;
		struct tok_ident ident;
		struct tok_punct_op punct_op;
	};
};

extern bool tok_seq_create(struct tok **ts, const char *cs, bool *te, const char **tep, char **te_msg);
extern void tok_seq_destroy(struct tok *ts);

#endif

