CC=gcc
CFLAGS=-c -Wall

all: custache

custache: custache.o main.c
	$(CC) -o custache custache.o main.c

custache.o: custache.c
	$(CC) $(CFLAGS) custache.c

custache.test: custache.test.o custache.o
	$(CC) -o custache.test custache.test.o custache.o

custache.test.o: custache.test.c
	$(CC) $(CFLAGS) custache.test.c

clean:
	rm -rvf *.o
	rm -vf custache.test test.txt

clobber: clean
	rm -rvf custache

test: custache.test
	./custache.test

