# Makefile

CC      = gcc
CFLAGS  = -Wall -Werror -std=gnu11 -ggdb
LDFLAGS = -ldl

default: driver

OBJS = jit.o error.o loader.o

error.o: error.c error.h
jit.o: jit.c jit.h
loader.o: loader.c loader.h

driver: driver.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o
	rm -f driver

.PHONY: clean
