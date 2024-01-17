// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 0
// Exercises 1, 2, 3
// Submission code: XXXXXX (provided by your TA-s)

// Include the required libraries
#include <stdio.h>
#include <string.h>

// Declaration of the `show_help` function so it can be accessed in the
// `main` function.
void show_help(char* basefile);

// The main entry point of the program
int main(int argc, char** argv) {
  /* Ensure that the expected number of arguments is passed. In case this is
   * not satisfied, i.e., the required argument is missing, the user is
   * prompted with a message and the program is exited with 1, hence indicating
   * an error. */
  if (argc != 2) {
    printf("Error: incorrect number of arguments. See '-h' for the expected usage.\n");
    return 1;
  }

  // Extract the argument from the command-line arguments
  char* arg = argv[1];

  /* Check for the -h (help) flag.
   * This is not an issue, hence 0 is returned (the user simply wants
   * to learn about the usage of this program). The `show_help` function
   * is called. */
  if (strcmp(arg, "-h") == 0) {
    show_help(argv[0]);
    return 0;
  }

  // Write the message to the user
  printf("Hello World! - I'm %s!\n", arg);
}

// Show the expected usage of this program in a subroutine.
// The basefile is ultimately the first argument from `argv`.
void show_help(char* basefile) {
  printf("Usage: %s <argument>\n", basefile);
  printf("This program greets the user with a simple message.\n");
  printf("Arguments:\n");
  printf("  <argument>: string to display in the console.\n");
  printf("Flags:\n");
  printf("  -h\tDisplays this help message to the user.\n");
}
