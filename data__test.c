#include "data.h"

static void print_data(struct data *d)
{
	printf("%s", data_get_label(d));

	switch (data_get_type(d))
	{
	case data_type_scalar:
		if (data_scalar_has_init_val(d))
			printf(" = %u", (unsigned) data_scalar_get_init_val(d));
		break;
	case data_type_array:
		printf("[%u]", (unsigned) data_array_get_num_vals(d));
		printf(" = {");
		cell *arr = data_array_get_init_vals(d);
		for (size_t i = 0; i < data_array_get_num_init_vals(d); ++i) {
			if (i != 0) printf(", ");
			printf("%u", (unsigned) arr[i]);
		}
		printf("}");
		break;
	}

	printf("\n");
}

int main()
{
	struct data ds;
	if (!data_scalar_create(&ds, "x")) return -1;
	print_data(&ds);

	printf("\n");

	data_scalar_set_init_val(&ds, 2000000001);
	print_data(&ds);

	data_destroy(&ds);

	printf("\n");

	struct data da;
	if (!data_array_create(&da, "arr")) return -2;
	data_array_set_num_vals(&da, 10);
	for (int n = 1; n <= 6; ++n) {
		if (!data_array_add_init_val(&da, (cell) n)) {
			data_destroy(&da);
			return -3;
		}
	}
	print_data(&da);

	data_destroy(&da);

	printf("\n");

	struct data da2;
	if (!data_array_create(&da2, "arr2")) return -3;
	data_array_set_num_vals(&da2, 5);
	print_data(&da2);

	data_destroy(&da2);
	
	return 0;
}

