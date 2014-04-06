#include "tok.h"

static const char *tokenize_lit_numb_prefix(const char *p, unsigned *b)
{
	if (*p == '0') {
		if ((p[1] == 'x' || p[1] == 'X') && char_is_hex_dig(p[2])) {
			*b = 16;
			return p + 2;
		}

		*b = 8;
		return p;
	}

	if (char_is_dec_dig(*p)) {
		*b = 10;
		return p;
	}

	return NULL;
}

static const char *tokenize_lit_numb(const char *p, struct tok_lit_numb *t)
{
	unsigned b;
	if ((p = tokenize_lit_numb_prefix(p, &b)) == NULL) return NULL;

	long unsigned val = char_get_dig_val(*p);
	++p;
	while (char_is_dig_of_base(*p, b)) {
		val *= b;
		val += char_get_dig_val(*p);
		if (val > UINT_MAX) return NULL;
		++p;
	}

	t->val = val;
	return p;
}

static const char *tokenize_lit_ch_or_str_elem_hex(const char *p, char *c)
{
	if (!(*p == '\\' && p[1] == 'x' && char_is_hex_dig(p[2]))) return NULL;
	p += 2;

	unsigned x = char_get_dig_val(*p);
	++p;
	if (char_is_hex_dig(*p)) {
		x *= 16;
		x += char_get_dig_val(*p);
		++p;
	}

	*c = x;
	return p;
}

static const char *tokenize_lit_ch_or_str_elem_oct(const char *p, char *c)
{
	if (!(*p == '\\' && char_is_oct_dig(*p))) return NULL;
	++p;

	unsigned o = char_get_dig_val(*p);
	++p;
	size_t i = 1;
	while (char_is_oct_dig(*p) && i != 3) {
		o *= 8;
		o += char_get_dig_val(*p);
		if (o > 255) return NULL;
		++p, ++i;
	}

	*c = o;
	return p;
}

static const char *tokenize_lit_ch_or_str_elem_esc(const char *p, char *c)
{
	if (*p != '\\') return NULL;
	++p;

	switch (*p)
	{
	case '\'':
		*c = '\'';
		return p + 1;
	case '"':
		*c = '"';
		return p + 1;
	case '?':
		*c = '?';
		return p + 1;
	case '\\':
		*c = '\\';
		return p + 1;
	case 'a':
		*c = '\a';
		return p + 1;
	case 'b':
		*c = '\b';
		return p + 1;
	case 'f':
		*c = '\f';
		return p + 1;
	case 'n':
		*c = '\n';
		return p + 1;
	case 'r':
		*c = '\r';
		return p + 1;
	case 't':
		*c = '\t';
		return p + 1;
	case 'v':
		*c = '\v';
		return p + 1;
	}

	return NULL;
}

static const char *tokenize_lit_ch_or_str_elem_raw(const char *p, char *c)
{
	if (!((char_is_print(*p) && *p != '\\') || *p == '\t')) return NULL;

	*c = *p;
	return p + 1;
}

static const char *tokenize_lit_ch_or_str_elem(const char *p, char *c)
{
	const char *p_new;
	
	if ((p_new = tokenize_lit_ch_or_str_elem_hex(p, c)) != NULL)
		return p_new;
	
	if ((p_new = tokenize_lit_ch_or_str_elem_oct(p, c)) != NULL)
		return p_new;
	
	if ((p_new = tokenize_lit_ch_or_str_elem_esc(p, c)) != NULL)
		return p_new;

	if ((p_new = tokenize_lit_ch_or_str_elem_raw(p, c)) != NULL)
		return p_new;

	return NULL;
}

static const char *tokenize_lit_ch(const char *p, struct tok_lit_ch *t)
{
	if (!(*p == '\'' && p[1] != '\'')) return NULL;
	++p;
	
	if ((p = tokenize_lit_ch_or_str_elem(p, &t->c)) == NULL) return NULL;

	if (*p != '\'') return NULL;
	
	return p + 1;
}

static const char *tokenize_lit_str(const char *p, struct tok_lit_str *t, bool *ok)
{
	if (*p != '"') {
		*ok = true;
		return NULL;
	}
	++p;

	struct vector vec;
	if (!vector_create(&vec, sizeof(char), 10, 10)) {
		*ok = false;
		return NULL;
	}
	char c;
	while (*p != '"') {
		if ((p = tokenize_lit_ch_or_str_elem(p, &c)) == NULL) {
			*ok = true;
			return NULL;
		}

		if (!vector_push_back(&vec, &c)) {
			vector_destroy(&vec);
			*ok = false;
			return NULL;
		};
	}

	c = '\0';
	if (!vector_push_back(&vec, &c)) {
		vector_destroy(&vec);
		*ok = false;
		return NULL;
	}

	size_t num_chars;
	t->s_beg = vector_move(&vec, &num_chars);
	t->s_end = t->s_beg + num_chars;

	*ok = true;
	return p + 1;
}

static const char *tokenize_ident(const char *p)
{
	if (!(char_is_letter(*p) || *p == '_')) return NULL;
	++p;

	while (char_is_letter(*p)) ++p;

	return p;
}

static const char *tokenize_other(const char *p, struct tok_other *t)
{
	switch (*p)
	{
	case '.': case ',': case ':': case ';':
	case '{': case '}': case '[': case ']': case '(': case ')':
	case '=':
		t->c = *p;
		return p + 1;
	default:
		return NULL;
	}
}

static const char *tokenize(const char *p, struct tok *t, bool *ok) 
{
	const char *p_new;

	if ((p_new = tokenize_lit_numb(p, &t->lit_numb)) != NULL) {
		t->type = lit_numb;
		goto tok_found;
	}

	if ((p_new = tokenize_lit_ch(p, &t->lit_ch)) != NULL) {
		t->type = lit_ch;
		goto tok_found;
	}

	if ((p_new = tokenize_lit_str(p, &t->lit_str, ok)) != NULL) {
		t->type = lit_str;
		goto tok_found;
	}
	
	if (*ok == false) return NULL;

	if ((p_new = tokenize_ident(p)) != NULL) {
		t->type = ident;
		goto tok_found;
	}

	if ((p_new = tokenize_other(p, &t->other)) != NULL) {
		t->type = other;
		goto tok_found;
	}

	*ok = true;
	return NULL;

tok_found:

	t->beg = p;
	t->end = p_new;

	*ok = true;
	return p_new;
}

static const char *tokenize_eat_ws(const char *p)
{
	while (char_is_ws(*p)) ++p;

	return p;
}

bool tok_seq_create(struct tok_seq *ts, const char *s, bool *ok, const char **tok_err_pos)
{
	struct vector vec;
	if (!vector_create(&vec, sizeof(struct tok), 10, 10)) {
		*ok = false;
		return false;
	}
	const char *p = s, *p_new;
	struct tok t;
	loop {
		p = tokenize_eat_ws(p);

		if (*p == '\0') break;

		if ((p_new = tokenize(p, &t, ok)) == NULL) {
			ts->toks = vector_move(&vec, &ts->num_toks);
			tok_seq_destroy(ts);
			if (ok) *tok_err_pos = p;
			return false;
		}

		p = p_new;

		if (!vector_push_back(&vec, &t)) {
			if (t.type == lit_str) free(t.lit_str.s_beg);
			ts->toks = vector_move(&vec, &ts->num_toks);
			tok_seq_destroy(ts);
			*ok = false;
			return false;
		}
	}

	ts->toks = vector_move(&vec, &ts->num_toks);
	*ok = true;
	return true;
}

void tok_seq_destroy(struct tok_seq *ts)
{
	for (size_t i = 0; i != ts->num_toks; ++i) {
		if (ts->toks[i].type == lit_str)
			free(ts->toks[i].lit_str.s_beg);
	}

	free(ts->toks);
}

