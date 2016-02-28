CC=gcc
CFLAGS=-c -Wall

all: custache

custache: custache.o main.c
	$(CC) -o custache custache.o main.c

custache.o: custache.c
	$(CC) $(CFLAGS) custache.c

tests: tests.o custache.o
	$(CC) -o tests tests.o custache.o

tests.o: tests.c
	$(CC) $(CFLAGS) tests.c

test.json:
	echo "{ \"foo\": \"bar\" }" > test.json

test.mustache:
	echo "Hello, {{foo}}" > test.mustache

clean:
	rm -rvf *.o
	rm -vf test test.mustache test.json

clobber: clean
	rm -rvf custache

test: tests test.mustache test.json
	./tests

