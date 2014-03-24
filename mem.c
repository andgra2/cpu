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

