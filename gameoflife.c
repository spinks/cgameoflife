#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gol.h"

int main(int argc, char* argv[]) {
  FILE* infile = stdin;
  FILE* outfile = stdout;
  int generations = 5;
  int statistics = 0;
  int torus = 0;

  for (int i = 1; i != argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
        default:
          fprintf(stderr, "Unknown option -%c\n\n", argv[i][1]);
          break;
        case 'i':
          if (i + 1 == argc) {
            fprintf(stderr, "No infile passed (-i)\n");
            return 1;
          };
          infile = fopen(argv[i + 1], "r");
          if (errno) {
            fprintf(stderr, "Error reading infile (-i): %s\n", strerror(errno));
            return 1;
          };
          break;
        case 'o':
          if (i + 1 == argc) {
            fprintf(stderr, "No outfile passed (-o)\n");
            return 1;
          };
          outfile = fopen(argv[i + 1], "w");
          if (errno) {
            fprintf(stderr, "Error opening outfile (-o): %s\n",
                    strerror(errno));
            return 1;
          };
          break;
        case 'g':
          if (i + 1 == argc) {
            fprintf(stderr, "No generations number passed (-g)\n");
            return 1;
          };
          generations = strtol(argv[i + 1], NULL, 10);
          if (errno) {
            fprintf(stderr, "Invalid generations number (-g): %s\n",
                    strerror(errno));
            return 1;
          };
          break;
        case 's':
          statistics = 1;
          break;
        case 't':
          torus = 1;
          break;
      }
    }
  }

  struct universe v;

  read_in_file(infile, &v);
  if (errno) {
    return 1;
  };
  for (int i = 0; i != generations; i++) {
    if (statistics == 1) {
      print_statistics(&v);
    }
    if (torus == 1) {
      evolve(&v, will_be_alive_torus);
    } else {
      evolve(&v, will_be_alive);
    }
  }
  write_out_file(outfile, &v);
  return 0;
}
