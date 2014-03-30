#include "util.h"

char *str_clone(const char *s)
{
	size_t size = (strlen(s) + 1) * sizeof(char);
	char *s_clone;
	if ((s_clone = malloc(size)) == NULL) return NULL;

	return strcpy(s_clone, s);
}

