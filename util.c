#include "util.h"

char *str_clone(const char *s)
{
	size_t size = (strlen(s) + 1) * sizeof(char);

	char *s_clone;
	if ((s_clone = malloc(size)) == NULL) return NULL;

	return strcpy(s_clone, s);
}

char *str_clone_sub(const char *beg, const char *end)
{
	size_t len = end - beg;

	char *clone;
	if ((clone = malloc((len + 1) * sizeof(char))) == NULL) return NULL;

	memcpy(clone, beg, len * sizeof(char));
	clone[len] = '\0';

	return clone;
}

