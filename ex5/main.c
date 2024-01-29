// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 1
// Exercises 5
// Submission code: 67882241

// Include section
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// ------ Function declarations ----------

// This function generates a set of random numbers and fills the table *tab
// with these numbers.
void create_random(int *tab);

// This function takes the *tab of random numbers and creates a table with the
// frequency counts for these numbers.
void count_frequency(int *tab, int *freq);

// This function takes the frequency count table and draws a histogram of the
// values in that frequency table.
void draw_histogram(int *freq);

// Declare global variables used throughout the program. These determine the
// size of the table and the max. value within the table respectively.
int MAX, MAXNUMBER;

// ------ Main --------------------------

// The main entry point for the program. This program uses subroutines with
// side effects, so they are called merely to emulate an action.
int main (int argc, char* argv[]) {
  srand(time(NULL)); // Reset the seed to ensure that the random numbers are
                     // different each time the program is run.

  // The program may accept two optional values, namely the size of the table
  // and the max. number within the table. Therefore, if `argc` is 3, then we
  // can assign the two command-line arguments to the variables. However, we
  // do not check if these values are valid, that is, if they are numbers, for
  // the sake of simplicity.
  if (argc == 3) {
    // Assign the variables with the values from the command-line.
    MAX = atoi(argv[1]); MAXNUMBER = atoi(argv[2]);
 
  // Detect the help flag to prompt the user with the expected usage of the
  // program. Afterwards, return 0 to terminate the program. However, the help
  // flag must only be detected when argc is 2, otherwise we may access memory
  // that we should not.
  } else if (argc == 2 && strcmp(argv[1], "-h") == 0) {
    printf("Usage: %s [TABLE_SIZE] [MAX_NUMBER]\n", argv[0]);
    printf("  TABLE_SIZE: the size of the random number sequence\n");
    printf("  MAX_NUMBER: the range of the random numbers\n");
    return 0;
  // Otherwise, assign the default values to the variables. These values
  // originate from the original template found in the assignment.
  } else {
    MAX = 100; MAXNUMBER = 20;
  }

  // Define the tables to be used in the program
  int table[MAX], frequency[MAXNUMBER];

  // Zero out the frequency table; this is feasible because we know the size of
  // the `frequency` table.
  memset(frequency, 0, sizeof frequency);

  // Create a set of random numbers
  create_random(table);

  // Count the frequency of the numbers
  count_frequency(table, frequency);

  // Finally, draw the histogram
  draw_histogram(frequency);
}

// ------ Function definitions ----------

/**
 * A helper function to create a set of random numbers
 * and fill the table *tab with these numbers. Each number
 * is at most MAXNUMBER.
 * @param tab - a pointer to a table of an int sequence
 */
void create_random(int *tab) {
  // Repeat MAX times; that's assumed to be the size of the table
  for (int i = 0; i < MAX; i++) {
    tab[i] = rand() % MAXNUMBER; // Compute a new random number, assign it to
                                 // the table at the i-th index.
  }
}

/**
 * A helper function to compute the frequency of numbers within
 * a table (i.e., an int sequence). The function does not return
 * anything, it merely causes a side effect of filling the frequency
 * table with the counts of the numbers in the sequence.
 *
 * @param tab - a pointer to a table of an int sequence
 * @param freq - a pointer to a frequency table
 */
void count_frequency(int *tab, int *freq) {
  // Repeat the loop MAX times; that's assumed to be the size of the table
  for (int i = 0; i < MAX; i++) {
    int val = tab[i]; // Extract the value from the current position from the
                      // table.
  
    // If the value is not zero at the frequency table, increment it.
    // Otherwise, set the value to 1 (as the initial count).
    if (freq[val] != 0) freq[val]++;
    else                freq[val] = 1;
  }
}

/**
 * A helper function to compute a histogram based on the frequency table
 * in the desired format. The function does not return anything, it merely
 * produces a side effect, that is, it prints the histogram to the console.
 *
 * @param freq - a pointer to a frequency table
 */
void draw_histogram(int *freq) {
  // Iterate MAXNUMBER times (i.e., the range of `freq`)
  for (int i = 0; i < MAXNUMBER; i++) {
    int f_val = freq[i];      // Extract the value from the i-th position from
                              // the frequency table.
    if (f_val == 0) continue; // If the value is 0, then nothing should be printed,
                              // skip the iteration.
  
    // Print the number whose frequency is displayed.
    printf("%d\t", i);

    // Print 'x' as many times as the specific number is found in the frequency
    // table. The print function is used without a newline character.
    for (int j = 0; j < f_val; j++) printf("x");
   
    printf("\n"); // Add a newline when printing each row.
  }
}

