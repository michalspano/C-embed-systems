// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 3
// Exercise 1
// Submission code: <XXXYYY>

const uint8_t PIN_OUT[]	 = { 4, 7 }; // Declare input pins
const uint8_t PIN_IN[]   = { 2 };    // Declare output pin
const uint8_t d_interval = 500;      // Declare an interval delay
int button_state         = 0;        // Store the state of the push button

/* Set up the individual pins */
void setup() {
  pinMode(PIN_IN[0],  INPUT);  // Push button at digital pin 2. 
  pinMode(PIN_OUT[0], OUTPUT); // LED at pin 4.
  pinMode(PIN_OUT[1], OUTPUT); // LED at pin 7.
}

/* Program's loop body */
void loop() {
  // Note: this blocks the button reading. Preferably, it should be extracted
  // to a stand-alone construct. Solution: use non-blocking millis().
  // Source: https://arduinogetstarted.com/faq/how-to-use-millis-instead-of-delay.
  // FIXME: use millis() instead of delay().

  digitalWrite(PIN_OUT[1], HIGH); // Light up LED
  delay(d_interval);              // Wait 0.5s
  digitalWrite(PIN_OUT[1], LOW);  // Dim the LED
  delay(d_interval);              // Wait 0.5s
 
  // Read the state of the button at digital port 2.
  button_state = digitalRead(PIN_IN[0]);

  // If the button is pressed, light up the LED. Otherwise, dim the LED if the
  // button is not pressed.
  button_state == HIGH ? digitalWrite(PIN_OUT[0], HIGH)
                       : digitalWrite(PIN_OUT[0], LOW);
  delay(10); // Add some small delay
}
