// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 2
// Exercises 4
// Submission code: <XXXYYY>

// Include section
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Macro section
// TODO zero-index instead?
#define BIT1 0x01    //00000001
#define BIT2 0x02    //00000010
#define BIT3 0x04    //00000100
#define BIT4 0x08    //00001000
#define BIT5 0x10    //00010000
#define BIT6 0x20    //00100000
#define BIT7 0x40    //01000000
#define BIT8 0x80    //10000000

#define NUM_OF_PARTS 5 // number of vehicle parts

// start bit position of each vehicle part
#define ENGINE 7  // starting bit position of the engine
#define GEAR   4  // starting bit position of the gear
#define KEY    2  // starting bit position of the key
#define BRAKE1 1  // starting bit position of break1
#define BRAKE2 0  // starting bit position of break2

// max value ranges for each vehicle part
#define ENGINE_MAX 1
#define GEAR_MAX 4
#define KEY_MAX 2
#define BRAKE_MAX 1

// Type definition(s)
typedef unsigned char byte;   // one 'byte' (8 bits) 

// Function prototype declaration(s)

bool valueWithinRange(char digit, int max);    // validate input argument is within defined range

void packBits(byte *vehicle, char instructions[]);  // function to pack all the instructions into a byte
void setEngine(byte *vehicle, char ignitionState);  // function to set the engine on/off
void setGear(byte *vehicle, char position);         // function to set the gear position
void setKey(byte *vehicle, char position);          // function to set the key position
void setBrakeOne(byte *vehicle, char position);     // function to set the first brake position
void setBrakeTwo(byte *vehicle, char position);     // function to set the second brake position

void printBinary(byte vehicle); // TODO remove


// Main program section
int main(int argc, char **argv) {
   // Variable declarations
   int maxRanges[] = {  // store the max ranges of each vehicle part in an array
      ENGINE_MAX,       // so we can use it to validate the input arguments
      GEAR_MAX, 
      KEY_MAX, 
      BRAKE_MAX, 
      BRAKE_MAX 
   };

   char vehicleInstructions[NUM_OF_PARTS];   // store the individual instructions in an array

   byte vehicle;  // byte that we pack the vehicle instructions (bits) into


   // Program logic
   printf("<< Program Start >>\n\n");

   // check that the amount of command-line arguments are correct (exactly 5)
   if (argc != 6) {
      // show error message and usage hint
      printf("Error: please provide exactly 5 command-line arguments!\n");
      printf("> Example: \"1 2 2 1 1\"\n");

      // exit program early with error code 1
      return 1;
   }

   // validate that all arguments are within defined ranges
   // iterate over all the input arguments
   for (int i = 0; i < argc - 1; i++) {
      // save the current argument
      char* argument = argv[i + 1]; // we add 1 since the first argument is the program call

      // make sure that the argument is exactly one in length
      if (strlen(argument) != 1) {
         // print error message
         printf("Error: invalid input argument!\n");
         printf("> Make sure that each argument is exactly one digit and that they are within the defined ranges!\n");
         // exit program early with error code 1
         return 1;
      }

      // make sure that the argument is within its defined ranges
      if (!valueWithinRange(*argument, maxRanges[i])) {
         // print error message
         printf("Error: invalid input argument!\n"); 
         printf("> Command-line argument #%d is invalid! (max value: %d)\n", i + 1, maxRanges[i]);
         // exit program early with error code 1
         return 1;
      }

      // save the now validated argument instruction to the array
      vehicleInstructions[i] = *argument;
   }

   // initiate the vehicle to 0 so we can set the bits manually
   vehicle = 0;

   // pack all the bit-values into a single byte
   packBits(&vehicle, vehicleInstructions);

   printf("Decimal: %d\n", vehicle); // TODO remove
   printBinary(vehicle);   //TODO remove

   // print the byte in hexadeximal form
   printf("%X\n", vehicle);

   // return exit code 0 for success
   return 0;
}

// Subroutine definition(s)

/**
 * Validates that the input argument is within the given range (0 to <max>).
 * @param {char} argument: the argument to validate
 * @param {int} max: the upper bound of the range
 * @returns {bool}: boolean indicating if the argument was within range
*/
bool valueWithinRange(char argument, int max) {
   // compare the character to digit ASCII codes in a loop
   for (int i = 0; i <= max; i++) {
      // add 'i' to the value of the '0' character
      // this means we check the argument against every digit between 0 to max
      if (argument == '0' + i) return true;  // return true if there is a match
   }
   // if no match return false
   return false;
}

// TODO: remove
void printBinary(byte vehicle) {
   const byte BYTE_MASK = 1 << 7;     // byte bitmask containing a 1 in the leftmost bit
   // print the binary representation of the byte by checking
   // each individual bit and printing 1 or 0 (only non-leading 0's) 
   printf("Binary: 0b");
   // we loop through each individual bit of the byte (left>right)
   // by left-shifting the byte and applying a bitmask using bitwise AND,
   // hence checking whether the current leftmost bit is 1 or 0
   for (byte i = 0; i < sizeof(byte) * 8; i++) {
      // left-shift by i positions and apply the bitmask
      // if the most significant bit is 1, we get a non-zero value
      // and the if-statement evaluates to true
      if (vehicle << i & BYTE_MASK) {
         putchar('1');
      } else {
         putchar('0');
      }
   }
   // print new line after loop is finished
   putchar('\n');
}

/**
 * Helper function to set all the bits in the byte by calling the correct methods.
 * @param
 * @param //TODO
*/
void packBits(byte *vehicle, char instructions[]) {
   // set the engine on/off
   setEngine(vehicle, instructions[0]);
   // set the gear position
   setGear(vehicle, instructions[1]);
   //set the key position
   setKey(vehicle, instructions[2]);
   //set the brake1 position
   setBrakeOne(vehicle, instructions[3]);
   //set the brake2 position
   setBrakeTwo(vehicle, instructions[4]);
}

/**
 * Sets the engine on or off.
 * @param //TODO
*/
void setEngine(byte *vehicle, char ignitionState) {
   // set the ignition state of the engine by left-shifting the ignition instructions
   // to the starting position of the engine and applying the MSB-bitmask 
   *vehicle += ignitionState << ENGINE & BIT8;
}

/** //TODO
 * @param
*/
void setGear(byte *vehicle, char position) {
   // set the gear position by left-shifting the gear position instructions
   // to the starting position of the gear box and applying the corresponding bitmask
   *vehicle += position << GEAR & (BIT7 + BIT6 + BIT5);
}

/** //TODO
 * @param
*/
void setKey(byte *vehicle, char position) {
   // set the key position by left-shifting the key position instructions
   // to the starting position of the key and applying the corresponding bitmask
   *vehicle += position << KEY & (BIT4 + BIT3);
}

/** //TODO
 * @param
*/
void setBrakeOne(byte *vehicle, char position) {
   // set the break position by left-shifting the brake position instructions
   // to the starting position of the first brake and applying the corresponding bitmask
   *vehicle += position << BRAKE1 & BIT2;
}

/** //TODO
 * @param
*/
void setBrakeTwo(byte *vehicle, char position) {
   // set the break position by left-shifting the brake position instructions
   // to the starting position of the second brake and applying the corresponding bitmask
   *vehicle += position << BRAKE2 & BIT1;
}
