#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_opcodes(int *opcodes, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", opcodes[i]);
    }
    printf("\n");
}

// Runs computer with opcodes in mem_orig of length size with inputs noun and verb.
// Does not modify mem_orig.
int run_computer(int* mem_orig, int size, int noun, int verb) {
  int* mem = malloc(size * sizeof(int));
  memcpy(mem, mem_orig, size * sizeof(int));

  mem[1] = noun;
  mem[2] = verb;

  int pc = 0;
  while (1) {
    switch (mem[pc]) {
      case 1:
        mem[mem[pc+3]] = mem[mem[pc+1]] + mem[mem[pc+2]];
        pc += 4;
        break;
      case 2:
        mem[mem[pc+3]] = mem[mem[pc+1]] * mem[mem[pc+2]];
        pc += 4;
        break;
      case 99:
        return mem[0];
      default:
        printf("ERROR: invalid opcode %d at %d, inputs %d %d\n", mem[pc], pc, noun, verb);
        return -1;
    }
  }
}

int main() {
  FILE *f = fopen("input.txt", "r");

  // read in opcodes
  int* mem;
  int capacity = 1;
  mem = malloc(sizeof(int) * capacity);
  int i = 0;
  int size = 0;
  while (fscanf(f, "%d,", &mem[i]) != EOF) {
    i++;
    // dynamically resize array of necessary
    if (i == capacity) {
      capacity *= 2;
      mem = realloc(mem, sizeof(int) * capacity);
    }
  }
  size = i;
  fclose(f);

  printf("Part 1: %d\n", run_computer(mem, size, 12, 2));

  // Part 2
  // Brute force search for noun and verb that produce desired output
  int desired_output = 19690720;
  for (int noun = 0; noun < 100; noun++) {
    for (int verb = 0; verb < 100; verb++) {
      int output = run_computer(mem, size, noun, verb);
      if (output == desired_output) {
        printf("Part 2: %d\n", 100 * noun + verb);
        return 0;
      }
    }
  }
}
