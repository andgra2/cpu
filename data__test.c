#include "data.h"

static void print_data(struct data *d)
{
	printf("%s", d->label);
	if (data_type(d) == data_type_array)
		printf("[%u]\n", (unsigned) data_array_len(d));
}

int main()
{
	int ret;

	struct data d1;
	if (!data_scalar_create(&d1, "x")) { ret = -1; goto out_1; }
	print_data(&d1);

	printf("\n");

	struct data d2;
	if (!data_array_create(&d2, "arr", 10)) { ret = -2; goto out_2; }
	print_data(&d2);

	ret = 0;

	data_destroy(&d2);
out_2:
	data_destroy(&d1);
out_1:
	return ret;
}

