// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * The two symbols lib/linux/xarray.c expects lib/radix-tree.c to provide
 * (radix_tree_node_cachep, radix_tree_node_rcu_free), plus the kmem_cache
 * calls behind them, on malloc.
 */
#include <stdlib.h>
#include <string.h>

#include <linux/list.h>
#include <linux/slab.h>
#include <linux/xarray.h>
#include <linux/radix-tree.h>

static void xa_node_ctor(void *p)
{
	struct xa_node *node = p;

	memset(node, 0, sizeof(*node));
	INIT_LIST_HEAD(&node->private_list);
}

/* What radix_tree_init() builds at boot; here it is a compile-time constant. */
static struct kmem_cache xa_node_cache = {
	.size  = sizeof(struct xa_node),
	.align = sizeof(void *),
	.ctor  = xa_node_ctor,
};
struct kmem_cache *radix_tree_node_cachep = &xa_node_cache;

/* ponytail: malloc per node; add a free list if allocation shows in profiles */
void *kmem_cache_alloc_lru(struct kmem_cache *c, struct list_lru *lru, gfp_t gfp)
{
	void *p;

	(void)lru; (void)gfp;
	if (posix_memalign(&p, c->align, c->size))
		return NULL;
	if (c->ctor)
		c->ctor(p);
	else
		memset(p, 0, c->size);
	return p;
}

void kmem_cache_free(struct kmem_cache *c, void *p)
{
	(void)c;
	free(p);
}

/*
 * The kernel re-zeroes the node here because slab hands constructed objects
 * back out without rerunning the ctor. Our alloc always runs it, so just free.
 */
void radix_tree_node_rcu_free(struct rcu_head *head)
{
	struct xa_node *node = caa_container_of(head, struct xa_node, rcu_head);

	kmem_cache_free(radix_tree_node_cachep, node);
}
