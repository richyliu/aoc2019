#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct direction {
    int x;
    int y;
};

struct direction* read_wire(int* size, FILE* fp) {
  char direction;
  int distance;
  int capacity = 10;
  struct direction* wire = malloc(sizeof(struct direction) * capacity);
  int i = 0;
  while (fscanf(fp, "%c%d,", &direction, &distance) == 2) {
    if (i >= capacity) {
      capacity *= 2;
      wire = realloc(wire, capacity * sizeof(struct direction));
    }
    switch (direction) {
    case 'U':
      wire[i].x = 0;
      wire[i].y = distance;
      break;
    case 'D':
      wire[i].x = 0;
      wire[i].y = -distance;
      break;
    case 'L':
      wire[i].x = -distance;
      wire[i].y = 0;
      break;
    case 'R':
      wire[i].x = distance;
      wire[i].y = 0;
      break;
    default:
      printf("Invalid direction: %c\n", direction);
      exit(1);
    }
    i++;
  }
  *size = i;
  return wire;
}

void get_min_max_dim(struct direction* wire, int len, int* max_dim, int* bias) {
  int x = 0;
  int y = 0;
  int max = *max_dim;
  int min = *bias;
  for (int i = 0; i < len; i++) {
    x += wire[i].x;
    y += wire[i].y;
    if (x > max) {
      max = x;
    }
    if (y > max) {
      max = y;
    }
    if (x < min) {
      min = x;
    }
    if (y < min) {
      min = y;
    }
  }
  *max_dim = max;
  *bias = min;
}

void print_grid(char* grid, int max_dim) {
  for (int i = 0; i <= max_dim; i++) {
    for (int j = 0; j <= max_dim; j++) {
      printf("%c ", grid[i * (max_dim + 1) + j] + ' ');
    }
    printf("\n");
  }
}

void part1(struct direction* wire1, int size1, struct direction* wire2, int size2, int max_dim, int bias) {
  // allocate grid
  char* grid = calloc((max_dim + 1) * (max_dim + 1), sizeof(char));
  grid[bias * (max_dim + 1) + bias] = 10;

  // trace wire 1
  int x = bias;
  int y = bias;
  for (int i = 0; i < size1; i++) {
    int xdir = wire1[i].x > 0 ? 1 : -1;
    for (int j = 0; abs(j) < abs(wire1[i].x); j += xdir) {
      x += xdir;
      assert(y * (max_dim + 1) + x >= 0);
      grid[y * (max_dim + 1) + x] = 1;
    }
    int ydir = wire1[i].y > 0 ? 1 : -1;
    for (int j = 0; abs(j) < abs(wire1[i].y); j += ydir) {
      y += ydir;
      assert(y * (max_dim + 1) + x >= 0);
      grid[y * (max_dim + 1) + x] = 1;
    }
  }

  /* // (DEBUG) trace wire 2 */
  /* x = bias; */
  /* y = bias; */
  /* for (int i = 0; i < size1; i++) { */
  /*   int xdir = wire2[i].x > 0 ? 1 : -1; */
  /*   for (int j = 0; abs(j) < abs(wire2[i].x); j += xdir) { */
  /*     x += xdir; */
  /*     assert(y * (max_dim + 1) + x >= 0); */
  /*     grid[y * (max_dim + 1) + x] |= 2; */
  /*   } */
  /*   int ydir = wire2[i].y > 0 ? 1 : -1; */
  /*   for (int j = 0; abs(j) < abs(wire2[i].y); j += ydir) { */
  /*     y += ydir; */
  /*     assert(y * (max_dim + 1) + x >= 0); */
  /*     grid[y * (max_dim + 1) + x] |= 2; */
  /*   } */
  /* } */
  /* print_grid(grid, max_dim); */

  // trace wire 2, tracking closest intersection
  int closest_distance = max_dim * max_dim;
  x = bias;
  y = bias;
  for (int i = 0; i < size2; i++) {
    int xdir = wire2[i].x > 0 ? 1 : -1;
    for (int j = 0; abs(j) < abs(wire2[i].x); j += xdir) {
      x += xdir;
      assert(y * (max_dim + 1) + x >= 0);
      if ((grid[y * (max_dim + 1) + x] & 1) == 1) {
        int distance = abs(x - bias) + abs(y - bias);
        if (distance < closest_distance) {
          closest_distance = distance;
        }
      }
    }
    int ydir = wire2[i].y > 0 ? 1 : -1;
    for (int j = 0; abs(j) < abs(wire2[i].y); j += ydir) {
      y += ydir;
      assert(y * (max_dim + 1) + x >= 0);
      if ((grid[y * (max_dim + 1) + x] & 1) == 1) {
        int distance = abs(x - bias) + abs(y - bias);
        if (distance < closest_distance) {
          closest_distance = distance;
        }
      }
    }
  }

  printf("Part 1: %d\n", closest_distance);
}

void part2(struct direction* wire1, int size1, struct direction* wire2, int size2, int max_dim, int bias) {
  // track steps needed to reach each segment of the wire
  int* grid1 = calloc((max_dim + 1) * (max_dim + 1), sizeof(int));
  int* grid2 = calloc((max_dim + 1) * (max_dim + 1), sizeof(int));

  int x, y, steps;
  // trace wire 1, tracking steps
  x = bias;
  y = bias;
  steps = 0;
  for (int i = 0; i < size1; i++) {
    int xdir = wire1[i].x > 0 ? 1 : -1;
    for (int j = 0; abs(j) < abs(wire1[i].x); j += xdir) {
      x += xdir;
      assert(y * (max_dim + 1) + x >= 0);
      steps++;
      grid1[y * (max_dim + 1) + x] = steps;
    }
    int ydir = wire1[i].y > 0 ? 1 : -1;
    for (int j = 0; abs(j) < abs(wire1[i].y); j += ydir) {
      y += ydir;
      assert(y * (max_dim + 1) + x >= 0);
      steps++;
      grid1[y * (max_dim + 1) + x] = steps;
    }
  }

  // trace wire 2, tracking steps
  x = bias;
  y = bias;
  steps = 0;
  for (int i = 0; i < size2; i++) {
    int xdir = wire2[i].x > 0 ? 1 : -1;
    for (int j = 0; abs(j) < abs(wire2[i].x); j += xdir) {
      x += xdir;
      assert(y * (max_dim + 1) + x >= 0);
      steps++;
      grid2[y * (max_dim + 1) + x] = steps;
    }
    int ydir = wire2[i].y > 0 ? 1 : -1;
    for (int j = 0; abs(j) < abs(wire2[i].y); j += ydir) {
      y += ydir;
      assert(y * (max_dim + 1) + x >= 0);
      steps++;
      grid2[y * (max_dim + 1) + x] = steps;
    }
  }

  /* // debug print */
  /* for (int i = 0; i <= max_dim; i++) { */
  /*   for (int j = 0; j <= max_dim; j++) { */
  /*     printf("%3d ", grid1[i * (max_dim + 1) + j] + grid2[i * (max_dim + 1) + j]); */
  /*   } */
  /*   printf("\n"); */
  /* } */

  // find intersections
  int closest_distance = max_dim * max_dim;
  x = bias;
  y = bias;
  for (int i = 0; i < size2; i++) {
    int xdir = wire2[i].x > 0 ? 1 : -1;
    for (int j = 0; abs(j) < abs(wire2[i].x); j += xdir) {
      x += xdir;
      assert(y * (max_dim + 1) + x >= 0);
      if (grid1[y * (max_dim + 1) + x] > 0) {
        int distance = grid1[y * (max_dim + 1) + x] + grid2[y * (max_dim + 1) + x];
        if (distance < closest_distance) {
          closest_distance = distance;
        }
      }
    }
    int ydir = wire2[i].y > 0 ? 1 : -1;
    for (int j = 0; abs(j) < abs(wire2[i].y); j += ydir) {
      y += ydir;
      assert(y * (max_dim + 1) + x >= 0);
      if (grid1[y * (max_dim + 1) + x] > 0) {
        int distance = grid1[y * (max_dim + 1) + x] + grid2[y * (max_dim + 1) + x];
        if (distance < closest_distance) {
          closest_distance = distance;
        }
      }
    }
  }

  printf("Part 2: %d\n", closest_distance);
}

int main() {
  FILE *f = fopen("input.txt", "r");

  int size1, size2;
  struct direction* wire1 = read_wire(&size1, f);
  struct direction* wire2 = read_wire(&size2, f);

  // get max dimensions of grid needed
  int max_dim = 0;
  int min_dim = 0;
  get_min_max_dim(wire1, size1, &max_dim, &min_dim);
  get_min_max_dim(wire2, size2, &max_dim, &min_dim);
  int bias = 0;
  if (min_dim < 0) {
    bias = -min_dim;
    max_dim += bias;
  }
  printf("max_dim: %d, bias: %d\n", max_dim, bias);

  part1(wire1, size1, wire2, size2, max_dim, bias);
  part2(wire1, size1, wire2, size2, max_dim, bias);
}
