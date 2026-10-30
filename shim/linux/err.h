/* SPDX-License-Identifier: GPL-2.0-or-later */
/* errno values (EBUSY, EINVAL, ENOMEM) and the error-pointer range. */
#ifndef _LINUX_ERR_H
#define _LINUX_ERR_H

#include <errno.h>

/* xa_is_err(): internal entries in [-MAX_ERRNO, -1] encode errors */
#define MAX_ERRNO	4095

#endif /* _LINUX_ERR_H */
