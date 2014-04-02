#include "util.h"

int main()
{
	char s[] = "Hello!";
	char *sc;
	if ((sc = str_clone(s)) == NULL) return -1;
	printf("%s\n", sc);

	printf("\n");

	s[0] = 'h';
	sc[2] = 'L';
	printf("%s %s", s, sc);

	printf("\n");

	free(sc);

	char *sc2;
	if ((sc2 = str_clone_sub(s + 1, s + 5)) == NULL) return -2;
	printf("%s\n", sc2);

	free(sc2);

	return 0;
}

