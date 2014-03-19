#include "memory.h"

const cell pc_null = 0x00000000;

static const size_t bs_size = 1 << 16;
static const size_t block_size = 1 << 16;
static const size_t bi_first_usable = 1;
static const size_t bi_last_usable = (1 << 16) - 2;
static const size_t bi_ff_when_no_free_blocks = (1 << 16) - 1;

static cell pc_from_bi(size_t bi) { return bi << 16; }

static size_t bi_from_pc(cell pc) { return pc >> 16; }

static size_t ci_from_pc(cell pc) { return pc & 0x0000FFFF; }

extern bool memory_create(struct memory *m)
{
	if ((m->bs = malloc(bs_size * sizeof(cell *))) == NULL)
		return false;

	for (size_t bi = 0; bi < bs_size; ++bi)
		m->bs[bi] = NULL;

	m->bi_ff = bi_first_usable;

	return true;
}

extern void memory_destroy(struct memory *m)
{
	for (size_t bi = bi_first_usable; bi <= bi_last_usable; ++bi) {
		free(m->bs[bi]);
	}

	free(m->bs);
}

extern cell memory_new_block(struct memory *m)
{
	if (m->bi_ff == bi_ff_when_no_free_blocks)
		return pc_null;

	size_t bi_nb = m->bi_ff;
	if ((m->bs[bi_nb] = malloc(block_size * sizeof(cell))) == NULL)
		return pc_null;

	while (m->bs[++m->bi_ff] != NULL)
		;

	return pc_from_bi(bi_nb);
}

extern void memory_delete_block(struct memory *m, cell pc)
{
	free(m->bs[bi_from_pc(pc)]);
	m->bs[bi_from_pc(pc)] = NULL;
}

extern cell memory_get(struct memory *m, cell pc)
{
	return m->bs[bi_from_pc(pc)][ci_from_pc(pc)];
}

extern void memory_set(struct memory *m, cell pc, cell c)
{
	m->bs[bi_from_pc(pc)][ci_from_pc(pc)] = c;
}

