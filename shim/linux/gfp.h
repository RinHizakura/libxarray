/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Allocation flags. Only the bits xarray inspects matter: it checks
 * __GFP_DIRECT_RECLAIM to decide whether it may drop the lock and retry,
 * and it stores its own XA_FLAGS_* above __GFP_BITS_SHIFT in the same word.
 */
#ifndef _LINUX_GFP_H
#define _LINUX_GFP_H

#include <linux/types.h>

#define __GFP_IO		((__force gfp_t)0x40u)
#define __GFP_FS		((__force gfp_t)0x80u)
#define __GFP_ZERO		((__force gfp_t)0x100u)
#define __GFP_DIRECT_RECLAIM	((__force gfp_t)0x400u)
#define __GFP_KSWAPD_RECLAIM	((__force gfp_t)0x800u)
#define __GFP_HIGH		((__force gfp_t)0x20u)
#define __GFP_ACCOUNT		((__force gfp_t)0x400000u)
#define __GFP_RECLAIM		(__GFP_DIRECT_RECLAIM | __GFP_KSWAPD_RECLAIM)

#define __GFP_BITS_SHIFT	26

#define GFP_KERNEL	(__GFP_RECLAIM | __GFP_IO | __GFP_FS)
#define GFP_NOWAIT	(__GFP_KSWAPD_RECLAIM)
#define GFP_ATOMIC	(__GFP_HIGH | __GFP_KSWAPD_RECLAIM)

static inline bool gfpflags_allow_blocking(const gfp_t gfp)
{
	return !!(gfp & __GFP_DIRECT_RECLAIM);
}

#endif /* _LINUX_GFP_H */
