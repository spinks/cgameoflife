#include <stdio.h>
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
          printf("Unknown option -%c\n\n", argv[i][1]);
          break;
        case 'i':
          infile = fopen(argv[i + 1], "r");
          break;
        case 'o':
          outfile = fopen(argv[i + 1], "w");
          break;
        case 'g':
          generations = atoi(argv[i + 1]);
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

  struct universe v = {.alive_cumulative = 0, .total_cumulative = 0};

  read_in_file(infile, &v);
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
