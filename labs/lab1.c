// Includes section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros section
#define false 0
#define true 1
#define INT_SIZE 32 // == 4 Bytes

// Function prototype declaration(s)
_Bool is_numeric(const char* buff);

// The entry point of the program
int main(int argc, char** argv) {
  // Handle the case when the incorrect number of arguments is passed.
  if (argc != 2) {
    printf("Error: No number provided. Please provide a number as a command line argument.\n");
    // Enforce that the passed argument is a valid number.
  } else if (!is_numeric(argv[1])) {
    printf("Error: Invalid number.\n");
    // Otherwise, proceed with the program.
  } else {

    // Extract the argument to an int.
    // Use the pre-defined `atoi` function from `stdlib.h`.
    int n = atoi(argv[1]);

    // Binary values: 0 index stores 0s, 1st index stores 1s.
    // The array is initialized with 0 for both indices.
    int bvs[2] = {0, 0};     

    printf("Number: %d\n", n);
    printf("Binary: 0b");

    /* Solution to transform an INT to its binary representation.
     * The complexity of this computation is O(INT_SIZE) and because
     * INT_SIZE is a constant value, we can approximate the complexity
     * to O(1) (i.e., constant).
     *
     * Note: both counting the 0s and 1s and printing the binary
     * representation are done in the same loop, for the sake of
     * efficiency.
     */
    for (int i = INT_SIZE - 1; i >= 0; i--) {
      int n_sub = n >> i;           // Take a subset of n shifted by i places
      if (n_sub != 0) {             // Ignore leading 0s
        int tail_bit = n_sub & 0b1; // Determine the bit from the tail of the subset.
                                    // This means that we extract each bite from left
                                    // to right.
        bvs[tail_bit]++;            // Increase the count of 0s and 1s of `bvs`
        printf("%d", tail_bit);     // Display bit value to the console
      } 
    }
    printf("\n"); // Add a new line
  
    /*               Counting 0s and 1s - Alternative solution
     *
     * It is possible to determine the 0s and 1s using the modulo operator
     * based on the following implementation. This was my initial approach
     * to the problem. The complexity of this solution is O(log_2(n)) which
     * could be more appropriate (in some cases) that the afore-mentioned one.
     *
     * The algorithm is the following:
     * ```
     * for (int m = n; m > 0; m /= 2) {
     *   int r = m % 2;  // Take module 2 of m
     *   bvs[r]++;       // Increment the number of 0s or 1s based on r
     * }
     * ```
     */
       
    // Print the number of 0s and 1s respectively.
    printf("Number of 0s: %d\n", bvs[0]);
    printf("Number of 1s: %d\n", bvs[1]);
  }

  // Successful completion, return 0.
  return 0;
}

/* Check if a string buffer contains only numeric characters.
 * This function is used to determine whether the passed argument
 * is a valid number.*/
_Bool is_numeric(const char* buff) {
  // Iterate over each char of the buffer
  for (int i = 0; i < strlen(buff); i++) {
    int c = buff[i]; // extract the ACII value of the char
    // If c is not any of the ascii nums, return false.
    if (c < '0' || c > '9') return false;
  }

  // Otherwise, all chars are numeric, so return true.
  return true;
}
