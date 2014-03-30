#include "mem.h"

const cell pc_null = 0x00000000;

static const size_t max_num_blocks = 1 << 16;
static const size_t block_size = 1 << 16;
static const size_t bi_first_usable = 1;
static const size_t bi_last_usable = (1 << 16) - 2;
static const size_t bi_first_free_when_no_free_blocks = (1 << 16) - 1;

static cell pc_from_bi(size_t bi) { return bi << 16; }

static size_t bi_from_pc(cell pc) { return pc >> 16; }

static size_t ci_from_pc(cell pc) { return pc & 0x0000FFFF; }

bool mem_create(struct mem *m)
{
	if ((m->blocks = malloc(max_num_blocks * sizeof(cell *))) == NULL)
		return false;

	for (size_t bi = 0; bi < max_num_blocks; ++bi)
		m->blocks[bi] = NULL;

	m->bi_first_free = bi_first_usable;

	return true;
}

void mem_destroy(struct mem *m)
{
	for (size_t bi = bi_first_usable; bi <= bi_last_usable; ++bi)
		free(m->blocks[bi]);

	free(m->blocks);
}

cell mem_new_block(struct mem *m)
{
	if (m->bi_first_free == bi_first_free_when_no_free_blocks)
		return pc_null;

	size_t new_bi = m->bi_first_free;
	if ((m->blocks[new_bi] = malloc(block_size * sizeof(cell))) == NULL)
		return pc_null;

	while (m->blocks[++m->bi_first_free] != NULL)
		;

	return pc_from_bi(new_bi);
}

void mem_delete_block(struct mem *m, cell pc)
{
	free(m->blocks[bi_from_pc(pc)]);
	m->blocks[bi_from_pc(pc)] = NULL;
}

cell *mem_ptr(struct mem *m, cell pc)
{
	return m->blocks[bi_from_pc(pc)] + ci_from_pc(pc);
}

void mem_state_debug(struct mem *m)
{
	printf("*** MEM STATE ***\n");
	printf("Constants:\n");
	printf("max_num_blocks\t\t%d\n", (int)max_num_blocks);
	printf("block_size\t\t%d\n", (int)block_size);
	printf("bi_first_usable\t\t%d\n", (int)bi_first_usable);
	printf("bi_last_usable\t\t%d\n", (int)bi_last_usable);
	printf("bi_last_usable\t\t%d\n", (int)bi_first_free_when_no_free_blocks);
	printf("State:\n");

	// find used blocks
	for (size_t bi = 0; bi <= max_num_blocks; bi++)
		if (m->blocks[bi] != NULL)
		{
			printf("  block: \t\t%d\n", (int)bi);

			for (int i = 0; i < block_size; i++)
			{
				cell c = (cell)*mem_ptr(m, pc_from_bi(bi) + i);
				if (c != pc_null)
					printf("    %d\t\t%d\n", pc_from_bi(bi) + i, c);
			}
		}

	printf("****************\n");
}

void mem_to_file(struct mem *m, const char *filename)
{
	bool blocks_used[max_num_blocks];

	FILE *file = fopen(filename, "wb");

	// find used blocks
	for (size_t bi = 0; bi <= max_num_blocks; bi++)
		blocks_used[bi] = m->blocks[bi] != NULL;

	fwrite(blocks_used, sizeof(bool), sizeof(blocks_used) / sizeof(bool), file);

	//cell c;

	// write blocks
	int b = sizeof(blocks_used) / sizeof(bool);

	printf("%d blocks written.\n", b);

	for (size_t bi = 0; bi <= max_num_blocks; bi++)
	{
		if (blocks_used[bi])
		{
			for (int i = 0; i < block_size; i++)
			{
				cell c = (cell)*mem_ptr(m, pc_from_bi(bi) + i);

				if (c > 0) {
					printf("written: %d\n", c);
				}

				fwrite(&c, sizeof(cell), 1, file);
			}
		}
	}

	fclose(file);
}

void mem_create_from_file (struct mem *m, const char *filename)
{
	mem_create(m);

	FILE *file = fopen(filename, "r");

	bool blocks_used[max_num_blocks];

	fread(&blocks_used, sizeof(blocks_used), 1, file);

	for (int i = 0; i < max_num_blocks; i++)
	{
		if (blocks_used[i])
		{
			cell new_block = mem_new_block(m);

			for (int c = 0; c < block_size; c++)
			{
				cell cl = -1;;

				fread(&cl, sizeof(cell), 1, file);

				*mem_ptr(m, new_block + i) = c;
			}
		}
	}

	fclose(file);
}
