#include "vector.h"

void print_int_vector(struct vector *v)
{
	int *arr = vector_get_vals(v);

	printf(
		"max_num_vals = %u, num_vals = %u",
		(unsigned) vector_get_max_num_vals(v),
		(unsigned) vector_get_num_vals(v)
	);

	printf(", vals = {");
	for (int i = 0; i != vector_get_num_vals(v); ++i) {
		if (i != 0) printf(", ");
		printf("%d", arr[i]);
	}
	printf("}\n");
}

int main()
{
	struct vector v;
	if (!vector_create(&v, sizeof(int), 2, 3)) return -1;
	print_int_vector(&v);

	printf("\n");

	vector_shrink(&v);
	print_int_vector(&v);

	printf("\n");

	int x = 2000000000;
	for(size_t i = 1; i <= 10; ++i) {
		++x;
		if (!vector_push_back(&v, &x)) {
			vector_destroy(&v);
			return -2;
		}
		print_int_vector(&v);
	}

	printf("\n");

	int *arr = (int *) vector_get_vals(&v);
	for (size_t i = 0; i < 10; ++i) ++arr[i];
	print_int_vector(&v);

	printf("\n");

	vector_shrink(&v);
	print_int_vector(&v);

	printf("\n");

	x = 2100100100;
	if (!vector_push_back(&v, &x)) {
		vector_destroy(&v);
		return -3;
	}
	print_int_vector(&v);

	vector_destroy(&v);

	printf("\n");

	struct vector v2;
	if (!vector_create(&v2, sizeof(int), 2, 2)) return -4;
	print_int_vector(&v2);

	printf("\n");

	x = 2;
	if (!vector_push_back(&v2, &x)) {
		vector_destroy(&v2);
		return -5;
	}
	print_int_vector(&v2);

	printf("\n");

	x = 3;
	if (!vector_push_back(&v2, &x)) {
		vector_destroy(&v2);
		return -6;
	}
	print_int_vector(&v2);

	printf("\n");

	x = 5;
	if (!vector_push_back(&v2, &x)) {
		vector_destroy(&v2);
		return -7;
	}
	print_int_vector(&v2);

	printf("\n");

	int *arr2;
	size_t arr_size;
	arr2 = vector_move(&v2, &arr_size);
	printf("{");
	for (int i = 0; i != arr_size; ++i) {
		if (i != 0) printf(", ");
		printf("%d", arr2[i]);
	}
	printf("}\n");
	free(arr2);

	return 0;
}

