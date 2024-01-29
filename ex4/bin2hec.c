// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 1
// Exercises 4
// Submission code: 67882241

// Include section
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Macro section
#define HEX_DIGIT 4      // 4 bits (0 to F)
#define LONG_BIT_SIZE 64 // A `long` integer has 64 bits

// Function prototype declaration(s)
bool is_bin_num(const char* buff);
char map_hexdigit(const char bin[HEX_DIGIT]);

// Main entry of the program
int main(int argc, char* argv[]) {
  char* bin; // A declaration of a pointer to a char sequence (i.e., the binary
             // sequence)
  
  // Attempt to read from a piped program, in that case, no additional command-line
  // argument is present.
  if (argc < 2) {
    // Declare a buffer of 64 bits, we assume at most an unsigned `long` int.
    char bit_buffer[LONG_BIT_SIZE];
    
    scanf("%64s", bit_buffer); // Populate the buffer with the value from the input
    bin = bit_buffer;          // Assign the obtained value to `bin`.

  // Ensure that incorrect number of arguments is not present. In that case return
  // 2 to indicate an error.
  } else if (argc > 2) {
    printf("Error: incorrect number of arguments.\n");
    return 2;

  // Otherwise, we assume that there's no piped value from another program, and the
  // command-line argument is to be utilized.
  } else {
    // Check whether the help flag is passed. In that case, show the desired usage of
    // the program and exit with a 0 (this is not an error).
    if (strcmp(argv[1], "-h") == 0) {
      printf("Usage: %s <64bit_binary_number>\n", argv[0]);
      printf("This program converts a binary 64-bit integer to its hexadecimal representation.\n");
      return 0;
    }
    // Extract the binary value from the command-line argument to `bin`.
    bin = argv[1];
  }

  // Ensure that the binary number is valid (i.e., it contains merely 0s and 1s).
  // Return 2 to indicate an error.
  if (!is_bin_num(bin)) {
    printf("Error: not a valid binary number.\n");
    return 2;
  }
  
  // Declare a buffer of char bits where each maps to a single hexadecimal digit
  char bin_buffer[HEX_DIGIT];
 
  // Declare an index to point to the relative position within a 4 bit subsequence
  int idx; 

  // Step over each 4 bit subsequence from the binary sequence
  for (int i = 0; i < strlen(bin); i += 4) {
    idx = 0; // Reset the index to 0

    // Extract the 4 bit subsequence
    for (int j = i; j < i + HEX_DIGIT; j++) {
      // Store the current bit in the buffer, and increment the index
      bin_buffer[idx] = bin[j]; idx++;
    }

    // Print the hexadecimal digit mapped from the 4 bit subsequence
    printf("%c", map_hexdigit(bin_buffer));
  }
}

/**
 * A helper function that transform a four bit sequence to a
 * single hexadecimal digit (i.e., 0 to E). This function uses
 * a more abstracted algorithm, so that it does not exhaustively
 * check all possible combinations of 4 bits (i.e., 16). This
 * may be refactored to a greater extent, but for now, it is
 * sufficient.
 *
 * @params buff - a buffer of size 4 bits that is to be mapped
 * to a single hexadecimal digit.
 * @returns - a char value of 0 through 9, or A through F.
 */
char map_hexdigit(const char buff[HEX_DIGIT]) {
  int sum = 0; // Initialize a sum to 0
  
  // Obtain each bit from the sequence. This way, we can
  // make a simple mapping, so that a value < 9 can be returned
  // as itself, otherwise we map 10 - 15 to A - F.
  for (int i = 0; i < HEX_DIGIT; i++) {
    char b = buff[i];                  // Extract the current bit
    if (b == '1') {                    // Check if its 1
      sum += 1 << (HEX_DIGIT - 1 - i); // Add the current power of 2
    }
  }

  // A digit can be directly returned. Because 0 does not
  // have an ACII value 0, we use the char '0' and add the
  // actual value, so we obtain characters 0 through 9.
  if (sum <= 9) return '0' + sum;

  // Otherwise, we aim to return 'A' through 'F'. 'A' has
  // an ASCII value 55. We know that `sum` is at least 10,
  // so 65 - 10 = 55 which is the lower bound. Suppose that
  // `sum` is 15 (which is the maximum), we obtain 55 + 15
  // = 70, which corresponds to F as expected.
  return 55 + sum;
}

/**
 * A helper function to determine whether some char sequence
 * contains merely 0s and 1s.
 *
 * @param buff - arbitrary char sequence
 * @returns - a boolean value
 */
bool is_bin_num(const char* buff) {
  // Iterate over each char from the buffer
  for (int i = 0; i < strlen(buff); i++) {
    // Return false when a char is not 0 or 1
    if (buff[i] != '0' && buff[i] != '1') return false;
  }

  // Otherwise, return true
  return true;
}

