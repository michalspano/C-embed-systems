// Includes section
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Macros section
// The maximum allowed size of a matrix: 4x4.
#define MATRIX_DIMENSION_MAX 4

// Function declaration prototype(s)
int *read_ints(int n);                                  // Read n ints from the console.
void clearBuffer(void);                                 // Consume remaining chars in the buffer.
void format_matrix(int* m, int size, int width);        // Format matrix per the instructions.
void zip_add(int size, int *arr1, int *arr2, int *res); // Zip the arrays and add pairs to res.

// A global flag to denote whether the matrices are of the desired size.
// This flag is extracted like so simply based on the fact that a function
// cannot return two values. It would, otherwise, be desirable that
// `read_ints()` would return some sort of signal to indicate this deviation.
_Bool is_valid_size = 1;

// Main entry point of the program. No command-line arguments are expected.
// Program assumption: the user is allowed to input only INT (i.e., 32-bit
// numbers). Note: we exit with 0 so that the tests do not fail.
int main(void) {
  int cols, rows; // The width and the height attributes of the matrices.

  // Declare variables that hold pointers to the individual arrays, namely
  // pointers for the values of the first and second matrices and for the
  // resulting matrix.
  int *m1_vals, *m2_vals, *res;

  // Prompt the user to provide the size. Ask for two integers, that is, the
  // number of columns and the number of rows of the matrices (this is the
  // width and the height of the matrix).
  printf("Input the size: ");
  scanf("%d %d", &cols, &rows); clearBuffer();

  // Verify that the matrix is not of size 1x1, because, technically, that is
  // not a two-dimensional matrix. Detect this case, notify the user, and exit
  // the program. Here, we return with 0, so that the CodeGrade tests do not
  // fail.
  if (cols == 1 && rows == 1) {
    printf("Error: 1x1 is an invalid two-dimensional matrix.\n");
    return 0;
  // Verify that the maximum width is not exceeded. In case it is, prompt the
  // user and exit the program.
  } else if (cols > MATRIX_DIMENSION_MAX) {
    printf("Error: invalid height of the matrix. Maximum permitted height is %d.\n", MATRIX_DIMENSION_MAX);
    return 0;
  // Verify that the maximum height is not exceeded. In case it is, prompt the
  // user and exit the program.
  } else if (rows > MATRIX_DIMENSION_MAX) {
    printf("Error: invalid width of the matrix. Maximum permitted width is %d.\n", MATRIX_DIMENSION_MAX);
    return 0;
  }

  int size = cols * rows;           // Calculate the size of the matrix.
  res = malloc(size * sizeof(int)); // Reserve `size` ints on the heap for
                                    // the resulting matrix.

  // Note: for the sake of simplicity, we treat a matrix, in this program, as a
  // finite one-dimensional array.
  // Suppose a matrix [[1, 2], [3, 4]]. In our understanding, this is [1, 2, 3, 4].
  // This makes the implementation much less cumbersome. Furthermore, there's no
  // specific requirement to use a two-dimensional array. Ultimately, the user is
  // not impacted by this decision.

  // Read ints for matrix 1. Ensure that all passed values are numbers. Verify
  // that exactly `size` ints are read. In case either of this properties is
  // not followed, prompt the user and return an exit code.
  printf("Input elements of matrix 1: ");
  if ((m1_vals = read_ints(size)) == NULL) {
    printf("Error: an invalid number was passed to matrix 1.\n");
    return 0;
  } else if (!is_valid_size) {
    printf("Error: invalid size for matrix 1.\n");
    return 0;
  }

  // Read ints for matrix 2. Ensure that all passed values are numbers. Verify
  // that exactly `size` ints are read. In case either of this properties is
  // not followed, prompt the user and return an exit code.
  printf("Input elements of matrix 2: ");
  if ((m2_vals = read_ints(size)) == NULL) {
    printf("Error: an invalid number was passed to matrix 2.\n");
    return 0;
  } else if (!is_valid_size) {
    printf("Error: invalid size for matrix 2.\n");
    return 0;
  }

  // Call the helper function, receive the sum of the 'matrices'. This is,
  // presumably, where most of the program's 'logic' takes place.
  zip_add(size, m1_vals, m2_vals, res);

  printf("The sum is:");          // No need line added here (it is added in the
                                  // function).
  format_matrix(res, size, rows); // Format the matrix per the instructions.
  
  free(m1_vals); // Free the allocated memory for matrix 1.
  free(m2_vals); // Free the allocated memory for matrix 2.
  free(res);     // Free the allocated memory for the resulting matrix.

  // Everything went well, return with a zero to indicate a success.
  return 0;
}

/**
 * A helper function to zip two arrays of identical sizes with an addition.
 * The result is stored in a pre-defined buffer. This is, in principle, an
 * abstraction to sum two matrices.
 *
 * @param size - the number of elements in each array.
 * @param arr1 - the first array to be zipped.
 * @param arr2 - the second array to be zipped.
 * @param res  - the resulting array.
 */
void zip_add(int size, int *arr1, int *arr2, int *res) {
  // Iterate `size` times, sum every i-th value from either array and assign
  // the result to i-th position of `res`.
  for (int i = 0; i < size; i++) res[i] = arr1[i] + arr2[i];
}

/**
 * A function to read n INTs from the console. The function returns NULL if is
 * fails to read the ints.
 * 
 * @param n - the number of integers to read.
 * @returns - a pointer to an int-based sequence.
 */
int *read_ints(int n) {
  int  *ints = malloc(n * sizeof(n)); // Create a buffer of at most n ints.
  int  *res  = ints;                  // Store the head of the sequence (this value is returned).

  // We create a buffer to store chars that can then be cast to a number. `calloc`
  // is called, so the values in the allocated memory are set to 0. Furthermore,
  // the maximum signed INT value is 2^32 - 1 = 4 294 967 295. This is 10 places.
  // Hence, the buffer is of size 11 (+1 for the null terminator). This value
  // is stored in a local constant for readability.
  const int INT_BUFF_SIZE = 11;
  char *raw_int  = calloc(INT_BUFF_SIZE, sizeof(char)); 

  // Relative index of `raw_int`, a counter to determine how many valid ints are read.
  int idx = 0, count = 0;

  // a single character read at once
  char c;
  
  // Consume the buffer until EOF or new-line are found.
  while ((c = getchar()) != '\n' && c != EOF) {
    // The values are to be separated by spaces. A space delimits an int
    // (previously before the space).
    if (isspace(c)) {
      raw_int[idx] = '\0'; // Terminate the string.

      // Reset the index, increase the int counter.
      idx = 0; count++;
      
      // Parse the raw string to int, add it to the buffer, increase the
      // pointer of `ints`.
      *ints++ = atoi(raw_int);

      // Zero out the whole buffer sequence; this way, it is ready to hold
      // a new int and parse it correctly.
      memset(raw_int, 0, INT_BUFF_SIZE);
    }
    // Detect any non-digit symbols. The ints can be negative, so - is ignored.
    // Return NULL.
    else if (!isdigit(c) && c != '-') return NULL;

    // Otherwise, add the raw int to the buffer.
    raw_int[idx++] = c;
  }

  // Append the last int. This is slightly hacky, though, and may not work
  // fully if there's extra white-spaces passed to the right. However, we assume
  // that is not the case. This could be fixed with a right-trim function.
  *ints = atoi(raw_int); count++;

  // Ensure that the desired number of ints was actually read. If not, update
  // the global flag.
  if (count != n) is_valid_size = 0;

  free(raw_int); // Free the dynamically allocated memory for the int buffer.
  return res;    // Return pointer to the head of the int sequence.
}

/**
 * A subroutine to format a 1-D array in a matrix-like manner.
 * @param m      - the 1-D array to be formatted.
 * @param size   - the size of m.
 * @param height - the number of rows the matrix should have.
 */
void format_matrix(int* m, int size, int height) {
  // Iterate size times
  for (int i = 0; i < size; i++) {
    // Check if a new-line is to be inserted with the modulo operator
    if (i % height == 0) printf("\n");
    // Print the i-th value of m.
    printf("%d ", m[i]);
  }
  // Add a trailing new-line.
  printf("\n");
}

/**
 * A helper subroutine to remove all characters from the buffer until EOF or
 * '\n' is found.
 */
void clearBuffer(void) {
  // Instantiate a char placeholder, read until the specified conditions are met.
  char c; while ((c = getchar()) != EOF && c != '\n');
}
