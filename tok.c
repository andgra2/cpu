#include "tok.h"

static void tokenize_eat_ws(const char **cp);
static bool tokenize(const char **cp, struct tok *t, bool *te, char **te_msg);
static bool tokenize_lit_numb(const char **cp, bool *tm, struct tok_lit_numb *t, bool *te, char **te_msg);
static bool tokenize_lit_ch(const char **cp, bool *tm, struct tok_lit_ch *t, bool *te, char **te_msg);
static bool tokenize_lit_str(const char **cp, bool *tm, struct tok_lit_str *t, bool *te, char **te_msg);
static bool tokenize_lit_ch_str_elem(const char **cp, char *c, bool *te, char **te_msg);
static bool tokenize_lit_ch_str_elem_esc_code(const char **cp, bool *m, char *c, bool *te, char **te_msg);
static void tokenize_lit_ch_str_elem_esc_symb(const char **cp, bool *m, char *c);
static bool tokenize_ident(const char **cp, bool *tm, struct tok_ident *t);
static void tokenize_punct_op(const char **cp, bool *tm, struct tok_punct_op *t);
static void tok_vector_destroy(struct vector *tv);
static void tok_destroy(struct tok *t);
static void tok_lit_str_destroy(struct tok_lit_str *t);
static void tok_ident_destroy(struct tok_ident *t);
static bool tok_err(bool *te, char **te_msg, const char *te_msg_src);

extern bool tok_seq_create(struct tok **ts, const char *cs, bool *te, const char **tep, char **te_msg)
{
	struct vector tv;
	if (!vector_create(&tv, sizeof(struct tok), 1000, 1000)) goto fail_1;
	struct tok t;
	do {
		tokenize_eat_ws(&cs);

		if (!tokenize(&cs, &t, te, te_msg)) goto fail_2;

		if (!vector_push_back(&tv, &t)) goto fail_3;
	} while (t.type != eot);

	*ts = vector_move(&tv, NULL);
	return true;

fail_1:
	*te = false;
	return false;
fail_2:
	*tep = cs;
	tok_vector_destroy(&tv);
	return false;
fail_3:
	tok_destroy(&t);
	tok_vector_destroy(&tv);
	*te = false;
	return false;
}

extern void tok_seq_destroy(struct tok *ts)
{
	for (struct tok *tp = ts; tp->type != eot; ++tp) tok_destroy(tp);

	free(ts);
}

static void tokenize_eat_ws(const char **cp)
{
	while (char_is_ws(**cp)) ++*cp;
}

static bool tokenize(const char **cp, struct tok *t, bool *te, char **te_msg)
{
	if (**cp == '\0') {
		t->type = eot;
		return true;
	}

	bool tm;

	if (!tokenize_lit_numb(cp, &tm, &t->lit_numb, te, te_msg)) goto fail_1;
	if (tm) {
		t->type = lit_numb;
		return true;
	}

	if (!tokenize_lit_ch(cp, &tm, &t->lit_ch, te, te_msg)) goto fail_1;
	if (tm) {
		t->type = lit_ch;
		return true;
	}

	if (!tokenize_lit_str(cp, &tm, &t->lit_str, te, te_msg)) goto fail_1;
	if (tm) {
		t->type = lit_str;
		return true;
	}

	if (!tokenize_ident(cp, &tm, &t->ident)) goto fail_2;
	if (tm) {
		t->type = ident;
		return true;
	}

	tokenize_punct_op(cp, &tm, &t->punct_op);
	if (tm) {
		t->type = punct_op;
		return true;
	}

	goto fail_3;

fail_1:
	return false;
fail_2:
	*te = false;
	return false;
fail_3:
	*te = true;
	if (!tok_err(te, te_msg, "Unknown token.")) goto fail_te;
	return false;
fail_te:
	*te = false;
	return false;
}

static bool tokenize_lit_numb(const char **cp, bool *tm, struct tok_lit_numb *t, bool *te, char **te_msg)
{
	const char *cp_start = *cp;

	unsigned b;
	if (**cp == '0') {
		switch ((*cp)[1])
		{
		case 'b':
		case 'B':
			b = 2;
			goto non_octal;
		case 'x':
		case 'X':
			b = 16;
			goto non_octal;
		non_octal:
			*cp += 2;
			if (!char_is_pseudo_letter(**cp)) goto fail_1;
			if (!char_is_dig(**cp, b)) goto fail_2;
			break;
		default:
			b = 8;
		}
	}
	else if (char_is_dec_dig(**cp)) b = 10;
	else {
		*tm = false;
		return true;
	}

	long unsigned val = char_dig_val(**cp);
	++*cp;
	while (char_is_pseudo_letter(**cp)) {
		if (!char_is_dig(**cp, b)) goto fail_3;

		val *= b, val += char_dig_val(**cp);
		if (val > UINT_MAX) goto fail_4;

		++*cp;
	}

	t->val = val;
	*tm = true;
	return true;

fail_1:
	if (!tok_err(te, te_msg, "Unexpected end of integer literal.")) goto fail_te;
	return false;
fail_2:
	if (!tok_err(te, te_msg, "Invalid digit found inside integer literal.")) goto fail_te;
	return false;
fail_3:
	if (!tok_err(te, te_msg, "Invalid digit found inside integer literal.")) goto fail_te;
	return false;
fail_4:
	*cp = cp_start;
	if (!tok_err(te, te_msg, "Integer literal is too large.")) goto fail_te;
	return false;
fail_te:
	*te = false;
	return false;
}

static bool tokenize_lit_ch(const char **cp, bool *tm, struct tok_lit_ch *t, bool *te, char **te_msg)
{
	const char *cp_start = *cp;

	if (**cp != '\'') {
		*tm = false;
		return true;
	}
	++*cp;

	if (**cp == '\'') goto fail_1;

	if (!tokenize_lit_ch_str_elem(cp, &t->c, te, te_msg)) goto fail_2;

	if (**cp != '\'') goto fail_3;
	++*cp;

	*tm = true;
	return true;

fail_1:
	*cp = cp_start;
	if (!tok_err(te, te_msg, "Empty character literal found.")) goto fail_te;
	return false;
fail_2:
	return false;
fail_3:
	if (!tok_err(te, te_msg, "Expected end of character literal.")) goto fail_te;
	return false;
fail_te:
	*te = false;
	return false;
}

static bool tokenize_lit_str(const char **cp, bool *tm, struct tok_lit_str *t, bool *te, char **te_msg)
{
	if (**cp != '"') {
		*tm = false;
		return true;
	}
	++*cp;

	struct vector sv;
	if (!vector_create(&sv, sizeof(char), 50, 50)) goto fail_1;
	char c;
	while (**cp != '"') {
		if (!tokenize_lit_ch_str_elem(cp, &c, te, te_msg)) goto fail_2;

		if (!vector_push_back(&sv, &c)) goto fail_3;
	}
	++*cp;

	size_t num_chars;
	t->s_beg = vector_move(&sv, &num_chars);
	t->s_end = t->s_beg + num_chars;
	*tm = true;
	return true;

fail_1:
	*te = false;
	return false;
fail_2:
	vector_destroy(&sv);
	return false;
fail_3:
	vector_destroy(&sv);
	*te = false;
	return false;
}

static bool tokenize_lit_ch_str_elem(const char **cp, char *c, bool *te, char **te_msg)
{
	if (**cp == '\\') {
		++*cp;

		bool m;

		if (!tokenize_lit_ch_str_elem_esc_code(cp, &m, c, te, te_msg)) goto fail_1;
		if (m) return true;

		tokenize_lit_ch_str_elem_esc_symb(cp, &m, c);
		if (m) return true;

		goto fail_2;
	}

	if (char_is_print(**cp) || **cp == '\t') {
		*c = **cp;
		++*cp;
		return true;
	}

	if (**cp == '\0') goto fail_3;
	if (**cp == '\n') goto fail_4;

	goto fail_5;

fail_1:
	return false;
fail_2:
	if (!tok_err(te, te_msg, "Unknown escape sequence.")) goto fail_te;
	return false;
fail_3:
	if (!tok_err(te, te_msg, "Unexpected end of character or string literal")) goto fail_te;
	return false;
fail_4:
	if (!tok_err(te, te_msg, "Found a new-line inside character or string literal")) goto fail_te;
	return false;
fail_5:
	if (!tok_err(te, te_msg, "Illegal character inside character or string literal")) goto fail_te;
	return false;
fail_te:
	*te = false;
	return false;
}

static bool tokenize_lit_ch_str_elem_esc_code(const char **cp, bool *m, char *c, bool *te, char **te_msg)
{
	const char *cp_start = *cp;

	unsigned b;
	size_t max_num_digs;
	if (**cp == 'x') {
		++*cp;
		if (!char_is_dig(**cp, 16)) goto fail_1;
		b = 16, max_num_digs = 2;
	}
	else if (char_is_oct_dig(**cp)) b = 8, max_num_digs = 3;
	else if (char_is_dec_dig(**cp)) goto fail_2;
	else {
		*m = false;
		return true;
	};

	unsigned val = char_dig_val(**cp);
	++*cp;
	for (size_t i = 1; i != max_num_digs; ++i) {
		if (!char_is_dig(**cp, b)) break;

		val *= b, val += char_dig_val(**cp);
		if (val > UCHAR_MAX) goto fail_3;

		++*cp;
	}

	*m = true;
	*c = val;
	return true;

fail_1:
	if (!tok_err(te, te_msg, "Unexpected end of hexadecimal escape sequence.")) goto fail_te;
	return false;
fail_2:
	if (!tok_err(te, te_msg, "Expected octal digit.")) goto fail_te;
	return false;
fail_3:
	*cp = cp_start;
	if (!tok_err(te, te_msg, "Overflow in octal escape sequence.")) goto fail_te;
	return false;
fail_te:
	*te = false;
	return false;
}

static void tokenize_lit_ch_str_elem_esc_symb(const char **cp, bool *m, char *c)
{
	switch (**cp)
	{
	case 'a':
		*c = '\a';
		goto esc_found;
	case 'b':
		*c = '\b';
		goto esc_found;
	case 'f':
		*c = '\f';
		goto esc_found;
	case 'n':
		*c = '\n';
		goto esc_found;
	case 'r':
		*c = '\r';
		goto esc_found;
	case 't':
		*c = '\t';
		goto esc_found;
	case 'v':
		*c = '\v';
		goto esc_found;
	case '\\':
		*c = '\\';
		goto esc_found;
	case '\'':
		*c = '\'';
		goto esc_found;
	case '"':
		*c = '"';
		goto esc_found;
	case '?':
		*c = '?';
		goto esc_found;
	esc_found:
		++*cp;
		*m = true;
	default:
		*m = false;
	}

	return;
}

static bool tokenize_ident(const char **cp, bool *tm, struct tok_ident *t)
{
	if (!(char_is_letter(**cp) || **cp == '_')) {
		*tm = false;
		return true;
	}

	struct vector v;
	if (!vector_create(&v, sizeof(char), 50, 50)) goto fail_1;
	char c;

	c = **cp;
	if (!vector_push_back(&v, &c)) goto fail_2;
	++*cp;

	while (char_is_pseudo_letter(**cp)) {
		c = **cp;
		if (!vector_push_back(&v, &c)) goto fail_3;
		++*cp;
	}

	c = '\0';
	vector_push_back(&v, &c);

	t->s = vector_move(&v, NULL);
	*tm = true;
	return true;

fail_1:
	return false;
fail_2:
	vector_destroy(&v);
	return false;
fail_3:
	vector_destroy(&v);
	return false;
}

static void tokenize_punct_op(const char **cp, bool *tm, struct tok_punct_op *t)
{
	switch (**cp)
	{
	case '(': case ')': case '[': case ']': case '{': case '}':
	case '.': case ',': case ':': case ';': case '=':
		t->c = **cp;
		++*cp;
		*tm = true;
		break;
	default:
		*tm = false;
	}

	return;
}

static void tok_vector_destroy(struct vector *tv)
{
	struct tok *toks = vector_get_vals(tv);
	size_t num_toks = vector_get_num_vals(tv);
	for (size_t i = 0; i != num_toks; ++i) tok_destroy(toks + i);

	vector_destroy(tv);
}

static void tok_destroy(struct tok *t)
{
	if (t->type == lit_str) tok_lit_str_destroy(&t->lit_str);
	if (t->type == ident) tok_ident_destroy(&t->ident);
}

static void tok_lit_str_destroy(struct tok_lit_str *t)
{
	free(t->s_beg);
}

static void tok_ident_destroy(struct tok_ident *t)
{
	free(t->s);
}

static bool tok_err(bool *te, char **te_msg, const char *te_msg_src)
{
	*te = true;
	if ((*te_msg = str_clone(te_msg_src)) == NULL) goto fail_1;
	return true;

fail_1:
	return false;
}

