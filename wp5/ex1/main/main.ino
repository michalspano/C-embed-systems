// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 5
// Exercise 1
// Submission code: HN1DS1

uint8_t button_is_on    = 0,        // Store the state of the push button (on/off)
        LED_is_on       = 0;        // Store the state of the LED (on/off)
const uint8_t PIN_IN[]  = { 2 };    // Declare output pin
const uint8_t PIN_OUT[] = { 4, 7 }; // Declare input pins
unsigned long t_buff    = 0;        // Time indicator buffer (used with `millis`)

/* Set up the individual pins */
void setup() {
  pinMode(PIN_IN[0],  INPUT);  // Push button at digital pin 2.
  pinMode(PIN_OUT[0], OUTPUT); // LED at pin 4.
  pinMode(PIN_OUT[1], OUTPUT); // LED at pin 7.
}

/* Program's loop body */
void loop() {
  // During each iteration of the loop, get the current elapsed time of the
  // program. This implementation uses the non-blocking `millis()` function.
  unsigned long current_time = millis();

  // The following code block will be executed each 1 second.
  // Note: the interval span is 1000 ms, because `millis()` returns the time in
  // milliseconds.
  if (current_time > t_buff + 1000) {
    t_buff = current_time;  // Assign the current time to the buffer.
    LED_is_on = !LED_is_on; // Negate the state of the LED (is should
                            // alternate every second).

    // Based on the state of the LED, either turn it on or off.
    LED_is_on ? digitalWrite(PIN_OUT[1], LOW)    // Dim the LED
              : digitalWrite(PIN_OUT[1], HIGH);  // Light up the LED
  }

  // The following code segment is executed separately, because `millis()` is
  // non-blocking.

  // Read the state of the button at digital port 2.
  button_is_on = digitalRead(PIN_IN[0]);

  // If the button is pressed, light up the LED. Otherwise, dim the LED if the
  // button is not pressed.
  button_is_on == HIGH ? digitalWrite(PIN_OUT[0], HIGH)
                       : digitalWrite(PIN_OUT[0], LOW);

  // Add some small delay. This value is minimal, so it is acceptable that the blocking
  // `delay` function is used.
  delay(10);
}

