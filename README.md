# libxarray

The Linux kernel's **XArray** as a userspace C library: a radix tree that
presents itself as a sparse, auto-growing array indexed by `unsigned long`.
RCU readers take no lock, writers take one spinlock per array, every slot
carries three mark bits, and `xa_alloc()` hands out unused indices.

`lib/linux/xarray.c` is the kernel source, unmodified. Everything it needs
from the rest of the kernel is supplied by a small set of hand-written
headers under `shim/`.

## Build

Requires `liburcu-dev`.

```sh
make        # build/libxarray.a
make test   # build and run tests/xarray_smoke
make clean
```

## Usage

Compile with `-Iinclude -Ishim -Ilib -D_LGPL_SOURCE`, link with
`build/libxarray.a -lurcu -lpthread`.

```c
#include <xarray.h>

static DEFINE_XARRAY(map);

int main(void)
{
	rcu_register_thread();          /* once per thread that touches an XArray */

	xa_store(&map, 42, xa_mk_value(7), GFP_KERNEL);
	unsigned long v = xa_to_value(xa_load(&map, 42));   /* 7 */

	xa_destroy(&map);
	rcu_barrier();
	rcu_unregister_thread();
	return 0;
}
```

The API is the kernel's: see the comments in `lib/linux/xarray.h` or
`Documentation/core-api/xarray.rst` in the kernel tree. `XA_CHUNK_SHIFT` is
the kernel default of 6 (64 slots per node).

## Layout

```
include/xarray.h         public header
shim/linux/*.h           replacements for the kernel headers lib/linux/ includes;
                         each provides only what xarray uses
shim/kshim.c             xa_node slab cache on malloc + RCU free callback
lib/linux/               unmodified Linux kernel files: xarray.c, xarray.h,
                         radix-tree.h
tests/xarray_smoke.c     store/load/erase, marks, iteration, xa_alloc
```

Kernel files are from torvalds/linux commit `d89e1d186ec9` (2026-10-29).

## How the kernel code runs in userspace

`lib/linux/xarray.c` is compiled as-is with `-Ishim` ahead of `-Ilib`, so
every `<linux/*.h>` it includes resolves to a shim header:

| Header | Kernel facility | Userspace replacement |
|---|---|---|
| `rcupdate.h` | RCU | liburcu; `rcu_dereference_protected` becomes a plain read |
| `spinlock.h` | `spinlock_t`, `_irq`/`_bh` variants | `pthread_mutex_t`; no interrupts, so all variants are the plain lock |
| `slab.h`, `shim/kshim.c` | `kmem_cache` for `xa_node` | `posix_memalign` + constructor |
| `gfp.h` | allocation flags | only `__GFP_DIRECT_RECLAIM` is inspected (may the allocator block?) |
| `bitmap.h` | 9 bit operations for marks | gcc builtins |
| `bug.h` | `BUG_ON`, `WARN_ON`, `printk` | `abort()`, `stderr` |
| `kconfig.h` | `IS_ENABLED(CONFIG_*)` | `CONFIG_64BIT`, `CONFIG_XARRAY_MULTI` |
| `lockdep.h`, `sched/mm.h` | lock validator hooks | no-ops |
| `compiler.h`, `types.h`, `err.h`, `limits.h`, `list.h`, `export.h` | annotations, types, errno | C99 / glibc |

## License

GPL-2.0-or-later (see `LICENSE`). Everything under `lib/linux/` is from the
Linux kernel and carries its original `GPL-2.0+` notices; the rest of the
project uses the same license so the whole tree is under one set of terms.
