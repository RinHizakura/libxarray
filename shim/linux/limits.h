/* SPDX-License-Identifier: GPL-2.0-or-later */
/* USHRT_MAX comes from the C library; max() is the kernel's type-checked one. */
#ifndef _LINUX_LIMITS_H
#define _LINUX_LIMITS_H

#include <limits.h>

#define max(a, b) ({						\
	__typeof__(a) __a = (a);				\
	__typeof__(b) __b = (b);				\
	(void)(&__a == &__b);	/* same type, or warn */	\
	__a > __b ? __a : __b;					\
})

#endif /* _LINUX_LIMITS_H */
