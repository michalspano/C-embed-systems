// Includes section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros section
#define MAX_STR_LEN 10 // The maximum length of a string to permute

// Function prototypes
long factorial(int n);       // Compute n!
void swap(char *a, char *b); // Swap char a, char b
void permute(char *str,      // String to permute
             int left,       // Start index of str
             int right,      // End index of str
             int *p_idx,     // Index of `perms`
             char **perms    // Array of permutations
);

/* The main entry of the program */
int main(int argc, char* argv[]) {
  // Ensure that the program's usage is correct. We expect that the user
  // provides a single string that is to be permuted. If that is not satisfied,
  // the expected usage of the program is shown.
  if (argc != 2) {
    printf("Error: invalid usage.\n");
    printf("Usage: %s <string>\n", argv[0]);
    return 0; // Return 1 (in a real-world case)
  }

  char* str = argv[1];     // Extract the string from the args
  int slen  = strlen(str); // Length of the given string

  // Ensure that the length of the string is not 0, neither the maximum value
  // is exceeded.
  if (slen == 0 || slen > MAX_STR_LEN) {
    printf("Error: invalid string length.\n");
    return 0;
  }

  // For any natural n, the number of all possible non-unique permutations is
  // equal to n!. For that, we call the `factorial` helper function. This value
  // is used to allocate enough resources when storing the permutations in an
  // array of strings (i.e., char**).
  long perms_count = factorial(slen);

  // The buffer to store all the computations. This is a pointer-pointer sequence,
  // such that every item is a pointer itself. This enables us to store several
  // sequence of characters (i.e., strings).
  char** permutations = (char** )malloc(perms_count * sizeof(char*));

  // Instantiate each string buffer in the permutations buffer
  for (int i = 0; i < perms_count; i++) {
    // Call the `malloc` function, add +1 for the null terminator
    permutations[i] = (char* )malloc((slen + 1) * sizeof(char));
  }

  // Generate permutations. Initialize the index to 0. Call the `permute`
  // function following it's API (in this case, the signature).
  int perm_idx = 0;
  permute(str, 0, slen - 1, &perm_idx, permutations);

  // Print the values from the string array.
  for (int i = 0; i < perms_count; i++) printf("%s ", permutations[i]);
  printf("\n"); // Add the trailing new line character

  // Free the memory (of both the individual strings, as well as the array buffer)
  for (int i = 0; i < perms_count; i++) free(permutations[i]);
  free(permutations);

  // Everything went well, exit out of the program with 0 to indicate a success.
  return 0;
}

/**
 * A recursive function to compute all permutations of a given string.
 * @param str   - the string to permute
 * @param left  - left index of the string
 * @param right - right index of the string
 * @param perms - array to store permutations
 * @param p_idx - pointer to the current index in permutations array
 */
void permute(char *str, int left, int right, int *p_idx, char** perms) {
  // Base case: record a new permutation in the buffer. Use `strcpy` to copy the contents
  // of the permutation to the buffer given the index. The index is then increased.
  if (left == right) { strcpy(perms[(*p_idx)++], str); return; }

  // Recursive case: iterate over the substring, swap the individual elements,
  // and call the permute function recursively, move one position to the right
  // (i.e., increase `left` by 1). This algorithm is based on the concept of 
  // **backtracing** which enables us to generate all possible permutations.
  for (int i = left; i <= right; i++) {
    swap((str + left), (str + i));
    permute(str, left + 1, right, p_idx, perms);
    swap((str + left), (str + i));
  }
}

/**
 * A helper function to swap two characters.
 * @param x - pointer to the first character
 * @param y - pointer to the second character
 */
void swap(char *a, char *b) {
  char tmp
     = *a;  // Temporarily store the value recorded at a.
  *a = *b;  // Assign a's value to b.
  *b = tmp; // Assign `tmp` to b's value.
}

/**
 * A recursive function to compute the factorial of any natural n.
 * @param n - natural number
 * @returns - result of n!
 */
long factorial(int n) {
  if (n == 2) return 2;        // Base case
  return n * factorial(n - 1); // Recursive case
}

