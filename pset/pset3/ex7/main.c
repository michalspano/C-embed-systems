// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 3
// Exercise 7
// Submission code: 0173549

// Includes section
#include <stdio.h>
#include <string.h>

// Macros section
#define STR_SIZE 20

// Function prototypes
void clearBuffer();                     // Consume remaining chars.
void copyString(char* buff, char* str); // Custom alias of `strcpy`.

/**
 * Main program section. The program does not use any command-line arguments
 * provided when the executable is run.
 */
int main(void) {
  // The string is to be of size 20 chars. Instantiate 3 buffers: (i) for the
  // input string, (ii) for the string copied with `strcpy`, (iii) for the
  // string copied with the custom function. Add +1 to include the null
  // terminator.
  char in_str[STR_SIZE + 1],
           s1[STR_SIZE + 1], 
           s2[STR_SIZE + 1];

  // Prompt the user to provide the string.
  printf("Provide a string: ");

  // Use a width specifier of 20 chars.
  if (scanf("%20s", in_str)) {
    // If reading was successful, add the null terminator at the end and clear
    // any remaining chars in the buffer.
    in_str[STR_SIZE] = '\0'; clearBuffer();
  } else {
    // Otherwise, notify the user about the error and exit the program.
    printf("Error while reading the input.\n");
    return 1;
  }

  // Use the `strcpy` function to copy the contents of the string to `s1`.
  strcpy(s1, in_str);

  // Use the custom function to copy the contents of the string to `s2`.
  copyString(s2, in_str);

  // 'Manual' check that verifies that the copied strings are identical. That is,
  // assuming some string s, strcpy(s) == copyString(s). Change #if 0 to #if 1 for
  // the pre-processor to recognize the code segment.
#if 0
  // Verify that the strings are equal. Use the `strcmp` function. Assume that
  // `strcpy` always yields, in this case, a correct result.
  _Bool are_identical = strcmp(s1, s2) == 0;

  // Print the result.
  are_identical ? printf("The strings are identical.\n")
                : printf("The strings are NOT identical.\n");
#endif

  // Print the string to the console.
  printf("The processed string: %s\n", s2);

  // Exit with 0 to indicate a successful completion.
  return 0;
}

/**
 * Custom function to copy the contents of a string to another char buffer.
 * @param buff - the buffer where the chars are to be placed.
 * @param str  - the string to be copied.
 *
 * This function does not return a value, albeit causes a side effect and the
 * state of `buff` is updated.
 */
void copyString(char* buff, char* str) {
  char c; // Initialize c

  // Attempt to read chars of str until the null-terminator is encountered.
  // Each char is placed at a location of the buffer where the current pointer
  // of buff points to. The pointers are consecutively incremented to denote.
  while ((c = *str) != '\0') *buff++ = *str++;

  // Ultimately, add the null terminator as the last element. We have to do
  // this manually, because, previously, the null-terminator was used as an
  // identifier to limit a char sequence.
  *buff = '\0';
}

/**
 * A helper subroutine to consume remaining chars in the buffer.
 */
void clearBuffer() {
  // Get all chars until new-line or EOF are encountered.
  char c; while ((c = getchar()) != '\n' && c != EOF);
}
