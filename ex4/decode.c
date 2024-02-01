// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 2
// Exercises 4
// Submission code: <XXXYYY>

// Include section
#include <stdio.h>
#include <stdlib.h>

// Macro section
#define MAX_VALUE 0xCB // max possible value for a vehicle configuration
#define MIN_VALUE 0x0  // min possible value for a vehicle configuration

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
void printBinary(byte number); // TODO remove


// Main program section
int main(int argc, char **argv) {
   // Variable declarations
   long input;    // stores the hexadecimal input 
   
   byte *endPtr;  // points to the last character strtol can't convert

   byte vehicleConfig;  // vehicle configuration converted from the input

   // Program logic
   printf("<< Program Start >>\n\n");

   // check that the amount of command-line arguments are correct (exactly 5)
   if (argc != 2) {
      // show error message and usage hint
      printf("Error: please provide exactly 1 command-line argument!\n");
      printf("> Example: \"AB\"\n");

      // exit program early with error code 1
      return 1;
   }

   // convert the input argument to a byte 
   input = strtol(argv[1], (char **)&endPtr, 0x10);   // Note: we pass base 16 to read hexadecimal

   // if the end pointer returned by the strtol function does not point to a value of 0, then the
   // argument was not successfully converted, meaning the input was not a valid hexadecimal number
   if (*endPtr != 0) {
      printf("Error: input argument is not a valid hexadecimal!\n");
      // exit program early with error code 1 
      return 1;
   }

   // check if the input value is greater than the max possible configuration value
   // or lower than the minimum possible configuration value
   if (input > MAX_VALUE || input < MIN_VALUE) {
      printf("Error: value of input argument is out-of-bounds!\n");
      printf("> Min possible configuration is \"0x00\"\n");
      printf("> Max possible configuration is \"0xCB\"\n");

      // exit program early with error code 1 
      return 1;
   }

   // input was successfully read and is within the lower and upper bounds
   // convert it to byte
   vehicleConfig = (byte)input;


   // now check that the bounds for the range of specific parts are valid

   // check that the gear values is not higher than its max range
   if ((vehicleConfig >> GEAR & (BIT3 + BIT2 + BIT1)) > GEAR_MAX) {
      printf("Error: invalid input!\n");
      printf("> Gear position can only be between 0-4\n");

      // exit program early with error code 1
      return 1;
   }
   // check that the key value is not higher than its max range
   if ((vehicleConfig >> KEY & (BIT2 + BIT1)) > KEY_MAX) {
      printf("Error: invalid input!\n");
      printf("> Key position can only be between 0-2\n");

      // exit program early with error code 1
      return 1;
   }

   // control prints //TODO remove
   printf("raw value: %d\n", input);
   printBinary(input);
   printf("converted value: %d\n", vehicleConfig);
   printBinary(vehicleConfig);
   

   // print the unpacked byte values
   printf("\nName         Value\n");
   printf("------------------\n");
   printf("engine_on    %d\n", vehicleConfig & BIT8 >> ENGINE);
   printf("gear_pos     %d\n", (vehicleConfig & (BIT7 + BIT6 + BIT5)) >> GEAR);
   printf("key_pos      %d\n", (vehicleConfig & (BIT4 + BIT3)) >> KEY);
   printf("brake1       %d\n", vehicleConfig & BIT2 >> BRAKE1);
   printf("brake2       %d\n", vehicleConfig & BIT1 >> BRAKE2);

   // exit program with code 0 for success
   return 0;
}

// Subroutine definition(s)

// TODO: remove
void printBinary(byte number) {
   const byte BYTE_MASK = 1 << 7;     // byte bitmask containing a 1 in the leftmost bit
   // print the binary representation of the byte by checking
   // each individual bit and printing 1 or 0 (only non-leading 0's) 
   printf("Vehicle: 0b");
   // we loop through each individual bit of the byte (left>right)
   // by left-shifting the byte and applying a bitmask using bitwise AND,
   // hence checking whether the current leftmost bit is 1 or 0
   for (byte i = 0; i < sizeof(byte) * 8; i++) {
      // left-shift by i positions and apply the bitmask
      // if the most significant bit is 1, we get a non-zero value
      // and the if-statement evaluates to true
      if (number << i & BYTE_MASK) {
         putchar('1');
      } else {
         putchar('0');
      }
   }
   // print new line after loop is finished
   putchar('\n');
}
