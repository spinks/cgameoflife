CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11
DEPS = gol.h
OBJ = gameoflife.o gol.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gameoflife: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	-rm -f $(OBJ)
	-rm -f gameoflife
