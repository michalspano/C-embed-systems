// Includes section
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Macros section
#define STR_NUM 5   // Number of strings to read
#define STR_MAX 256 // The largest size possible of a string

// Function prototypes - API
char *findShortestStr(char** str_arr);
char **removeStr(char** str_arr, char* str);
void print_str_arr(char* prefix, char** str_arr, int len);
void print_str_arr2(char* prefix, char** str_arr, int len); // Alternative
                                                            // print function
/* The main entry of the program */
int main(void) {
  // Allocate memory for the string array (i.e., char**)
  char** pStr_arr = (char**)malloc(sizeof(char* ) * STR_NUM);

  // Allocate memory for each individual string
  for (int i = 0; i < STR_NUM; i++) {
    // Assign the pointer to the string array sequence
    pStr_arr[i] = malloc(sizeof(char) * STR_MAX);
  }
  
  // Read 5 strings from the user
  for (int i = 0; i < STR_NUM; i++) {
    printf("String %d: ", i + 1); // Prompt the user
    scanf("%255s", pStr_arr[i]);  // Read the value to the array (at most
                                  // 255chars)
    pStr_arr[i][STR_MAX - 1] = '\0';      // Add the trailing null terminator
  }

  // Print the given array
  print_str_arr("Old array:\n", pStr_arr, STR_NUM);
  // print_str_arr2("Old array: ", pStr_arr, STR_NUM - 1); // Alt. call
  
  // Compute the shortest string from the string array
  char* str_to_remove = findShortestStr(pStr_arr);
  
  // Print the string to remove
  printf("Shortest string removed: %s\n", str_to_remove);
  
  // Update the state of the array; remove the shortest string
  char** pNew_arr = removeStr(pStr_arr, str_to_remove);

  // Print the updated array (without the string to remove)
  print_str_arr("Array after removal:\n", pNew_arr, STR_NUM - 1);
  // print_str_arr2("New array: ", pNew_arr, STR_NUM - 1); // Alt. call

  // Free the memory occupied by both string arrays
  for (int i = 0; i < STR_NUM - 1; i++) {
    free(pStr_arr[i]); free(pNew_arr[i]); // Free the individual strings
  }
  free(pStr_arr[STR_NUM - 1]);    // Free the trailing string in `pStr_arr`
  free(pStr_arr); free(pNew_arr); // Free the placeholders for the string arrays
}

// Find the shortest string from an array of strings
char *findShortestStr(char** str_arr) {
  // Extract the first element, store it in a temporary buffer
  char *tmp = str_arr[0];

  // Iterate over all the strings (from index 1)
  for (int i = 1; i < STR_NUM; i++) {
    // If the current string length is lower than the recorded string length.
    if (strlen(str_arr[i]) < strlen(tmp)) {
      tmp = str_arr[i]; // Update the recorded string with the current string
    }
  }
  // Return char pointer to the shorted string found
  return tmp;
}

// Remove a desired string from an array of strings
char **removeStr(char** str_arr, char* str) {
  // Allocate memory: the previous buffer size - 1 (there's one string less)
  char** pStr_arr = (char**)malloc(sizeof(char* ) * STR_NUM - 1);

  // Allocate enough memory for each individual string
  for (int i = 0; i < STR_NUM - 1; i++) {
    pStr_arr[i] = malloc(sizeof(char) * STR_MAX);
  }
  
  int j = 0;                             // Relative index of the new string buffer
  for (int i = 0; i < STR_NUM; i++) {    // Iterate over all strings
    if (str_arr[i] != str) {             // If the desired string is not found,
                                         // add it to the new buffer.
      strcpy(pStr_arr[j++], str_arr[i]); // Add the string to the buffer
    }
  }
  
  // Return the pointer to the string array
  return pStr_arr;
}

// Print all elements of a string array, separated by a new-line (\n)
void print_str_arr(char* prefix, char** str_arr, int len) {
  // Print the prefix
  printf("%s", prefix);

  // Iterate over all the possible strings
  for (int i = 0; i < len; i++) {
    // Print the string
    printf("%s\n", str_arr[i]);
  }
}

// A more "fancy" way to print a string array: ["ab", "cd", "ef", ... ]
void print_str_arr2(char* prefix, char** str_arr, int len) {
  printf("%s[", prefix); // Print the leading prefix

  // Iterate over all the possible strings
  for (int i = 0; i < len; i++) {
    if (i == 0) printf("\"%s\"", str_arr[i]);   // The first element is not comma separated
    else        printf(", \"%s\"", str_arr[i]); // Add leading comma
  }
  printf("]\n"); // Print the trailing bracket
}

