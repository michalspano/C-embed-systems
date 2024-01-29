// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 1
// Exercise 1 
// Submission code: 67882241

// Include section
#include <stdio.h>

/**
 * Declare some sentences that the user can observe.
 * Note: these sentences have been chosen randomly.
 */
const char* SENTENCES[] = {
  "C is a great programming language.",
  "But Haskell is even better.",
  "JavaScript? Please don't mention that.",
  "public static void main ...",
  "Life's good! =)"
};

/**
 * Main program section. This program assumes that the user provides merely
 * numbers. That is, there's no dedicated mechanism that ensures that the given
 * value is a valid number.
 
 * Note: this exercise can be completed with using an if-elif-else construct or
 * a switch statement. However, I've decided to select an array index approach,
 * which is more efficient and elegant.
 */
int main(void) {
  // Declare a variable that will hold the number that the user provides.
  int num;

  // Proceed to prompt the user in a loop.
  while (1) {
    // Prompt the user and assign the value to `num`
    printf("Number: ");
    scanf("%d", &num);
  
    // Ensure that the value satisfies the interval, if not,
    // exit the loop. Show a message to the screen.
    if (num < 1 || num > 5) {
      printf("Exiting...\n");
      break;
    }
    
    // Print the corresponding sentence, continue to loop. Use the `SENTENCES`
    // array to map each index to a sentence.
    printf("%s\n", SENTENCES[num - 1]);
  }
}

