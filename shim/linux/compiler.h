/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Compiler hints and sparse annotations used by xarray. */
#ifndef _LINUX_COMPILER_H
#define _LINUX_COMPILER_H

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

#ifndef __always_inline	/* glibc sys/cdefs.h defines the same thing */
#define __always_inline	inline __attribute__((always_inline))
#endif
#define __must_check	__attribute__((warn_unused_result))

/* sparse-only annotations */
#define __rcu
#define __must_hold(lock)

#define barrier()	__asm__ __volatile__("" : : : "memory")

#endif /* _LINUX_COMPILER_H */
