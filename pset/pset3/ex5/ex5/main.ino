// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 3
// Exercise 5
// Submission code: 0173549

// code has been adapted from example code by Mark Stanley & Alexander Brevig for the Keypad.h library
// github repo: https://github.com/Chris--A/Keypad/blob/master/examples/CustomKeypad/CustomKeypad.ino

// include section
#include <Keypad.h>                     // include library for using matrix style keypads with Arduino

// define section
#define SERIAL_BAUD_RATE 9600           // define baud rate for serial monitor

// variable declarations
const byte ROWS = 4;                    // number of rows on keypad
const byte COLS = 4;                    // number of columns on keypad

byte ROW_PINS[ROWS] = {11, 10, 9, 8};   // array mapping pins to rows
byte COL_PINS[COLS] = {7, 6, 5, 4};     // array mapping pins to columns

// multidimensional array represents keypad
// values correspond to actual keypad's row/column configuration 
char keypad[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// initialize custom keypad using keypad.h library function
Keypad customKeypad = Keypad(makeKeymap(keypad), 
                             ROW_PINS, 
                             COL_PINS, 
                             ROWS, 
                             COLS); 

/*
* Setup function runs once at startup
*/
void setup() {

  // setup serial monitor as output with defined baud rate
  Serial.begin(SERIAL_BAUD_RATE);
}

/*
* Main loop runs continuously
*/
void loop()
{
  // call keypad.h library function to store the value of any pressed key
  char customKey = customKeypad.getKey();
  
  // if the value of customKey is truthy (!= 0)
  if (customKey){

    // print the character to the serial monitor
    Serial.println(customKey);
  }
}
