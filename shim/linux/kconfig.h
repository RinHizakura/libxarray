/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Kernel config: IS_ENABLED(CONFIG_X) is 1 iff CONFIG_X is defined as 1. */
#ifndef _LINUX_KCONFIG_H
#define _LINUX_KCONFIG_H

#define CONFIG_64BIT		1
#define CONFIG_XARRAY_MULTI	1	/* multi-index entries (large folios) */

#define __ARG_PLACEHOLDER_1 0,
#define __take_second_arg(__ignored, val, ...) val
#define ____is_defined(arg1_or_junk) __take_second_arg(arg1_or_junk 1, 0)
#define ___is_defined(val) ____is_defined(__ARG_PLACEHOLDER_##val)
#define __is_defined(x) ___is_defined(x)
#define IS_ENABLED(option) __is_defined(option)

#endif /* _LINUX_KCONFIG_H */
