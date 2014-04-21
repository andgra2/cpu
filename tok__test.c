#include "tok.h"

static void print_c(char c)
{
	if (isgraph(c))
		printf("%c", c);
	else
		printf("(%u)", (unsigned) c);
}

static void print_tok(const struct tok *t)
{
	switch (t->type)
	{
	case lit_numb:
		printf("N	%u", t->lit_numb.val);
		break;
	case lit_ch:
		printf("C	");
		print_c(t->lit_ch.c);
		break;
	case lit_str:
		printf("S	");
		for (const char *cp = t->lit_str.s_beg; cp != t->lit_str.s_end; ++cp) {
			print_c(*cp);
			if (cp != t->lit_str.s_end) printf(" ");
		}
		break;
	case ident:
		printf("I	%s", t->ident.s);
		break;
	case punct_op:
		printf("P	%c", t->punct_op.c);
		break;
	case eot:
		assert(false);
		break;
	}
}

int main()
{
	bool te;
	const char *tep;
	char *te_msg;

	char s[] = "\
data {\n\
	i = 0xaf43;\n\
	s = \"h\\x45js\\101n!\\a\";\n\
	d[34] = {1, 2, 3};\n\
}\n\
\n\
code {\n\
	print_str(s);\n\
	add(i);\n\
}\n\
";

	struct tok *ts;
	if (!tok_seq_create(&ts, s, &te, &tep, &te_msg)) goto fail_1;

	printf("Tokenization result:\n\n");

	for (const struct tok *tp = ts; tp->type != eot; ++tp) {
		print_tok(tp);
		printf("\n");
	}

	tok_seq_destroy(ts);
	return 0;

fail_1:
	if (te)	{
		printf("Token error at %u: %s\n", (unsigned) (tep - s), te_msg);
		free(te_msg);
	}
	else printf("Unexpected error!\n");
	return -1;
}

