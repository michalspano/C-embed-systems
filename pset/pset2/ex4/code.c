// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 2
// Exercises 4
// Submission code: 5129972

// Include section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Macro section
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

bool isNumeric(char *buffer);    // function to check if all the characters in a String are digits

void packBits(byte *vehicleConfig, int settings[]);      // function to pack all the vehicle settings into a byte
void setEngine(byte *vehicleConfig, int ignitionState);  // function to set the engine on/off
void setGear(byte *vehicleConfig, int position);         // function to set the gear position
void setKey(byte *vehicleConfig, int position);          // function to set the key position
void setBrakeOne(byte *vehicleConfig, int position);     // function to set the first brake position
void setBrakeTwo(byte *vehicleConfig, int position);     // function to set the second brake position


// Main program section
int main(int argc, char **argv) {
   // Variable declarations
   int maxRanges[] = {  // store the max ranges of each vehicle part in an array
      ENGINE_MAX,       // so we can use it to validate the input arguments
      GEAR_MAX, 
      KEY_MAX, 
      BRAKE_MAX,        // Note: both break 1 and 2 share the same max value
      BRAKE_MAX
   };

   int setting;    // integer value containing a setting for a vehicle part

   int vehicleSettings[NUM_OF_PARTS];   // stores all the individual vehicle part settings

   byte vehicleConfig;  // byte that we pack all of the vehicle settings into


   // Program logic
   printf("<< Program Start >>\n\n");

   // check that the amount of command-line arguments are correct (exactly 5)
   if (argc != 6) {
      // show error message and usage hint
      printf("Error: please provide exactly 5 command-line arguments!\n");
      printf("> Example: \"1 2 2 0 1\"\n");

      // exit program early with error code 1
      return 1;
   }

   // validate that all arguments are within defined ranges by iterating over all the input arguments
   for (int i = 0; i < argc - 1; i++) {
      // save the current argument
      char *argument = argv[i + 1]; // we add 1 since the first argument is the program call 

      // validate that the input argument String is only digits
      if (!isNumeric(argument)) {
         // print error message
         printf("Error: invalid input argument!\n");
         printf("> Make sure that each argument is numeric and that they are within the defined ranges!\n");
         // exit program early with error code 1
         return 1;
      }

      // convert the argument to an int so we can validate that it is within the correct range
      // Note: while we could just assume that each input arg is a single digit and just treat it as a char, this
      // ensures that the program logic is compatible even if the max range of any part is increased to a double digit number
      setting = atoi(argument);

      // make sure that the argument is within the defined ranges of the corresponding part
      if ((setting < 0 || setting > maxRanges[i])) {
         // print error message
         printf("Error: invalid input argument!\n"); 
         printf("> Command-line argument #%d is invalid! (max value: %d)\n", i + 1, maxRanges[i]);
         // exit program early with error code 1
         return 1;
      }

      // save the now validated setting to the array
      vehicleSettings[i] = setting;
   }

   // initiate the vehicle to 0 so we can set each bit individually
   vehicleConfig = 0b0;

   // pack all the bit-values into a single byte
   packBits(&vehicleConfig, vehicleSettings);

   // print the single byte containing all the vehicle configurations, in hexadecimal form
   printf("%X\n", vehicleConfig);   // capital 'X' to print hexadecimal in upper-case

   // return exit code 0 for success
   return 0;
}


// Subroutine definition(s)

/**
 * Check that a String is made up of only numeric characters.
 * @param char* buffer - the String to validate
 * @return [boolean] - indicating whether the String is all digits 
*/
bool isNumeric(char *buffer) {
   // loop through each character of the String
   for (int i = 0; i < strlen(buffer); i++) {
      // check if the current character is numeric by comparing with the ASCII values of the 0 and 9 digits
      if (buffer[i] < '0' || buffer[i] > '9') {
         // return false if it is not
         return false;
      }
   }
   // when loop has finished we know that the String was only numeric characters
   return true;
}

/**
 * Helper function to set all the bits in the byte by calling the correct methods.
 * @param byte* config - the byte variable to pack with the settings
 * @param int settings[] - array of integers containing the individual settings
*/
void packBits(byte *config, int settings[]) {
   // set the engine on/off
   setEngine(config, settings[0]);
   // set the gear position
   setGear(config, settings[1]);
   //set the key position
   setKey(config, settings[2]);
   //set the brake1 position
   setBrakeOne(config, settings[3]);
   //set the brake2 position
   setBrakeTwo(config, settings[4]);
}

/**
 * Sets the engine on or off.
 * @param byte* config - reference to the vehicle config
 * @param int ignitionState - the ignition setting to pack into the vehicle config
*/
void setEngine(byte *config, int ignitionState) {
   // set the ignition state of the engine by left-shifting the ignition setting
   // to the starting position of the engine and applying the corresponding bitmask 
   *config += ignitionState << ENGINE & BIT8;
}

/**
 * Sets the position of the gears.
 * @param byte* config - reference to the vehicle config
 * @param int gearPosition - the gear setting to pack into the vehicle config
*/
void setGear(byte *config, int gearPosition) {
   // set the gear position by left-shifting the gear position setting
   // to the starting position of the gear box and applying the corresponding bitmask
   *config += gearPosition << GEAR & (BIT7 + BIT6 + BIT5);
}

/**
 * Sets the position of the key.
 * @param byte* config - reference to the vehicle config
 * @param int keyPosition - the key setting to pack into the vehicle config
*/
void setKey(byte *config, int keyPosition) {
   // set the key position by left-shifting the key position setting
   // to the starting position of the key and applying the corresponding bitmask
   *config += keyPosition << KEY & (BIT4 + BIT3);
}

/**
 * Sets the position of brake 1.
 * @param byte* config - reference to the vehicle config
 * @param int brakePosition - the brake setting to pack into the vehicle config
*/
void setBrakeOne(byte *config, int brakePosition) {
   // set the break position by left-shifting the brake position setting
   // to the starting position of the first brake and applying the corresponding bitmask
   *config += brakePosition << BRAKE1 & BIT2;
}

/**
 * Sets the position of brake 2.
 * @param byte* config - reference to the vehicle config
 * @param int brakePosition - the brake setting to pack into the vehicle config
*/
void setBrakeTwo(byte *config, int brakePosition) {
   // set the break position by left-shifting the brake position setting
   // to the starting position of the second brake and applying the corresponding bitmask
   *config += brakePosition << BRAKE2 & BIT1;
}
