// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 3
// Exercise 4
// Submission code: <XXXYYY>

const int row1 = 11;	// digital pin for row 1
const int row2 = 10;	// digital pin for row 2
const int row3 = 9;		// digital pin for row 3
const int row4 = 8;		// digital pin for row 4

const int col1 = 7;		// digital pin for column 1
const int col2 = 6; 	// digital pin for column 2
const int col3 = 5; 	// digital pin for column 3
const int col4 = 4; 	// digital pin for column 4

/*
* Setup function runs once at startup
* Within it we configure the pins that connect to the keypad
*/
void setup() {

    pinMode(row1, OUTPUT);	// configure pin representing row as an output pin
    pinMode(row2, OUTPUT);	// as above
    pinMode(row3, OUTPUT);	// as above
    pinMode(row4, OUTPUT);	// as above
  
    pinMode(col1, INPUT); 	// configure pin representing column as input pin
    pinMode(col2, INPUT); 	// as above
    pinMode(col3, INPUT); 	// as above
    pinMode(col4, INPUT); 	// as above
  
    Serial.begin(9600);		// setup serial monitor as output with baud rate of 9600
}

/*
* Main loop runs continuously
* Each loop, we check if a key has been pressed
*/
void loop() {

    // multidimensional array represents keypad
    // values correspond to actual keypad's row/column configuration 
    char keypad[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    // loop through rows
    for (int row = 0; row < 4; row++) {

        // send a signal through current row
        digitalWrite(row1 - row, LOW);

        // loop through columns
        for (int col = 0; col < 4; col++) {

            // read the current column and store signal in keySignal variable
            int keySignal = digitalRead(col1 - col);

            // check if the key is pressed (if keySignal == 0)
            if (keySignal == LOW) {

                // print the corresponding character from the keypad array
                Serial.println(keypad[row][col]);

                // delay for debouncing
                delay(1000);
            }
        }

        // reset the current row
        digitalWrite(row1 - row, HIGH);
    }
}
