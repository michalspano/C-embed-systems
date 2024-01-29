// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 1
// Exercise 3
// Submission code: <67882241>

// Include section
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Macro section
#define MIN_NUMBER 1    // the lower bound of the generated number
#define MAX_NUMBER 100  // the upper bound of the generated number
#define MAX_GUESSES 7   // maximum amount of guesses (worst case binary search ;))
#define MAX_INPUT_LEN 8 // max length of input buffer 


// Function prototype declaration(s)
// Note: hover for function descriptions

bool readInput();
void clearInputStream(); 
void playRound();
int generateRandomInt(int low, int high);

// Global variables
char userInput[MAX_INPUT_LEN];   // global user input buffer (initialised with the specified max length)

// game stats-trackers
int totalGuesses;    // total number of guesses across all rounds
int totalRounds;     // total number of rounds played
int totalWins;       // total number of rounds won


// Main program section
int main(int argc, char** argv) {
   // Variable declarations
   bool keepPlaying;  // flag to continue playing 
   bool nextRound;    // flag to initiate the next round


   // Program logic

   // initialise the global stat-tracking variables
   totalGuesses = 0;
   totalRounds = 0;
   totalWins = 0;

   // set seed for the random number generator to the current time (by passing NULL)
   // this avoids generating the same number sequence every time the program runs
   srand(time(NULL)); 

   // show greeting message
   printf("\n<<Program Start>>\n\n");
   printf("The computer will generate a random number between %d and %d.\n", MIN_NUMBER, MAX_NUMBER);
   printf("See if you can guess it right!\n");

   // initialise gameplay flags to "true" to start the first game
   keepPlaying = true;
   nextRound = true;


   // MAIN GAME-LOOP
   while (keepPlaying) {

      // check if a new round should be initiated
      if (nextRound) {
         // call play new round function
         playRound();
         // set flag to "false" after round ends to prevent another round from prematurely starting
         nextRound = false;
      }

      // ask user if they want to play another round
      printf("\nDo you want to play another round? (Y/N)\n");

      // try to read the answer
      if (readInput()) {
         // Note: we only check the first letter to be lenient with their answer

         // user wants to play another round
         if (userInput[0] == 'y' || userInput[0] == 'Y') {
            // set flag to "true" to trigger a new round on the next loop iteration
            nextRound = true;

         // user does not want to play more
         } else if (userInput[0] == 'n' || userInput[0] == 'N') {
            // set flag to "false" to show post-game message and exit program
            keepPlaying = false;

         // user did not enter a valid answer
         } else {
            printf("Error: invalid input. Please type either 'Y' or 'N'!\n");
            // Note: none of the game flags are set, triggering the "keep playing?"-prompt again
         }
      }

   } /* main game loop */

   // show exit message
   // print game stats from the current session
   // Note: this is merely optional and not demanded by the assignment.
   // Comment the following paragraph if undesired.
   printf("\nThank you for playing! Your stats were:\n");
   printf("  > Rounds played: %d\n", totalRounds);
   printf("  > Rounds Won: %d\n", totalWins);
   printf("  > Win/Loss ratio: %d%%\n", (totalWins/totalRounds) * 100);
   printf("  > Total guesses: %d\n", totalGuesses);
   printf("  > Avg. guesses per round: %.1f\n", (float)totalGuesses/totalRounds);
   putchar('\n');

   // exit with code 0 for success
   return 0;
}


// Subroutine definitions

/**
 * A helper function to play one self-contained round of the game.
 */
void playRound() {
   // Variable declarations
   bool roundOver;       // flag to check if the current round is over

   char* endPtr;        // pointer to the first character strol() can't convert (used in input validation)

   int randomNum;       // the randomly generated number
   long guess;          // number that the player has guessed, converted from the raw String input
   int timesGuessed;    // number of guesses made during the current round


   // Subroutine logic

   // initialise flag to "false"
   roundOver = false;

   // initialise the number of guesses to 0
   timesGuessed = 0;

   // generate the random number 
   randomNum = generateRandomInt(MIN_NUMBER, MAX_NUMBER);

   // show round start message
   printf("\n<<New Round Start>>\n");

   // GAME-ROUND LOOP
   while (!roundOver) {
      // ask the user to input a new guess on every loop iteration
      printf("\nEnter your guess: ");

      // attempt to read the user input (function returns "false" if there is an error)
      if (readInput()) {

         // try to convert the input to an integer
         // Note: strtol() saves the address of the first character it could not convert to the variable "endPtr"
         guess = strtol(userInput, &endPtr, 10);  // Note: we pass 10 as argument for base 10 numbers 
      
         // either no characters could be converted to digits (both pointers point to the same address)
         // or endPtr does not point to end-of-String (\0) nor newline (meaning we encountered a non-digit character)
         if ((userInput == endPtr) || (*endPtr && *endPtr != '\n')) {
            printf("Error: invalid input. Please input only numbers.\n");


         // input was successfully converted to a number 
         // check if it is outside of the specified range
         } else if (guess < MIN_NUMBER || guess > MAX_NUMBER) {
            printf("The guess is outside of bounds, try again!\n");


         // input is a valid guess
         } else {
            // increment the number of guesses
            timesGuessed++;

            // guess was either too low or too high
            if (guess < randomNum || guess > randomNum) {
               // indicate whether guess was too low or too high
               printf("Too %s!\n", guess < randomNum ? "low" : "high"); // return the correct String based the ternary outcome

               // dynamically display the remaining number of guesses
               // print "guess" when there is exactly 1 guess remaining, otherwise print "guesses"
               printf("You have %d %s left...\n", 
                  MAX_GUESSES - timesGuessed,  
                  timesGuessed == (MAX_GUESSES - 1) ? "guess" : "guesses"
               );      
            }

            // guess was correct
            if (guess == randomNum) {
               // update win statistics
               totalWins++;

               // set flag to "true" to break out of the game-round loop
               roundOver = true;

               // display "win" message
               printf("Your have guessed %d %s and your guess is correct - you win! :)\n", 
                  timesGuessed, 
                  timesGuessed > 1 ? "times" : "time"
               );

            // if not correct, see if the player has any guesses left
            } else {
               // if the player ran out of guesses the round is over
               if (timesGuessed >= MAX_GUESSES) {
                  // set round over to "true"
                  roundOver = true;

                  // print "round over" message
                  printf("Round over... You ran out of guesses!\n");
               }
            }
         }
      }
   }  /* end of game-round loop */


   // Post-round logic

   // add the guesses from this round to the total tracker
   totalGuesses += timesGuessed;

   // increment total rounds played
   totalRounds++;
}

/**
 * Reads the stdin input stream and stores it in the global input buffer.
 * Uses fgets() to ensure no buffer overflow.
 * @returns - boolean indicating the success
 */
bool readInput() {
   // Variable declaration
   bool inputWasRead;  // boolean indicating whether the input was successfully read
   
   // Function logic

   // fgets() returns a NULL pointer if there is any error reading the stdin input stream,
   // which then triggers the error message
   // Note: we use fgets to ensure we don't overflow our buffer array.
   if(fgets(userInput, MAX_INPUT_LEN, stdin)) {
      // input successfully read, set "true"
      inputWasRead = true;
   } else {
      // error when attempting to read the input stream
      printf("Error reading input... try again!\n");
      printf("Press 'enter' to continue");
      // set "false"
      inputWasRead = false;
   }

   // ensure no garbage is left in the input stream
   clearInputStream();

   // return the boolean indicating success
   return inputWasRead;
}


/**
 * A helper function to clear the stdin input stream. 
 */
void clearInputStream() { 
   // if the buffer does not end in '\n' there is garbage data left in the stdin input stream
   // and we have to clear it before reading any new input from the user

   // Note: this guard clause prevents user getting stuck in an "infinite" loop if the stream is actually empty,
   // in which case the user must press "enter" again to escape it
   if (userInput[strlen(userInput) - 1] != '\n') {
      // declare temp variable
      int temp;
      // clear stdin input stream
      while ((temp = getchar()) != '\n');
   }
}

/**
 * Generates random integer number between the given range. 
 * @param {int} low - the lower bound of the range
 * @param {int} high - the upper bound of the range
 * @returns {int} - the newly randomly generated integer 
 */
int generateRandomInt(int low, int high) {
   // put the generated int in a modulo expression to ensure we get number within the defined range
   return (rand() % (high - low + 1)) + low;  
}

