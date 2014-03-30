#include "vector.h"

void print_int_vector(struct vector *v)
{
	int *arr = v->vals;

	printf(
		"max_num_vals = %u, num_vals = %u",
		(unsigned) vector_max_num_vals(v),
		(unsigned) vector_num_vals(v)
	);

	printf(", vals = {");
	for (int i = 0; i != vector_num_vals(v); ++i) {
		if (i != 0) printf(", ");
		printf("%d", arr[i]);
	}
	printf("}\n");
}

int main()
{
	int ret;

	struct vector v;
	if (!vector_create(&v, sizeof(int), 2, 3)) { ret = -1; goto out_1; }
	print_int_vector(&v);

	printf("\n");

	vector_shrink(&v);
	print_int_vector(&v);

	printf("\n");

	int x = 2000000000;
	for(size_t i = 1; i <= 10; ++i) {
		++x;
		if (!vector_push_back(&v, &x)) { ret = -2; goto out_2;}
		print_int_vector(&v);
	}

	printf("\n");

	int *arr = (int *) vector_vals(&v);
	for (size_t i = 0; i < 10; ++i) ++arr[i];
	print_int_vector(&v);

	printf("\n");

	vector_shrink(&v);
	print_int_vector(&v);

	printf("\n");

	x = 2100100100;
	if (!vector_push_back(&v, &x)) {ret = -3; goto out_2;}
	print_int_vector(&v);

	ret = 0;
out_2:
	vector_destroy(&v);
out_1:
	return ret;
}
