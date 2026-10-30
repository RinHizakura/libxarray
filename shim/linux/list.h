/* SPDX-License-Identifier: GPL-2.0-or-later */
/* xarray only inits and tests xa_node.private_list (used by the page cache). */
#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#include <linux/types.h>

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

#endif /* _LINUX_LIST_H */
