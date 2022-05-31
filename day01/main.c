#include <stdio.h>

int main() {
  // read input.txt numbers
  FILE *f = fopen("input.txt", "r");

  long fuel_sum_part1 = 0;
  long fuel_sum_part2 = 0;
  while (1) {
    long mass = 0;
    fscanf(f, "%ld", &mass);
    if (feof(f)) {
      break;
    }
    fuel_sum_part1 += (mass / 3) - 2;
    while (1) {
      long fuel = (mass / 3) - 2;
      if (fuel <= 0) {
        break;
      }
      fuel_sum_part2 += fuel;
      mass = fuel;
    }
  }

  printf("Part 1: %ld\n", fuel_sum_part1);
  printf("Part 2: %ld\n", fuel_sum_part2);

  fclose(f);
  return 0;
}
