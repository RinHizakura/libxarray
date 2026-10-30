/* SPDX-License-Identifier: GPL-2.0-or-later */
/* might_alloc() is a lockdep/might_sleep hook; nothing to check here. */
#ifndef _LINUX_SCHED_MM_H
#define _LINUX_SCHED_MM_H

#define might_alloc(gfp)	do { } while (0)

#endif /* _LINUX_SCHED_MM_H */
