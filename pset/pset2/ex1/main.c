// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 2
// Exercise 1 
// Submission code: 5129972

// Includes section
#include <stdio.h>
#include <string.h>

// Macros section
#define POS_MAX 99  // The maximum allowed value for x, y
#define DIR_COUNT 4 // The number of direction
// A formatted message with the instructions of the program.
#define HELP_MSG "Write 'm' to move (by 1 unit) or 't' to"\
                 " turn (by 90deg clockwise) the robot."\
                 "\nThese command are chained in a sequence,"\
                 " entered when the '>' symbol is displayed."\

// Enumeration of the possible directions of the robot.
enum DIRECTION { N, E, S, W };

// Definition of a robot (x, y and direction)
typedef struct {
  int xpos;           // The x position
  int ypos;           // The y position
  enum DIRECTION dir; // The current direction
} ROBOT;

// Function prototypes
void move(ROBOT *r);    // Move the robot in the direction by one unit.
void turn(ROBOT *r);    // Turn the robot clockwise by 90 deg.
void clearBuffer(void); // Clear chars from the buffer until new-line or EOF.

/**
 * Main program section. The program does not use any command-line arguments
 * provided when the executable is run.
 */
int main(void) {
  int x, y;                // Declare placeholders for x, y values read from the
                           // user.
  char loop_flag;          // A char to determine if the program is to be exited.
  char line[256];          // Declare a buffer for one line with at most 256 chars.

  // Prompt the user, explain the program (in a readable and pleasant manner).
  printf("\n\t\t\t\t***ROBOT***\n");
  printf("%s\n\n", HELP_MSG);

  do {
    // Attempt to read an int from the user. Herein, we assume that the user
    // provides an arbitrary integer value.
    printf("x: "); scanf("%d", &x);

    // Validate the provided value, it must be between 0-POS_MAX. In case it
    // is not, skip the current iteration of the loop.
    if (x < 0 || x > POS_MAX) {
      printf("Error: invalid x position.\n"); continue;
    }

    // Attempt to read an int from the user. Herein, we assume that the user
    // provides an arbitrary integer value.
    printf("y: "); scanf("%d", &y);
 
    // Validate the provided value, it must be between 0-POS_MAX. In case it
    // is not, skip the current iteration of the loop.
    if (y < 0 || y > POS_MAX) {
      printf("Error: invalid y position.\n"); continue;
    }

    // Declare a robot instance given the x and y coordinates provided from the
    // user. By default, the NORTH (N) direction is assigned. Because `rbt` is
    // not used outisde the do-while block, we can declare it here. This way, we
    // ensure that each 'round', the robot is reset properly, and the default
    // direction is assigned. Ultimately, the reason why the robot is declared
    // here is that we only create it if the position is valid, so that we don't
    // have to reset it in case of an error, and so forth.
    ROBOT rbt = { x, y, N };

    // Attempt to read one line (i.e., 256 chars) from stdin. The width specified does not
    // include the null-terminator. The user is prompted to enter a command.
    printf("> "); scanf("%255s", line);
    clearBuffer(); // Consume all remaining characters in the buffer.

    // Iterate over each character in the line buffer.
    for (int i = 0; i < strlen(line); i++) {
      // Declare a switch statement. When the character is 'm', call the `move`
      // function, and call the `turn` function for the latter case. The cases
      // are break-ed for the sake of consistency.
      switch (line[i]) {
        case 'm': { move(&rbt); break; }
        case 't': { turn(&rbt); break; }
      }
    }

    // Display to the user the updated position of the robot.
    printf("New position: [%d, %d]\n\n", rbt.xpos, rbt.ypos);

    // Ask the user if they want to continue a new round. If the user enters 'n',
    // the program will exit. Otherwise, the program will continue.
    printf("Continue? [y/n]: ");

    // Read a character from the user, consume all remaining characters in the
    // buffer.
    loop_flag = getchar(); clearBuffer();
  } while (loop_flag != 'n'); // Continue until the user enters 'n'.

  // The user has indicated to exit the program. The program returns 0 to
  // indicate a success. 
  printf("\nExiting...\n"); return 0;
}

/**
 * This function is used to move the robot by one unit in the current
 * direction.
 * @param r - a pointer to a ROBOT struct.
 */
void move(ROBOT *r) {
  // Establish a switch case based on the current direction of the robot (i.e.,
  // r). If the robot is to move right, the y position is decreased. For moving
  // east, the x position is increased. For moving south, the y position is
  // increased. Ultimately, for moving west, the x position is decreased. All
  // cases are 'escaped' with an immediate break. Because the enum has only four
  // possible values, and we check them, there's no need for a `default` case.
  switch (r->dir) {
    case N: { r->ypos--; break; }
    case E: { r->xpos++; break; }
    case S: { r->ypos++; break; }
    case W: { r->xpos--; break; }
  }

  // Ensure that the values of the x, y positions do not exceed the upper boundary,
  // that is, `POS_MAX`. For this, the modulo operator is to be used. We use +1 to
  // ensure that the value can be of `POS_MAX`, albeit not greater than it.
  r->xpos %= (POS_MAX + 1); r->ypos %= (POS_MAX + 1);
}

/**
 * This function is used to turn the robot in the clockwise direction by 90
 * degrees.
 *
 * @param r - a pointer to a ROBOT struct.
 */
void turn(ROBOT *r) {
  // Increment the direction (this is possible because of the nature of the
  // enum) and take the modulo of `DIR_COUNT`. This ensures a full 360deg turn
  // that can be repeated indefinitely.
  r->dir++; r->dir %= DIR_COUNT;
}

/**
 * A helper subroutine to clear the remaining characters in the buffer.
 */
void clearBuffer(void) {
  // Read all characters in the buffer, consume them, terminate when the whole
  // buffer is read or a newline character is encountered.
  char c; while ((c = getchar()) != EOF && c != '\n');
}
