// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 5
// Exercise 2
// Submission code: HN1DS1

// Includes section
#include "DHT.h"

// Macros section
#define f_BAUD     9600  // Frequency of the serial port
#define DHTPIN     6     // Digital pin of the Temp&Humid sensor
#define LED_COUNT  6     // Number of LEDs in this exercise (5 default, +1
                         // extra)
#define T_INTERVAL 1000  // Reading delay (ms)

#define LED_LOWER_BOUND -1           // The reading is too low  (< min_val)
#define LED_UPPER_BOUND LED_COUNT -1 // The reading is too high (> max_val)

// Function prototypes
int8_t t_map(const int t); // Map temperature value to an ordinal of LED

// Instantiate a DHT instance (for temperature and humidity sensor)
DHT dht(DHTPIN, DHT11);

// Global time buffer, coupled with `millis()`
unsigned long t_buff = 0;

// Global state of the LEDs
uint8_t led_state = 0;

// Digital pins of the LEDs in use
const uint8_t LED[] = { 13, 12, 8, 7, 4, 2 };

/* ==================== Sensor Ranges ==================== */
// Herein, the developer (or user) of the system can specify the
// individual intervals of the temperature readings for the LEDs.
//
// Note: the ranges are inclusive from both sides. The second ranges
// in the `else` block are merely used for testing (with some realistic
// temperature values) and for the presentation. Replace 1 by 0 to use
// these intervals instead.

#if 1
const uint8_t LED1_MIN =  0, // <- Start editing ranges here...
              LED1_MAX = 10,
              LED2_MIN = 11,
			        LED2_MAX = 20,
              LED3_MIN = 21,
			        LED3_MAX = 30,
              LED4_MIN = 31,
              LED4_MAX = 40,
              LED5_MIN = 41,
              LED5_MAX = 50;
#else
const uint8_t LED1_MIN = 22,
              LED1_MAX = 22,
              LED2_MIN = 23,
			        LED2_MAX = 23,
              LED3_MIN = 24,
			        LED3_MAX = 24,
              LED4_MIN = 25,
              LED4_MAX = 25,
              LED5_MIN = 26,
              LED5_MAX = 27;
#endif

/* ==================== Sensor Ranges ==================== */

/**
 * Initialize all components required by the program.
 */
void setup() {
  Serial.begin(f_BAUD); // Serial monitor
  dht.begin();          // Begin DHT sensor   
  
  // Initialize all LEDs
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    pinMode(LED[i], OUTPUT); // Set the mode to `OUTPUT` of each LED
  }
}

/**
 * This function is run continually while the board is turned on. Perform the
 * purpose of the program.
 */
void loop() {
  // Read the current time since the execution of the program elapsed
  unsigned long current_time = millis();

  // Detect if the time interval has passed and proceed with the reading
  if (current_time >= t_buff + T_INTERVAL) {
    t_buff = current_time; // Update the time buffer
    int temperature = dht.readTemperature(); // Read temp value in Celsius

    // Print the value of the temperature (used namely for testing and presentation
    // purposes). This can be disabled by changing 1 to 0.
#if 1
    Serial.print(temperature); Serial.println("C");
#endif

    // Check if any changes are to be carried out with the LEDs; if so, alter the state
    // of the LEDs of the program.
    LED_routine(temperature);
  }
}

/* LED subroutine to change the state of the LEDs */
void LED_routine(const int temp) {
  int8_t led_idx = t_map(temp); // Assign the ordinal that the temp corresponds to

  // Detect a change in the number of LEDs to be on or off. This ensures that writings
  // to the LEDs are not carried out exhaustively and that resources are properly managed.
  if (led_state != led_idx) {
    
    // Detect if the current reading crossed any of the interval bounds,
    // if so, turn on the red LED. Otherwise, turn it off.
    led_idx == LED_LOWER_BOUND || led_idx == LED_UPPER_BOUND
      ? digitalWrite(LED[LED_COUNT - 1], HIGH)
      : digitalWrite(LED[LED_COUNT - 1], LOW);
	
    // Some LEDs are to be dimmed (i.e., turned off)
    if (led_state > led_idx) {
      // Take the difference of led_state and led_idx that determines
      // how many LEDs are to be dimmed. Iterate that many times.
      for (uint8_t i = 0; i < led_state - led_idx; i++) {
        uint8_t idx = led_state - i; // Index of the current LED to dim
        
        // Note: this is a corner case. When led_state is 5 and led_idx is
        // -1, this loop would mistakenly dim the red LED, even though that
        // should not happen, hence the following:
        if (idx == LED_UPPER_BOUND) continue;
        
        // Dim the LED at the current index.
        digitalWrite(LED[idx], LOW);
      }
    // Otherwise, some LEDs are to be turned on.
    } else {
      // Take the difference of led_idx and led_state to determine how many
      // more LEDs are to be turned on. Then, iterate that many times.
      for (uint8_t i = 0; i <= led_idx - led_state; i++) {
        // Light up an LED at the current index
        digitalWrite(LED[led_state + i], HIGH);
      }
    }
  } 
  
  // Assign the global state variable with the current reading index.
  led_state = led_idx;
}

/**
 * A helper function to map a temperature value to an ordinal. This value is
 * then used to light up particular LEDs from the sequence. -1 denotes that the
 * value is too low (red LED is turned on) and 5 denotes that the value is too
 * high (red LED is too turned on).
 *
 * @param t - temperature value (in Celsius).
 * @returns - ordinal value (-1 [LED_LOW_BOUND], 0 through 4 or 5 [LED_UPPER_BOUND])
 */
int8_t t_map(const int t) {
  if      (t <  LED1_MIN)                  return LED_LOWER_BOUND; // too low
  else if (t >= LED1_MIN && t <= LED1_MAX) return 0; // range1 satisfied
  else if (t >= LED2_MIN && t <= LED2_MAX) return 1; // range2 satisfied
  else if (t >= LED3_MIN && t <= LED3_MAX) return 2; // ...
  else if (t >= LED4_MIN && t <= LED4_MAX) return 3;
  else if (t >= LED5_MIN && t <= LED5_MAX) return 4;
  else                                     return LED_UPPER_BOUND; // too high
}
