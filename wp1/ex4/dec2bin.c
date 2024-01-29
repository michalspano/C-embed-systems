// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 1
// Exercises 4
// Submission code: 67882241

// Include section
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

// Macro section
#define BIT1 0b1;

// Function prototype declaration(s)
bool is_numeric(const char* buff);

// Main program section
int main(int argc, char* argv[]) {
  // Ensure that the expected number of command-line arguments is passed.
  // Return 2 to indicate an error.
  if (argc != 2) {
    printf("Error: missing argument.\n");
    return 2;
  // Detect the help flag. Prompt the user with the usage of the program.
  // Return 0 to indicate success.
  } else if (strcmp(argv[1], "-h") == 0) {
    printf("Usage: %s <64bit_positive_number>\n", argv[0]);
    printf("This program converts a decimal 64-bit integer to its binary representation.\n");
    return 0;
  // Ensure that the decimal number is valid (i.e., it contains merely digits).
  // Return 2 to indicate an error.
  } else if (!is_numeric(argv[1])) {
    printf("Error: not a valid number.\n");
    return 2;
  }

  // Declare `bit_format`. It essentially represents the datatype, that is,
  // how many bit positions are required to store the decimal number. Because
  // `long` is assumed to be the upper bound, the maximum number of bits is 64.
  // Here, we start with 8 bits as the initial value, and we will increase it
  // if the decimal number is greater than the maximum value of the current
  // bit format.
  int bit_format = 8;

  // Convert the decimal number to its unsigned long representation
  unsigned long n = atol(argv[1]);

  if (n > UINT8_MAX)  bit_format = 16; // Exceeds the maximum value of 8 bits
  if (n > UINT16_MAX) bit_format = 32; // Exceeds the maximum value of 16 bits
  if (n > UINT32_MAX) bit_format = 64; // Exceeds the maximum value of 32 bits
                                       // Assumption: we expect at most a long int

  // Iterate over each bit position from the bit format. Start from the most
  // significant bit position, and end at the least significant bit position.
  for (int i = bit_format - 1; i >= 0; i--) {

    // Shift the bit position to the right by `i` places, receiving a subsequence
    // of bits. Then, extract the last bit from the subsequence.
    unsigned long n_sub = n >> i;

    // Extract the trailing bit from the subsequence, and print it. This is done
    // using the `&` operator, which is a bitwise AND operator. The `&` operator
    // returns 1 if both operands are 1, and 0 otherwise. In this case, the
    // right operand is 1, and the left operand is the subsequence. Therefore,
    // the `&` operator returns 1 if the last bit of the subsequence is 1, and
    // 0 otherwise.
    int last_bit = n_sub & BIT1;
    printf("%d", last_bit);
  }
  printf("\n"); // Add the missing new-line
 }

/**
 * A helper function to ensure that each char from a char sequence is numeric
 * (i.e., 0 to 9). 
 *
 * @param buff - a char sequence
 * @returns - a boolean value indicating whether the char sequence is numeric
 */
bool is_numeric(const char* buff) {
  // Iterate over each char from the buffer
  for (int i = 0; i < strlen(buff); i++) {

    // Return false if the current char is not a digit
    if (!isdigit(buff[i])) return false;
  }

  // Otherwise, return true to indicate tha the sequence is numeric
  return true;
}

