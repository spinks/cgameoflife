#include "gol.h"

void read_in_file(FILE *infile, struct universe *u) {
  u->alive_cumulative = 0;
  u->total_cumulative = 0;
  int lines = 0;
  int first_line_length = 0;
  int first_line_finished = 0;

  char ch;
  while (fscanf(infile, "%c", &ch) != EOF) {
    if (ch != '\n' && ch != '.' && ch != '*') {
      fprintf(stderr, "Invalid characters found in input.\n");
      errno = EINVAL;
      return;
    }
    if (ch == '\n') {
      lines++;
      first_line_finished = 1;
    }
    if (first_line_finished == 0) {
      first_line_length++;
    }
  }

  u->arr = malloc(lines * sizeof(*u->arr));
  for (int i = 0; i < lines; i++) {
    u->arr[i] = malloc(first_line_length * sizeof(u->arr[0]));
  }

  fseek(infile, 0L, SEEK_SET);
  int row = 0;
  int col = 0;
  while (fscanf(infile, "%c", &ch) != EOF) {
    if (ch == '\n') {
      if (col != first_line_length) {
        fprintf(stderr, "Line lengths not consistent.\n");
        errno = EINVAL;
        return;
      }
      row++;
      col = 0;
    } else {
      u->arr[row][col] = (ch == '.') ? 0 : 1;
      col++;
    }
  }

  u->rows = lines;
  u->cols = first_line_length;
  fclose(infile);
}

void write_out_file(FILE *outfile, struct universe *u) {
  for (int x = 0; x != u->rows; x++) {
    for (int y = 0; y != u->cols; y++) {
      char c = u->arr[x][y] ? '*' : '.';
      fputc(c, outfile);
    }
    fputs("\n", outfile);
  }
}

int is_alive(struct universe *u, int column, int row) {
  if (column < 0 || column >= u->cols) {
    fprintf(stderr, "%s", "Invalid column index\n");
    return -1;
  }
  if (row < 0 || row >= u->rows) {
    fprintf(stderr, "%s", "Invalid row index\n");
    return -1;
  }
  return u->arr[row][column];
}

int will_be_alive(struct universe *u, int column, int row) {
  if (column < 0 || column >= u->cols) {
    fprintf(stderr, "%s", "Invalid column index\n");
    return -1;
  }
  if (row < 0 || row >= u->rows) {
    fprintf(stderr, "%s", "Invalid row index\n");
    return -1;
  }
  int neighbors = 0;
  for (int x_off = -1; x_off != 2; x_off++) {
    for (int y_off = -1; y_off != 2; y_off++) {
      int nX = row + x_off;
      int nY = column + y_off;
      if (!(nX < 0 || nX >= u->rows || nY < 0 || nY >= u->cols) &&
          !(x_off == 0 && y_off == 0)) {
        neighbors += is_alive(u, nY, nX);
      }
    }
  }
  if (neighbors == 3) {
    return 1;
  } else if (neighbors == 2 && u->arr[row][column] == 1) {
    return 1;
  } else {
    return 0;
  }
}

int will_be_alive_torus(struct universe *u, int column, int row) {
  if (column < 0 || column >= u->cols) {
    fprintf(stderr, "%s", "Invalid column index\n");
    return -1;
  }
  if (row < 0 || row >= u->rows) {
    fprintf(stderr, "%s", "Invalid row index\n");
    return -1;
  }
  int neighbors = 0;
  for (int x_off = -1; x_off != 2; x_off++) {
    for (int y_off = -1; y_off != 2; y_off++) {
      int nXr = (row + x_off) % u->rows;
      int nX = nXr < 0 ? nXr + u->rows : nXr;
      int nYr = (column + y_off) % u->cols;
      int nY = nYr < 0 ? nYr + u->rows : nYr;
      if (!(x_off == 0 && y_off == 0)) {
        neighbors += is_alive(u, nY, nX);
      }
    }
  }
  if (neighbors == 3) {
    return 1;
  } else if (neighbors == 2 && u->arr[row][column] == 1) {
    return 1;
  } else {
    return 0;
  }
}

void evolve(struct universe *u,
            int (*rule)(struct universe *u, int column, int row)) {
  int **new = malloc(u->rows * sizeof(*u->arr));
  for (int i = 0; i < u->rows; i++) {
    new[i] = malloc(u->cols * sizeof(new[0]));
  }
  for (int x = 0; x != u->rows; x++) {
    for (int y = 0; y != u->cols; y++) {
      new[x][y] = rule(u, y, x);
    }
  }
  for (int x = 0; x != u->rows; x++) {
    for (int y = 0; y != u->cols; y++) {
      u->arr[x][y] = new[x][y];
    }
  }
  free(new);
}

void print_statistics(struct universe *u) {
  int alive_round = 0;
  int total_round = 0;
  for (int x = 0; x != u->rows; x++) {
    for (int y = 0; y != u->cols; y++) {
      alive_round += u->arr[x][y];
      total_round++;
    }
  }
  u->alive_cumulative += alive_round;
  u->total_cumulative += total_round;

  float percent_round = alive_round / (float)(total_round);
  float percent_cumulative = u->alive_cumulative / (float)(u->total_cumulative);

  printf("%.3f%% of cells currently alive\n", percent_round);
  printf("%.3f%% of cells alive on average\n", percent_cumulative);
}
