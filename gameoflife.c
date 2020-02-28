#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gol.h"

int main(int argc, char* argv[]) {
  FILE* inFile = stdin;
  char* inFileName;
  int inFlag = 0;

  FILE* outFile = stdout;
  char* outFileName;
  int outFlag = 0;

  int generations = 5;
  int genFlag = 0;

  int statistics = 0;
  int torus = 0;

  for (int i = 1; i != argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
        default:
          fprintf(stderr, "Unknown option -%c\n\n", argv[i][1]);
          return 1;
          break;
        case 'i':
          if (i + 1 == argc) {
            fprintf(stderr, "No infile passed (-i)\n");
            return 1;
          };
          if (inFlag) {
            if (strcmp(inFileName, argv[i + 1])) {
              fprintf(stderr, "Multiple conflicting infile arguments passed\n");
              return 1;
            }
            continue;
          }
          inFileName = argv[i + 1];
          inFile = fopen(inFileName, "r");
          if (errno) {
            fprintf(stderr, "Error reading infile (-i): %s %s\n", inFileName,
                    strerror(errno));
            return 1;
          }
          inFlag = 1;
          // Increment past read file
          i++;
          break;
        case 'o':
          if (i + 1 == argc) {
            fprintf(stderr, "No outfile passed (-o)\n");
            return 1;
          };
          if (outFlag) {
            if (strcmp(outFileName, argv[i + 1])) {
              fprintf(stderr,
                      "Multiple conflicting outfile arguments passed\n");
              return 1;
            }
            continue;
          }
          outFileName = argv[i + 1];
          outFile = fopen(outFileName, "w");
          if (errno) {
            fprintf(stderr, "Error opening outfile (-o): %s\n",
                    strerror(errno));
            return 1;
          };
          outFlag = 1;
          // Increment past opened outfile
          i++;
          break;
        case 'g':
          if (genFlag) {
            int generationsNew = strtol(argv[i + 1], NULL, 10);
            if (errno) {
              fprintf(stderr, "Invalid generations number (-g): %s\n",
                      strerror(errno));
              return 1;
            };
            if (generationsNew != generations) {
              fprintf(stderr,
                      "Multiple conflicting generation arguments passed\n");
              return 1;
            }
            continue;
          }
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
          if (generations < 0) {
            fprintf(
                stderr,
                "Invalid generations number (must be positive >= 0) (-g)\n");
            return 1;
          }
          genFlag = 1;
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

  read_in_file(inFile, &v);
  if (errno) {
    return 1;
  };
  fclose(inFile);
  for (int i = 0; i != generations; i++) {
    if (torus == 1) {
      evolve(&v, will_be_alive_torus);
    } else {
      evolve(&v, will_be_alive);
    }
    if (errno) {
      fprintf(stderr, "Error encountered for evolve calls\n");
      return 1;
    };
  }
  write_out_file(outFile, &v);
  if (statistics == 1) {
    print_statistics(&v);
  }
  fclose(outFile);
  return 0;
}
