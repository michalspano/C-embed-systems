// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 3
// Exercise 4
// Submission code: <XXXYYY>

// Macros section
#define ROW_COUNT  4    // # of rows of the keypad
#define COL_COUNT  4    // # of columns of the keypad
#define f_BAUD     9600 // Frequency of the serial port
#define loop_delay 1000 // 1 second delay each key reading

unsigned long time_buff = 0; // Used with `millis()` for non-blocking

// Ports that represents the rows of the keypad
uint8_t PIN_ROWS[ROW_COUNT] = { 11, 10, 9, 8 };

// Ports that represents the columns of the keypad
uint8_t PIN_COLS[COL_COUNT] = { 7,  6,  5, 4 };
 
// The keypad layout that we're using (4x4) based on the provided template.
const char KEYS[ROW_COUNT][COL_COUNT] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

/* Set up all required components */
void setup() {
  // Set up all ports for rows with the OUTPUT mode.
  for (uint8_t i = 0; i < ROW_COUNT; i++) {
    pinMode(PIN_ROWS[i], OUTPUT);
  }
  
  // Set up all ports for columns with the INPUT mode.
  for (uint8_t i = 0; i < COL_COUNT; i++) {
    pinMode(PIN_COLS[i], INPUT);
  }

  // Realize the serial monitor at the given frequency port.
  Serial.begin(f_BAUD);
}

/* Loop of the program */
void loop() {
  // Get the current time since the program elapsed.
  unsigned long current_time = millis();
 
  // Iterate over all rows of the keypad
  for (uint8_t i = 0; i < ROW_COUNT; i++) {
    // Send a signal to each row
    digitalWrite(PIN_ROWS[i], LOW);

    // Iterate over all columns of the keypad
    for (uint8_t j = 0; j < COL_COUNT; j++) {

      // Read the signal at the current column
      int signal = digitalRead(PIN_COLS[j]);

      // If signal was found (i.e., is LOW) and the delay has debounced,
      // print the pressed character to the serial monitor.
      //
      // With the help of the `millis()` function, we can delay an iteration
      // in a non-blocking fashion. If `delay()` were to be used, it would be
      // placed inside the conditional (after calling `println()`).
      if (signal == LOW && current_time > time_buff + loop_delay) {
        char key  = KEYS[i][j];   // Map the signal to a char (i.e., key)
        time_buff = current_time; // Update the time buffer
        Serial.println(key);      // Print the key
      }
    }

    // Reset the row (i.e., set the signal to HIGH)
    digitalWrite(PIN_ROWS[i], HIGH);
  }
}
