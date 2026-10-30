/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * RCU comes from liburcu. It already provides rcu_read_lock/unlock,
 * rcu_dereference, rcu_assign_pointer, call_rcu, synchronize_rcu,
 * rcu_barrier and struct rcu_head with kernel-compatible names.
 * Every thread must call rcu_register_thread() before touching an XArray.
 */
#ifndef _LINUX_RCUPDATE_H
#define _LINUX_RCUPDATE_H

#include <urcu.h>

#define rcu_dereference_raw(p)		rcu_dereference(p)
#define rcu_dereference_check(p, c)	rcu_dereference(p)
/* "protected" means the caller holds the lock, so a plain read is correct */
#define rcu_dereference_protected(p, c)	(p)
#define RCU_INIT_POINTER(p, v)		do { (p) = (v); } while (0)

#endif /* _LINUX_RCUPDATE_H */
