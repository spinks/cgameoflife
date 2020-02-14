CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11

gameoflife:
	$(CC) $(CFLAGS) -shared -fPIC gol.c -o libgol.so 
	$(CC) $(CFLAGS) -L. -lgol gameoflife.c -o $@ $<

clean:
	-rm -f gameoflife libgol.so
