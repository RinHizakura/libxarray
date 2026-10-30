/* SPDX-License-Identifier: GPL-2.0-or-later */
/* BUG/WARN and printk: diagnostics go to stderr, BUG aborts. */
#ifndef _LINUX_BUG_H
#define _LINUX_BUG_H

#include <stdio.h>
#include <stdlib.h>

#define BUG() do {							\
	fprintf(stderr, "BUG at %s:%d\n", __FILE__, __LINE__);		\
	abort();							\
} while (0)
#define BUG_ON(cond) do { if (cond) BUG(); } while (0)

/* Evaluates to the condition so callers can write: if (WARN_ON(x)) ... */
#define WARN_ON(cond) ({						\
	int __warned = !!(cond);					\
	if (__warned)							\
		fprintf(stderr, "WARNING at %s:%d: %s\n",		\
			__FILE__, __LINE__, #cond);			\
	__warned;							\
})
/* ponytail: warns every time; add a static flag if it ever gets noisy */
#define WARN_ON_ONCE(cond)	WARN_ON(cond)

#define printk(...)	fprintf(stderr, __VA_ARGS__)
#define pr_info		printk
#define pr_cont		printk
#define pr_err		printk

#endif /* _LINUX_BUG_H */
