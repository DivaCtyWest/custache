CC=gcc
CFLAGS=-c -Wall

all: custache

tags:
	ctags -R .

custache: custache.o main.c
	$(CC) -o custache custache.o main.c

custache.o: custache.c
	$(CC) $(CFLAGS) custache.c

tests: tests.o custache.o
	$(CC) -o tests tests.o custache.o

tests.o: tests.c
	$(CC) $(CFLAGS) tests.c

clean:
	@rm -rf *.o test autom4te.cache autoscan.log configure config.log

clobber: clean
	@rm -rvf custache tags

test: tests test.mustache test.json
	./tests
