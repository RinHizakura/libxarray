/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Kernel fixed-width types and annotations, mapped onto C99. */
#ifndef _LINUX_TYPES_H
#define _LINUX_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t  s32;
typedef int64_t  s64;

/* sparse annotations: no meaning to gcc */
#define __bitwise
#define __force

typedef unsigned int __bitwise gfp_t;

struct list_head {
	struct list_head *next, *prev;
};

#endif /* _LINUX_TYPES_H */
