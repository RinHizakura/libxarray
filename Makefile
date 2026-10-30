# vbd - branchable, lazy-fetching, time-travel block device built on the kernel XArray

CC      ?= gcc
CFLAGS  ?= -O2 -g -Wall
# include/ public API, shim/ kernel replacements, lib/ unmodified kernel code
KCFLAGS  = -Iinclude -Ishim -Ilib -D_LGPL_SOURCE
LDLIBS   = -lurcu -lpthread

LIB      = build/libxarray.a
LIB_OBJS = build/xarray.o build/kshim.o

.PHONY: all test clean
all: $(LIB)

$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

build/xarray.o: lib/linux/xarray.c | build
	$(CC) $(CFLAGS) $(KCFLAGS) -c $< -o $@

build/kshim.o: shim/kshim.c | build
	$(CC) $(CFLAGS) $(KCFLAGS) -c $< -o $@

build/xarray_smoke: tests/xarray_smoke.c $(LIB) | build
	$(CC) $(CFLAGS) $(KCFLAGS) $< $(LIB) $(LDLIBS) -o $@

test: build/xarray_smoke
	./$<

build:
	mkdir -p $@

clean:
	rm -rf build
