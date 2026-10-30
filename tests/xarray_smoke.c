// SPDX-License-Identifier: GPL-2.0-or-later
/* Smallest check that the userspace XArray build actually works. */
#include <assert.h>
#include <stdio.h>
#include <xarray.h>

static DEFINE_XARRAY_ALLOC(ids);

int main(void)
{
	DEFINE_XARRAY(xa);
	unsigned long idx, count = 0;
	void *entry;
	u32 id;

	rcu_register_thread();

	/* store / load / erase on a sparse range (forces a multi-level tree) */
	assert(xa_store(&xa, 0, xa_mk_value(10), GFP_KERNEL) == NULL);
	assert(xa_store(&xa, 70000, xa_mk_value(70), GFP_KERNEL) == NULL);
	assert(xa_store(&xa, 1UL << 40, xa_mk_value(40), GFP_KERNEL) == NULL);
	assert(xa_to_value(xa_load(&xa, 70000)) == 70);
	assert(xa_load(&xa, 69999) == NULL);
	assert(xa_to_value(xa_erase(&xa, 0)) == 10);
	assert(xa_load(&xa, 0) == NULL);

	/* marks + marked iteration */
	xa_set_mark(&xa, 70000, XA_MARK_0);
	xa_for_each_marked(&xa, idx, entry, XA_MARK_0)
		count++;
	assert(count == 1);
	count = 0;
	xa_for_each(&xa, idx, entry)
		count++;
	assert(count == 2);
	xa_destroy(&xa);
	assert(xa_empty(&xa));

	/* id allocation */
	assert(xa_alloc(&ids, &id, xa_mk_value(1), xa_limit_32b, GFP_KERNEL) == 0 && id == 0);
	assert(xa_alloc(&ids, &id, xa_mk_value(2), xa_limit_32b, GFP_KERNEL) == 0 && id == 1);
	xa_erase(&ids, 0);
	assert(xa_alloc(&ids, &id, xa_mk_value(3), xa_limit_32b, GFP_KERNEL) == 0 && id == 0);
	xa_destroy(&ids);

	rcu_barrier();
	rcu_unregister_thread();
	puts("xarray smoke: ok");
	return 0;
}
