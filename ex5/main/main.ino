// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 3
// Exercise 5
// Submission code: <XXXYYY>

// Includes section
#include <Keypad.h>

// Macros section
#define f_BAUD     9600   // Serial port to write to
#define LOOP_DELAY 1000   // Delay in each iteration

const byte ROW_COUNT = 4; // # of rows of the keypad
const byte COL_COUNT = 4; // # of columns of the keypad

unsigned long time_buffer = 0; // Time buffer used with `millis()`.

// Pins that hold each individual row of the keypad
byte PIN_ROWS[ROW_COUNT] = { 11, 10, 9, 8 };

// Pins that hold each individual columns of the keypad
byte PIN_COLS[COL_COUNT] = { 7,  6,  5, 4 };

// The custom layout of the keypad that is used (based on the instructions of
// the assignment).
const char KEYS[ROW_COUNT][COL_COUNT] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

// Make a new instance of `Keypad`. Populate the constructor with the required
// attributes.
Keypad keypad = Keypad(
  makeKeymap(KEYS),    // custom key layout
  PIN_ROWS, PIN_COLS,  // pins for rows, cols
  ROW_COUNT, COL_COUNT // #rows, #cols
);

/* Set up the required components */
void setup() {
  // Begin a serial monitor at the chosen port
  Serial.begin(f_BAUD);
}

/* Loop of the program */
void loop() {
  // Get the current time
  unsigned long current_time = millis();

  // Get a key that is currently pressed
  char key = keypad.getKey();

  // If the key is not empty and the delay interval has debounced,
  // print the pressed key. Note, we're using `millis()` for delaying
  // the execution in a non-blocking fashion (by 1 second).
  if (key && current_time > time_buffer + LOOP_DELAY) {
    // Update the current time buffer with the new time
    time_buffer = current_time;
    Serial.println(key);
  }
}
