# test/Makefile

MAKE = make

CC     = gcc
CFLAGS = -Wall -Werror -std=gnu11 -ggdb

default: parent child

# NOTE: Link order matters! *facepalm*

parent: parent.c libjit.a 
	$(CC) $(CFLAGS) -I.. $^ -o $@ 

child: child.c libjit.a 
	$(CC) $(CFLAGS) -I.. $^ -o $@

libjit.a:
	$(MAKE) -C .. libjit.a
	cp ../libjit.a .

clean:
	rm -f *.o
	rm -f parent
	rm -f child
	rm -f libjit.a
	$(MAKE) -C .. clean
