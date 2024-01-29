// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 1
// Exercises 2
// Submission code: 67882241

// Include section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Macro section
#define MAX_STR 128         // Max length of a single input sequence.
                            // Note: this depends on the specifications
                            // of the program; for now, 128 is chosen.
#define ALPHA_CHAR_COUNT 26 // No. of chars in English alphabet

// Function prototype declaration(s)
char shift_alpha_char(char c, int s);

// Main program section
int main(int argc, char* argv[]) {
  // Make sure that an argument is passed. Otherwise, exit with a 1 to
  // indication an error.
  if (argc != 2) {
    printf("Error: provide a number to shift the characters by.\n");
    return 1;
  }

  // Extract the shift value from the command-line args.
  int shift = atoi(argv[1]);

  // Create a char-based buffer with at most MAX_STR elements. We assume
  // that a user will never write more than MAX_STR - 1 chars in a single line
  // (the last char is reserved for the '\0' char).
  char buffer[MAX_STR];

  // Instantiate an index to store the location of the current char
  // within the buffer.
  int i = 0;
 
  // Read an initial char before entering the loop and assign it
  // to `c` which will be used later in the loop. Because we are
  // supposed to read char by char, we use the `getchar` function.
  char c = getchar();

  // Simulate a repl. environment; enter a loop.
  do {
    // Detect a newline character. This means that the typed sequence is to be
    // encrypted. Therefore, the buffer is printed and reset afterwards.
    if (c == '\n') {
      // Add trailing '\0' to delimit a char sequence
      buffer[i + 1] = '\0'; 

      // Print the encrypted word
      printf("%s\n", buffer);

      // Zero out the whole char buffer. Because `buffer` has a fixed
      // size, we may use `sizeof` here.
      memset(buffer, 0, sizeof(buffer));
      i = 0; // Reset the index within the buffer
    } else {
      // Otherwise, shift the char by `s` places. `sc` means a 'shifted
      // character'.
      char sc = shift_alpha_char(c, shift);

      // Include the char in the buffer at `i`-th position. 
      buffer[i] = sc;
      i++; // Increment the current index of the buffer
    }

    // Proceed to read a character from the user. This takes
    // place at the end of the `do` block, because our condition
    // relies on the value of `c`. In other words, we need to
    // read a char before we can check whether it is EOF. This check
    // happens at the beginning of the `do` block, therefore, we need
    // to have read a char before we can check for EOF.
    c = getchar();
  } while (c != EOF);
}

/**
 * A function to shift an alphabetical (i.e., alpha) character
 * by `s` places. This function is case-sensitive and ensures
 * that merely alpha characters can be returned (i.e., Z + 1
 * becomes A). In case a non-alpha character is passed, the
 * character is unchanged. Here, we rely on the ctype.h library
 * to determine whether a char is upper or lower case, in a more
 * elegant manner (otherwise, we could simply check the ASCII codes,
 * however, that introduces magic numbers, and this approach is,
 * generally speaking, better in terms of the overall readability.
 *
 * @param c - the original character
 * @param s - the number of places to shift the character by
 */
char shift_alpha_char(char c, int s) {
  char result = c;   // Initially, assign result the original value of c
  if (isupper(c)) {  // Detect an upper-case char
    result = c + s;  // Apply the shift

    // If the max. value of an upper-case char is exceeded, shift the value by
    // 26 places (i.e., the number of letters in an English alphabet.
    if (result > 'Z') result -= ALPHA_CHAR_COUNT;
  } else if (islower(c)) {  // Detect a lower-case char
    result = c + s;         // Apply the shift

    // If the max. value of a lower-case char is exceeded, shift the value by
    // 26 places (i.e., the number of letters in an English alphabet.
    if (result > 'z') result -= ALPHA_CHAR_COUNT;
  }

   // Not an alpha char is detected, so return the original char.
  return result;
}

