/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Slab cache for xa_node, implemented in shim/kshim.c on top of malloc. */
#ifndef _LINUX_SLAB_H
#define _LINUX_SLAB_H

#include <stddef.h>
#include <linux/gfp.h>

struct list_lru;	/* memcg shrinker hook, unused here */

struct kmem_cache {
	size_t size;
	size_t align;
	void (*ctor)(void *);
};

void *kmem_cache_alloc_lru(struct kmem_cache *c, struct list_lru *lru, gfp_t gfp);
void kmem_cache_free(struct kmem_cache *c, void *p);

#endif /* _LINUX_SLAB_H */
