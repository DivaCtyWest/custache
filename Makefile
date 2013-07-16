CC=gcc
CFLAGS=-c -Wall

all: custache

custache: custache.o
	$(CC) custache.o -o custache

custache.o: custache.c
	$(CC) $(CFLAGS) custache.c

clean:
	rm -rvf *.o

clobber:
	rm -rvf custache

run: all
	./custache 'Hello {{name}}!\nYou have just won ${{value}}!\n{{#in_ca}}\n(Well, ${{taxed_value}}, after taxes.)\n{{/in_ca}}' '{"name": "JJ", "in_ca": "true", taxed_value: "6000.0", "value": 10000.0}'
