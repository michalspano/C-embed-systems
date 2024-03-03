// Include section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Type definitions
typedef unsigned char byte; // To represent a byte, 8 bits.

// Function prototypes - API
_Bool is_numeric(const char* buff);

// An enum to hold the maximum values of each listed component of the elevator.
typedef enum {
  ENGINE_ON_MAX = 1,
  FLOOR_POS_MAX = 7,
  DOOR_POS_MAX  = 1, // Note: 2 bits, albeit 0 or 1 (value)
  BRAKE_MAX     = 1  // Both for brake1 and brake2
} CAR_GEAR_RANGE;

// A struct to represent the abstraction of an elevator per the instructions.
typedef struct {
  int engine_on;
  int floor_pos;
  int  door_pos;
  int    brake1;
  int    brake2;
} ELEVATOR;

/**
 * The main entry point of the program. This program relies on command-line
 * arguments, hence `argc` and `argv` are used.
 */
int main(int argc, char* argv[]) {
  byte pByte = 0b0; // A buffer to store the 'packed' byte.

  // Six command-line arguments are assumed, namely the file's basename and
  // then 5 different numbers with different ranges). The ranges are verified
  // later.
  if (argc != 6) {
    printf("Error: invalid number of arguments.\n"); return 0;
  // Alternatively, the help flag is passed (argc = 2). Detect it and display
  // the message.
  } else if (argc == 2) {
    if (strcmp(argv[1], "-h")) {
      printf("Usage: %s [-h] <engine_on> <floor_pos> <door_pos> <brake1> <brake2>\n", argv[0]);
      return 0;
    }
  // Verify that argument 1 has the valid type (i.e., is a number). If not,
  // prompt the user and return 0.
  } else if (!is_numeric(argv[1])) {
    printf("Error: argument 1 is of invalid type.\n");
    return 0;
  // Verify that argument 2 has the valid type (i.e., is a number). If not,
  // prompt the user and return 0.
  } else if (!is_numeric(argv[2])) {
    printf("Error: argument 2 is of invalid type.\n");
    return 0;
  // Verify that argument 3 has the valid type (i.e., is a number). If not,
  // prompt the user and return 0.
  } else if (!is_numeric(argv[3])) {
    printf("Error: argument 3 is of invalid type.\n");
    return 0;
  // Verify that argument 4 has the valid type (i.e., is a number). If not,
  // prompt the user and return 0.
  } else if (!is_numeric(argv[4])) {
    printf("Error: argument 4 is of invalid type.\n");
    return 0;
  // Verify that argument 5 has the valid type (i.e., is a number). If not,
  // prompt the user and return 0.
  } else if (!is_numeric(argv[5])) {
    printf("Error: argument 5 is of invalid type.\n");
    return 0;
  }

  // Populate the elevator construct with the passed values. Herein, we may assume
  // that the values can be parsed to integers, because the previous step did
  // not fail.
  ELEVATOR c = (ELEVATOR) {
    atoi(argv[1]), // engine_on
    atoi(argv[2]), // floor_pos
    atoi(argv[3]), // door_pos
    atoi(argv[4]), // brake1
    atoi(argv[5]), // brake2
  };
 
  // Verify the individual values of the struct. Use the `GEAR_RANGE` enum to
  // obtain the maximum values. All minimum values are 0. If a certain interval
  // is not met, the program prompts the user, and 1 is returned to indicate an
  // error. This check is rather exhaustive, but it is necessary to ensure that
  // the input is valid.
  if (c.engine_on < 0 || c.engine_on > ENGINE_ON_MAX) {
    // Verify for `engine_on`.
    printf("Error: invalid value for the parameter `engine_on`.\n");
    return 0;
  } else if (c.floor_pos < 0 || c.floor_pos > FLOOR_POS_MAX) {
    // Verify for `gear_pos`.
    printf("Error: invalid value for the parameter `floor_pos`.\n");
    return 0;
  } else if (c.door_pos < 0 || c.door_pos > DOOR_POS_MAX) {
    // Verify for `key_pos`.
    printf("Error: invalid value for the parameter `door_pos`.\n");
    return 0;
  } else if (c.brake1 < 0 || c.brake1 > BRAKE_MAX) {
    // Verify for `brake1`.
    printf("Error: invalid value for the parameter `brake1`.\n");
    return 0;
  } else if (c.brake2 < 0 || c.brake2 > BRAKE_MAX) {
    // Verify for `brake2`.
    printf("Error: invalid value for the parameter `brake2`.\n");
    return 0;
  }

  /* ============================= Algorithm ============================= */

  // Note: this algorithm could perhaps be refactored in a less verbose manner,
  // however, for the sake of readability, it is kept as is.

  // The least significant bit. Merely add the values to the byte.
  // No shifting is necessary. By 'add' we mean the bitwise OR operation which
  // will simply replace the 0s with the values of the struct.
  pByte |= c.brake2;

  // The next bit. We desire to shift the bits to the left by 1. Afterwards, we
  // 'add' the value of `brake1` to the byte.
  c.brake1 <<= 1;
  pByte |= c.brake1;

  // The next two bits. We shift the bits by 1 (from before). By 2 and then -1,
  // because we have a 2-bit value (a masked boolean). Afterwards, we 'add' the
  // value of `door_pos` to the byte.
  c.door_pos <<= 2; 
  // The same as the following:
  // c.door_pos <<= 1 + 2 - 1; 
  pByte |= c.door_pos;

  // Same as the before, but we have a 3-bit value. We shift the bits by 1 (from
  // before). By 2 and then -2, because we have a 3-bit value. Afterwards, we
  // 'add' the value of `floor_pos` to the byte.
  c.floor_pos <<= 4;
  // The same as the following:
  // c.floor_pos <<= 1 + 2 + 3 - 2;
  pByte |= c.floor_pos;

  // Lastly, we shift by 7 (to get the most significant bit). We then 'add' the
  // value of `engine_on` to the byte.
  c.engine_on <<= 7; 
  // The same as the following:
  // c.engine_on <<= 1 + 2 + 3 + 1 - 0;
  pByte |= c.engine_on;

  /* ============================= Algorithm ============================= */

  // Print the obtained hexadecimal value. Use '%X' to print the value in
  // capital. Afterwards, return 0 to indicate a successful completion of the
  // program.
  printf("0x%X\n", pByte);
  return 0;
}

/**
 * A helper function to verify that a certain char sequence contains strictly
 * digits.
 *
 * @param buff - the buffer to verify
 * @returns    - a boolean value indicating whether the buffer contains only
 * digits or not.
 */
_Bool is_numeric(const char* buff) {
  // Iterate over the length of the buffer.
  for (int i = 0; i < strlen(buff); i++) {
    // Check if the current char is a digit, if not, interrupt the loop and
    // return false.
    if (!isdigit(buff[i])) return 0;
  }
  // Otherwise, return true.
  return 1;
}
