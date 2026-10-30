/* SPDX-License-Identifier: GPL-2.0-or-later */
/* No lock validator in userspace: every "is held" check passes. */
#ifndef _LINUX_LOCKDEP_H
#define _LINUX_LOCKDEP_H

#define lockdep_is_held(lock)	(1)

#endif /* _LINUX_LOCKDEP_H */
