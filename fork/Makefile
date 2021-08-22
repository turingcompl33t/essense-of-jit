# Makefile

CC      = gcc
CFLAGS  = -Wall -Werror -std=gnu11 -ggdb
LDFLAGS = -ldl

OBJS = error.o gcc.o io.o jit.o loader.o subprocess.o

default: driver0 driver1 driver2

error.o: error.c error.h
gcc.o: gcc.c gcc.h
io.o: io.c io.h
jit.o: jit.c jit.h
loader.o: loader.c loader.h
subprocess.o: subprocess.c subprocess.h

# Static library for JIT
libjit.a: $(OBJS)
	ar cr -o $@ $(OBJS)

driver0: driver0.c libjit.a
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

driver1: driver1.c libjit.a
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# NOTE: use -Wl,--export-dynamic to export all symbols
# from the program so they are available to JITed code
driver2: driver2.c libjit.a
	$(CC) $(CFLAGS) -Wl,--export-dynamic $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o
	rm -f driver0
	rm -f driver1
	rm -f driver2
	rm -f libjit.a

.PHONY: clean
