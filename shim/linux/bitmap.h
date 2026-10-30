/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * The nine bit operations xarray uses, on gcc builtins. Marks live in
 * unsigned long tags[XA_MAX_MARKS][XA_MARK_LONGS] inside each node.
 * All of these are the non-atomic (__ prefixed) forms: the caller holds
 * the xa_lock.
 */
#ifndef _LINUX_BITMAP_H
#define _LINUX_BITMAP_H

#include <string.h>
#include <linux/types.h>

#define BITS_PER_LONG		(__CHAR_BIT__ * __SIZEOF_LONG__)
#define BITS_TO_LONGS(n)	(((n) + BITS_PER_LONG - 1) / BITS_PER_LONG)
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)

static inline unsigned long __ffs(unsigned long word)
{
	return __builtin_ctzl(word);
}

static inline bool test_bit(unsigned long nr, const unsigned long *addr)
{
	return (addr[BIT_WORD(nr)] >> (nr % BITS_PER_LONG)) & 1;
}

static inline void __set_bit(unsigned long nr, unsigned long *addr)
{
	addr[BIT_WORD(nr)] |= BIT_MASK(nr);
}

static inline bool __test_and_set_bit(unsigned long nr, unsigned long *addr)
{
	unsigned long *w = addr + BIT_WORD(nr), old = *w;

	*w = old | BIT_MASK(nr);
	return (old & BIT_MASK(nr)) != 0;
}

static inline bool __test_and_clear_bit(unsigned long nr, unsigned long *addr)
{
	unsigned long *w = addr + BIT_WORD(nr), old = *w;

	*w = old & ~BIT_MASK(nr);
	return (old & BIT_MASK(nr)) != 0;
}

/* First set bit at or after @offset, or @size if none. */
static inline unsigned long find_next_bit(const unsigned long *addr,
		unsigned long size, unsigned long offset)
{
	while (offset < size) {
		unsigned long w = addr[BIT_WORD(offset)] >> (offset % BITS_PER_LONG);

		if (w)
			return offset + __builtin_ctzl(w) < size ?
			       offset + __builtin_ctzl(w) : size;
		offset = (BIT_WORD(offset) + 1) * BITS_PER_LONG;
	}
	return size;
}

static inline void bitmap_fill(unsigned long *dst, unsigned int nbits)
{
	memset(dst, 0xff, BITS_TO_LONGS(nbits) * sizeof(unsigned long));
}

static inline bool bitmap_empty(const unsigned long *src, unsigned int nbits)
{
	return find_next_bit(src, nbits, 0) == nbits;
}

/* Clear @len bits starting at @start. */
static inline void bitmap_clear(unsigned long *map, unsigned int start,
		unsigned int len)
{
	for (unsigned int i = start; i < start + len; i++)
		map[BIT_WORD(i)] &= ~BIT_MASK(i);
}

#endif /* _LINUX_BITMAP_H */
