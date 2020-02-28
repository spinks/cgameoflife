CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11

gameoflife: gameoflife.c libgol.so
	$(CC) $(CFLAGS) -L. -lgol -o $@ $<

libgol.so: gol.c gol.h
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $<

gol.o: gol.c gol.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

gameoflife.o: gameoflife.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f gameoflife gameoflife.o libgol.so gol.o
