// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 2
// Exercises 4
// Submission code: <XXXYYY>

// Include section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macro section
#define BIT1 0x01    //00000001
#define BIT2 0x02    //00000010
#define BIT3 0x04    //00000100
#define BIT4 0x08    //00001000
#define BIT5 0x10    //00010000
#define BIT6 0x20    //00100000
#define BIT7 0x40    //01000000
#define BIT8 0x80    //10000000

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

// max input length of the given hexadecimal number
#define MAX_INPUT_LEN 2

// Type definition(s)
typedef unsigned char byte;   // one 'byte' (8 bits)

struct Vehicle {  // struct containing all the vehicle part configurations
   byte engine;
   byte gear;
   byte key;
   byte brake1;
   byte brake2;
};


// Main program section
int main(int argc, char **argv) {
   // Variable declarations
   long input;    // stores the hexadecimal input 
   
   char *endPtr;  // points to the last character strtol can't convert

   byte vehicleConfig;  // vehicle configuration converted from the input

   struct Vehicle vehicle; // struct storing each individual vehicle part configuration

   // Program logic
   printf("<< Program Start >>\n\n");

   // check that the amount of command-line arguments are correct (exactly 1)
   if (argc != 2) {
      // show error message and usage hint
      printf("Error: please provide exactly 1 command-line argument!\n");
      printf("> Example: \"AB\"\n");

      // exit program early with error code 1
      return 1;
   }

   // since we pack the value of the given hexadecimal into a byte, it can at most be of length two 
   // as the value of the hexadecimal number would otherwise be greater than what fits into a single byte
   // Note: this prevents unwanted behaviour where a hexadecimal number that is too big would be accepted
   // as long as the last two hexadecimal "digits" represent a valid vehicle config (e.g. AAB). 
   // This allows us to inform the user, and keep the program functionality consistent.
   if (strlen(argv[1]) != MAX_INPUT_LEN) {
      // show error message and usage hint
      printf("Error: input hexadecimal can at most be of length %d!\n", MAX_INPUT_LEN);

      // exit program early with error code 1
      return 1;
   }

   // convert the input argument to a byte 
   input = strtol(argv[1], &endPtr, 16);   // Note: we pass base 16 to read hexadecimal

   // if the end pointer returned by the strtol function does not point to a value of 0, then the
   // argument was not successfully converted, meaning the input was not a valid hexadecimal number
   if (*endPtr != 0) {
      printf("Error: input argument is not a valid hexadecimal!\n");
      // exit program early with error code 1 
      return 1;
   }

   // input was successfully read - convert it to byte
   vehicleConfig = (byte)input;

   // Now we check that the values for values for each part configuration are within bounds.
   // We do this by right-shifting the configuration (input value) by the starting bit position of each part
   // and applying a bitmask corresponding to the number of bits that this part's config is represented by.

   // Note: technically, the parts that are represented with just one bit can't be higher than the max, as they 
   // are only 0 or 1, but by including these checks we ensure that any future changes are easily accommodated
   // by modifying existing code rather than introducing more/new logic.

   // assign the engine value to the struct and check that is not higher than the max range
   if ((vehicle.engine = (vehicleConfig >> ENGINE & BIT1)) > ENGINE_MAX) {
      printf("Error: invalid input!\n");
      printf("> Engine position can only be between (0-%d)\n", ENGINE_MAX);

      // exit program early with error code 1
      return 1;
   }
   // assign the gear value to the struct and check that it is not higher than the max range
   if ((vehicle.gear = (vehicleConfig >> GEAR & (BIT3 + BIT2 + BIT1))) > GEAR_MAX) {
      printf("Error: invalid input!\n");
      printf("> Gear position can only be between (0-%d)\n", GEAR_MAX);

      // exit program early with error code 1
      return 1;
   }
   // assign the key value to the struct and check that it is not higher than the max range
   if ((vehicle.key = (vehicleConfig >> KEY & (BIT2 + BIT1))) > KEY_MAX) {
      printf("Error: invalid input!\n");
      printf("> Key position can only be between (0-%d)\n", KEY_MAX);

      // exit program early with error code 1
      return 1;
   }
   // assign the brake1 value to the struct and check that it is not higher than the max range
   if ((vehicle.brake1 = (vehicleConfig >> BRAKE1 & BIT1)) > BRAKE_MAX) {
      printf("Error: invalid input!\n");
      printf("> Brake1 position can only be between (0-%d)\n", BRAKE_MAX);

      // exit program early with error code 1
      return 1;
   }
   // assign the brake2 value to the struct and check that it is not higher than the max range
   if ((vehicle.brake2 = (vehicleConfig >> BRAKE2 & BIT1)) > BRAKE_MAX) {
      printf("Error: invalid input!\n");
      printf("> Brake2 position can only be between (0-%d)\n", BRAKE_MAX);

      // exit program early with error code 1
      return 1;
   }

   // print the unpacked byte values
   printf("Name         Value\n");
   printf("------------------\n");
   printf("engine_on    %d\n", vehicle.engine);
   printf("gear_pos     %d\n", vehicle.gear);
   printf("key_pos      %d\n", vehicle.key);
   printf("brake1       %d\n", vehicle.brake1);
   printf("brake2       %d\n", vehicle.brake2);

   // exit program with code 0 for success
   return 0;
}
