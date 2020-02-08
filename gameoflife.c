#include <stdio.h>
#include "gol.h"

int main(int argc, char* argv[]) {
  FILE* infile = stdin;
  FILE* outfile = stdout;
  int generations = 5;
  int statistics = 0;
  int torus = 0;

  struct universe v;
  v.alive_cumulative = 0;
  v.total_cumulative = 0;

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
  write_out_file(stdout, &v);
  return 0;
}
