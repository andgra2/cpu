#include "tok.h"

static void print_ch(char c)
{
	if (char_is_print(c))
		printf("%c", c);
	else
		printf("(%u)", (unsigned) c);
}

static void print_tok(struct tok *t)
{
	switch (t->type)
	{
	case lit_numb:
		printf("Literal number: %u", t->lit_numb.val);
		break;
	case lit_ch:
		printf("Literal char: ");
		print_ch(t->lit_ch.c);
		break;
	case lit_str:
		printf("Literal string: ");
		for (const char *p = t->lit_str.s_beg; p != t->lit_str.s_end; ++p) {
			if (p != t->lit_str.s_beg) printf(" ");
			print_ch(*p);
		}
		break;
	case ident:
		printf("Identifier: ");
		for (const char *p = t->beg; p != t->end; ++p)
			printf("%c", *p);
		break;
	case other:
		printf("Other: ");
		printf("%c", t->other.c);
		break;
	}
}

int main()
{
	const char s[] = "\
 .data   { x, y, z; arr[] = \"12\\\\df\\x2Fg\"; }\
 .code { add(x, y, z); print_str(arr); exit(); }\
";

	struct tok_seq ts;
	bool ok;
	const char *tok_err_pos;
	if (!tok_seq_create(&ts, s, &ok, &tok_err_pos)) {
		if (ok) {
			printf("Tokenizing error!\n");
			printf(
				"Invalid token starting at char pos %u",
				(unsigned) (tok_err_pos - s)
			);
		} else
			printf("Unexpected error!\n");

		return -1;
	}

	for (size_t i = 0; i != ts.num_toks; ++i) {
		print_tok(ts.toks + i);
		printf("\n");
	}

	tok_seq_destroy(&ts);

	return 0;
}

