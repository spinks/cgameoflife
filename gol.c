#include "gol.h"

void read_in_file(FILE *infile, struct universe *u) {
  u->alive_cumulative = 0;
  u->total_cumulative = 0;

  u->arr = malloc(sizeof(u->arr[0]));
  u->arr[0] = malloc(sizeof(u->arr[0][0]));
  if (u->arr == NULL || u->arr[0] == NULL) {
    fprintf(stderr, "Unable to allocate memory.\n");
    errno = ENOMEM;
    return;
  }

  int first_line_length = 0;
  int first_line_finished = 0;
  int row = 0;
  int max_row = 0;
  int col = 0;
  int max_col = 0;

  char ch;
  while (fscanf(infile, "%c", &ch) != EOF) {
    if (row == max_row) {
      int new_row = (max_row + 1) * 2;
      int **new_arr = realloc(u->arr, new_row * sizeof(*u->arr));

      if (new_arr == NULL) {
        fprintf(stderr, "Unable to allocate memory.\n");
        errno = ENOMEM;
        return;
      }
      for (int i = row; i != new_row; i++) {
        new_arr[i] = malloc(sizeof(u->arr[0][0]));
        if (new_arr[i] == NULL) {
          fprintf(stderr, "Unable to allocate memory.\n");
          errno = ENOMEM;
          return;
        }
      }
      max_row = new_row;
      u->arr = new_arr;
    }
    if (col == max_col) {
      int new_col = (max_col + 1) * 2;
      int *new_line = realloc(u->arr[row], new_col * sizeof(*u->arr[row]));
      if (new_line == NULL) {
        fprintf(stderr, "Unable to allocate memory.\n");
        errno = ENOMEM;
        return;
      }
      max_col = new_col;
      u->arr[row] = new_line;
    }
    if (ch == '.' || ch == '*') {
      if (first_line_finished == 0) first_line_length++;
      if (first_line_length > 512) {
        fprintf(stderr, "Line length is over permited 512 columns.\n");
        errno = EINVAL;
        fclose(infile);
        return;
      }
      u->arr[row][col] = (ch == '.') ? 0 : 1;
      col++;
    } else if (ch == '\n') {
      if (first_line_finished == 0) first_line_finished = 1;
      if (col != first_line_length) {
        fprintf(stderr, "Line lengths not consistent.\n");
        errno = EINVAL;
        fclose(infile);
        return;
      }
      col = 0;
      max_col = 0;
      row++;
    } else {
      fprintf(stderr, "Invalid characters found in input: %c\n", ch);
      errno = EINVAL;
      fclose(infile);
      return;
    }
  }
  u->rows = row;
  u->cols = first_line_length;

  for (int x = 0; x != u->rows; x++) {
    for (int y = 0; y != u->cols; y++) {
      u->alive_cumulative += u->arr[x][y];
      u->total_cumulative++;
    }
  }
  fclose(infile);
}

void write_out_file(FILE *outfile, struct universe *u) {
  for (int x = 0; x != u->rows; x++) {
    for (int y = 0; y != u->cols; y++) {
      char c = u->arr[x][y] ? '*' : '.';
      fputc(c, outfile);
    }
    fputc('\n', outfile);
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
  if ((neighbors == 3) || (neighbors == 2 && u->arr[row][column] == 1)) {
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
      int nX = (row + x_off + u->rows) % u->rows;
      int nY = (column + y_off + u->cols) % u->cols;
      if (!(x_off == 0 && y_off == 0)) {
        neighbors += is_alive(u, nY, nX);
      }
    }
  }
  if ((neighbors == 3) || (neighbors == 2 && u->arr[row][column] == 1)) {
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
  int alive_round = 0;
  int total_round = 0;
  for (int x = 0; x != u->rows; x++) {
    for (int y = 0; y != u->cols; y++) {
      new[x][y] = rule(u, y, x);
      alive_round += new[x][y];
      total_round++;
    }
  }
  u->alive_cumulative += alive_round;
  u->total_cumulative += total_round;
  for (int x = 0; x != u->rows; x++) {
    for (int y = 0; y != u->cols; y++) {
      u->arr[x][y] = new[x][y];
    }
    free(new[x]);
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

  float percent_round = 100 * alive_round / (float)(total_round);
  float percent_cumulative =
      100 * u->alive_cumulative / (float)(u->total_cumulative);

  printf("%.3f%% of cells currently alive\n", percent_round);
  printf("%.3f%% of cells alive on average\n", percent_cumulative);
}
