CC=gcc
CFLAGS=-c -Wall

all: custache

custache: custache.o cJSON.o
	$(CC) -o custache custache.o cJSON.o

custache.o: custache.c
	$(CC) $(CFLAGS) custache.c

cJSON.o: cJSON/cJSON.c
	$(CC) $(CFLAGS) cJSON/cJSON.c

clean:
	rm -rvf *.o

clobber: clean
	rm -rvf custache

run: all
	./custache "Hello {{name}}!\nYou have just won €{{value}}!\n{{#in_ca}}\n(Well, €{{taxed_value}}, after taxes.)\n{{/in_ca}}\nWell done!" '{"name": "JJ", "in_ca": "true", taxed_value: "6000.0", "value": 10000.0}'
