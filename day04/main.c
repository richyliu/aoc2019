#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int isValidPassword(int num) {
  int prev_digit = 11;
  int has_double = 0;
  while (num > 0) {
    int digit = num % 10;
    if (digit > prev_digit) {
      return 0;
    }
    if (digit == prev_digit) {
      has_double = 1;
    }
    prev_digit = digit;
    num /= 10;
  }

  return has_double;
}

int isValidPasswordPart2(int num) {
  if (!isValidPassword(num)) {
    // a valid part 2 password is also a valid part 1 password
    return 0;
  }
  // check for a double pair
  int digits[10];
  for (int i = 0; i < 10; i++) {
    digits[i] = 0;
  }

  while (num > 0) {
    int digit = num % 10;
    digits[digit]++;
    num /= 10;
  }

  for (int i = 0; i < 10; i++) {
    if (digits[i] == 2) {
      return 1;
    }
  }
  return 0;
}

int main() {
  FILE *fp = fopen("input.txt", "r");

  int min = 0;
  int max = 0;
  fscanf(fp, "%d-%d", &min, &max);

  int count = 0;
  int count_part2 = 0;
  for (int i = min; i <= max; i++) {
    if (isValidPassword(i)) {
      count++;
    }
    if (isValidPasswordPart2(i)) {
      count_part2++;
    }
  }
  printf("Part 1: %d\n", count);
  printf("Part 2: %d\n", count_part2);

  fclose(fp);
}
