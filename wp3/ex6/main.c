// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 3
// Exercise 6
// Submission code: 0173549

// Includes section
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Macros section
#define MAX 20     // Number of ints in the buffer array.
#define MAX_VAL 99 // Maximum allowed integer placed in the buffer.

// Function prototypes
int random_int();  // Compute a random int in the desired range.

/**
 * Main program section. The program does not use any command-line arguments
 * provided when the executable is run.
 */
int main(void) {
  srand(time(NULL));   // Reset the seed.
  int array[MAX];      // Declare integer array of size MAX.
  int *pArray = array; // Store the pointer to MAX in a stand-alone variable.
                       // This is used for pointer incrementation.

  // Populate the array, use the `random_int()` function to generate the
  // individual integer values.
  for (int i = 0; i < MAX; i++) array[i] = random_int();

  // 1. The value of the address of the array
  printf("The value of the address of the array (pointer) is: %p\n", array);

  // 2. First integer in the array
  // We can simply extract the value of the first integer in the array by
  // dereferencing the pointer. Alternatively we can use array[0] to access the
  // first element.
  printf("First integer in the array is (array[0]) %d\n", *pArray);

  // 3. The last integer
  // We can extract the value of the last integer in the array by adding the
  // number of elements in the array to the pointer and then dereferencing it.
  // Alternatively, we can use array[MAX - 1] to access the last element.
  printf("The last integer in the array is: %d\n", *(pArray + MAX - 1));

  // 4. Size of an int
  printf("The size of an integer (number of bytes) is: %lu\n", sizeof(int));

  // 5. The size of the whole array
  printf("The size of the whole array in byte is: %lu\n\n", MAX * sizeof(int));

  // 6. Print out each value from the int buffer and multiply it by two.
  for (int i = 0; i < MAX; i++) {
    int val = *pArray++; // Extract the value of the address, increment the
                         // pointer (to point to the next value in the
                         // sequence).
    // Print each line in a formatted manner.
    printf("array[%d] = %d, array[%d] * 2 = %d\n", i, val, i, val * 2);
  }

  // Everything went well, return 0.
  return 0;
}

/**
 * A helper function to generate a random integer in the range [1, 99].
 * @returns - a new integer in the above interval.
 */
int random_int() {
  // Use the `rand()` function with the modulo operator, use +1 to make the
  // boundary values included in the interval.
  return (rand() % MAX_VAL) + 1;
}
