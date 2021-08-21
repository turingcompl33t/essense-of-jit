# Makefile

CC     = gcc
CFLAGS = -Wall -Werror -std=gnu11 -ggdb

default: driver0 driver1 driver2

driver0: driver0.c error.c
	$(CC) $(CFLAGS) $^ -o $@

driver1: driver1.c jit.c error.c
	$(CC) $(CFLAGS) $^ -o $@

driver2: driver2.c jit.c error.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o
	rm -f driver0
	rm -f driver1
	rm -f driver2

.PHONY: clean
