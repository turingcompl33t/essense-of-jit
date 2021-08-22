# Makefile

CC      = gcc
CFLAGS  = -Wall -Werror -std=gnu11 -ggdb
LDFLAGS = -ldl

OBJS = error.o gcc.o io.o jit.o loader.o subprocess.o

default: driver

error.o: error.c error.h
gcc.o: gcc.c gcc.h
io.o: io.c io.h
jit.o: jit.c jit.h
loader.o: loader.c loader.h
subprocess.o: subprocess.c subprocess.h

# Static library for JIT
libjit.a: $(OBJS)
	ar cr -o $@ $(OBJS)

driver: driver.c libjit.a
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o
	rm -f driver
	rm -f libjit.a

.PHONY: clean
