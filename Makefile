CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11

gameoflife: gameoflife.c libgol.so
	$(CC) $(CFLAGS) -L. -lgol -o $@ $<

libgol.so: gol.o
	$(CC) $(CFLAGS) -shared gol.o -o libgol.so

gol.o: gol.c gol.h
	$(CC) $(CFLAGS) -fPIC -c gol.c

gameoflife.o: gameoflife.c libgol.so
	$(CC) $(CFLAGS) -L. -lgol -o $@ $<

clean:
	-rm -f gameoflife gameoflife.o libgol.so gol.o
