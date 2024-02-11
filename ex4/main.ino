// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 3
// Exercise 4
// Submission code: <XXXYYY>

// define section
#define NUM_ROW  4            // define number of rows on keypad
#define NUM_COL  4            // define number of columns on keypad
#define SERIAL_BAUD_RATE 9600 // define baud rate for serial monitor
#define DEBOUNCE_LIMIT 200    // define time (ms) required between key presses to register input 
                              // (used to debounce unwanted repeat inputs)

// variable declarations
const int row1 = 11;	      // digital pin for row 1
const int col1 = 7;		      // digital pin for column 1

unsigned long currentTime;    // variable tracking current time (to be compared against previous input time)
unsigned long prevInputTime;  // variable tracking the previous time an input was registered 
                              // (used to debounce unwanted repeat inputs)

// multidimensional array represents keypad
// values correspond to actual keypad's row/column configuration 
const char keypad[4][4] = {
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
    for(int row = 0; row <NUM_ROW; row++) {

        // configure pin representing current row as output pin
        pinMode(row1 - row, OUTPUT);
    }

    // loop through columns
    for(int col = 0; col < NUM_COL; col++) {

        // configure pin representing columns as input pin
        pinMode(col1 - col, INPUT);
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
    for (int row = 0; row < NUM_ROW; row++) {

        // send a signal through current row
        digitalWrite(row1 - row, LOW);

        // loop through columns
        for (int col = 0; col < NUM_COL; col++) {

            // read the current column and store signal in keySignal variable
            int keySignal = digitalRead(col1 - col);

            // track current time 
            currentTime = millis();

            // check if the key is pressed (if keySignal == 0)
            // check also that current time is not within certain ms of last registered input
            // this ensures we ignore unwanted input noise / bouncing from physical key presses
            if (keySignal == LOW && currentTime - prevInputTime > DEBOUNCE_LIMIT) {

                // print the corresponding character from the keypad array
                Serial.println(keypad[row][col]);

                // track this registered input time to compare with future inputs
                prevInputTime = currentTime;
            }
        }

        // reset the current row
        digitalWrite(row1 - row, HIGH);
    }
}
