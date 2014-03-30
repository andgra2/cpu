#include "util.h"

int main()
{
	int ret;

	char s[] = "Hello!";
	char *sc;
	if ((sc = str_clone(s)) == NULL) { ret = -1; goto out_1; }
	printf("%s\n", sc);

	printf("\n");

	s[0] = 'h';
	sc[2] = 'L';
	printf("%s %s", s, sc);

	printf("\n");

	ret = 0;

	free(sc);
out_1:
	return ret;
}

