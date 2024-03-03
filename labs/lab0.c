// Include the required libraries.
#include <stdio.h>
#include <string.h>

// Note: this flow is not very good, and return <N> should be used instead.
// However, for this assignment, this was not possible and caused the
// submission to fail, hence the wonky workflow.
int main(int argc, char** argv) {
  // Check whether the command-line is provided.
  if (argc != 2) {
    printf("No command line argument provided, please provide a name. \n");
  } else {
    // Extract the `name` from the arguments.
    char* name = argv[1];

    // Ensure that the name is of the desired length: [3, 50].
    if (strlen(name) > 50) {
      printf("Name too long, max 50 characters\n");
    } else if (strlen(name) < 3) {
      printf("Name too short, min 3 characters\n");
    } else {
      // Otherwise, prompt the user!
      printf("Hello %s, how is your day today?\n", name);
    }
  }

  // All went well, return 0.
  return 0;
}
