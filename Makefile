CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11

gameoflife: gameoflife.c libgol.so gol.c
	$(CC) $(CFLAGS) -L. -lgol -o $@ $<

libgol.so: gol.c
	$(CC) $(CFLAGS) -shared -fPIC gol.c -o libgol.so 

clean:
	-rm -f gameoflife libgol.so
