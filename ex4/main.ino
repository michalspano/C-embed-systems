// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 3
// Exercise 3
// Submission code: <XXXYYY>

// define section
#define ROWS  4               // define number of rows on keypad
#define COLS  4               // define number of columns on keypad
#define SERIAL_BAUD_RATE 9600 // define baud rate for serial monitor
#define DB_INTERVAL 1000      // define time (ms) required between key presses to register input 
                              // (used in delay function to debounce unwanted repeat inputs)

// variable declarations
const int ROW1 = 11;	      // digital pin for row 1
const int COL1 = 7;           // digital pin for column 1

// multidimensional array represents keypad
// values correspond to actual keypad's row/column configuration 
const char keypad[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

/*
* Setup function runs once at startup
* Within it we configure the pins that connect to the keypad
*/
void setup() {

    // loop through rows
    for(int i = 0; i < ROWS; i++) {

        // configure pin representing current row as output pin
        pinMode(ROW1 - i, OUTPUT);
    }

    // loop through columns
    for(int j = 0; j < COLS; j++) {

        // configure pin representing columns as input pin
        pinMode(COL1 - j, INPUT);
    }
  
    // setup serial monitor as output with defined baud rate
    Serial.begin(SERIAL_BAUD_RATE);		
}

/*
* Main loop runs continuously
* Each loop, we check if a key has been pressed
*/
void loop() {

    // loop through rows
    for (int i = 0; i < ROWS; i++) {

        // send a signal through current row
        digitalWrite(ROW1 - i, LOW);

        // loop through columns
        for (int j = 0; j < COLS; j++) {

            // read the current column and store signal in keySignal variable
            int keySignal = digitalRead(COL1 - j);

            // check if the key is pressed (if keySignal == 0)
            // check also that current time is not within certain ms of last registered input
            // this ensures we ignore unwanted input noise / bouncing from physical key presses
            if (keySignal == LOW) {

                // print the corresponding character from the keypad array
                Serial.println(keypad[i][j]);

                // delay the program by 1 second to ensure no unwanted input noise / bouncing is registered
                delay(DB_INTERVAL);
            }
        }

        // reset the current row
        digitalWrite(ROW1 - i, HIGH);
    }
}
