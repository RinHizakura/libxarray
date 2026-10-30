/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * spinlock_t on a pthread mutex. Userspace has no interrupts or softirqs,
 * so the _irq/_bh/_irqsave variants are all the plain lock.
 * ponytail: mutex sleeps under contention; switch to pthread_spinlock_t
 * if profiling shows the xa_lock hot.
 */
#ifndef _LINUX_SPINLOCK_H
#define _LINUX_SPINLOCK_H

#include <pthread.h>

typedef pthread_mutex_t spinlock_t;

#define __SPIN_LOCK_UNLOCKED(name)	PTHREAD_MUTEX_INITIALIZER
#define DEFINE_SPINLOCK(name)		spinlock_t name = PTHREAD_MUTEX_INITIALIZER
#define spin_lock_init(l)		pthread_mutex_init(l, NULL)

#define spin_lock(l)			pthread_mutex_lock(l)
#define spin_unlock(l)			pthread_mutex_unlock(l)
#define spin_trylock(l)			(pthread_mutex_trylock(l) == 0)

#define spin_lock_bh(l)			spin_lock(l)
#define spin_unlock_bh(l)		spin_unlock(l)
#define spin_lock_irq(l)		spin_lock(l)
#define spin_unlock_irq(l)		spin_unlock(l)
#define spin_lock_irqsave(l, flags)	do { (void)(flags); spin_lock(l); } while (0)
#define spin_unlock_irqrestore(l, flags)	do { (void)(flags); spin_unlock(l); } while (0)

/* _nested: lockdep subclass hints, irrelevant without lockdep */
#define spin_lock_nested(l, sub)		spin_lock(l)
#define spin_lock_bh_nested(l, sub)		spin_lock(l)
#define spin_lock_irq_nested(l, sub)		spin_lock(l)
#define spin_lock_irqsave_nested(l, flags, sub)	spin_lock_irqsave(l, flags)

#endif /* _LINUX_SPINLOCK_H */
