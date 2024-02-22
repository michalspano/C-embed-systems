// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 5
// Exercise 3
// Submission code: HN1DS1

// Includes section
#include "DHT.h"

// Macros section
#define f_BAUD    9600 // Frequency of the serial port
#define LIGHT_MAX 1023 // Max raw value of the light intensity sensor

#define GREEN_LED  7 // Digital pin of the green LED
#define RED_LED    4 // Digital pin of the red LED
#define YELLOW_LED 2 // Digital pin of the yellow LED
#define DHTPIN    12 // Pin for the Temp&Humid sensor

// Function prototypes
int l_map(const int l); // Map light value to an ordinal
int t_map(const int t); // Map temperature value to an ordinal

const float sDELAY = 1; // Span of each iteration (in seconds)

DHT dht(DHTPIN, DHT11); // Initialize the DHT sensor

/**
 * Initialize all components, begin a Serial Monitor.
 */
void setup() {
  Serial.begin(f_BAUD);        // Serial monitor
  dht.begin();                 // Begin reading for the DHT sensor
  pinMode(A0, INPUT);          // Read the light intensity
  pinMode(YELLOW_LED, OUTPUT); // Yellow LED
  pinMode(RED_LED, OUTPUT);    // Red LED
  pinMode(GREEN_LED, OUTPUT);  // Green LED
}

/**
 * This function is run continually while the board is turned on. Perform the
 * purpose of the program.
 */
void loop() {
  int temperature     = dht.readTemperature(); // Read the temperature (in Celsius)
  int light_intensity = analogRead(A0);        // Read the raw light intensity

  // Map the obtained raw light intensity value to percentage (i.e., 0-100).
  light_intensity = map(light_intensity, 0, LIGHT_MAX, 0, 100);
  
  // Log the values to the serial monitor. This can be disabled by replacing 1
  // with 0. This is helpful when debugging and showcasing the solutions to the
  // instructors.
#if 1
  Serial.println("==========");
  Serial.print(temperature);     Serial.println(" C");
  Serial.print(light_intensity); Serial.println("%");
#endif
  
  // Obtain the mapped ordinal values based on the current temperature and
  // light intensity readings.
  int m_t = t_map(temperature);     // For temperature
  int m_l = l_map(light_intensity); // For light_intensity
 
  // When the values are within a range of the same degree, the green LED is on.
  if (m_t == m_l) {
    digitalWrite(GREEN_LED, HIGH); // Turn on GREEN_LED
    digitalWrite(YELLOW_LED, LOW); // Turn off YELLOW_LED
    digitalWrite(RED_LED, LOW);    // Turn off RED_LED

  // When the temperature range is of a higher degree than the one of the light
  // intensity, the red LED is on.
  } else if (m_t > m_l) {
    digitalWrite(RED_LED, HIGH);   // Turn on RED_LED
    digitalWrite(GREEN_LED, LOW);  // Turn off GREEN_LED
    digitalWrite(YELLOW_LED, LOW); // Turn off YELLOW_LED
  // Otherwise (light intensity is greater than the temperature), the yellow
  // LED is on.
  } else {
    digitalWrite(YELLOW_LED, HIGH); // Turn on YELLOW_LED
    digitalWrite(GREEN_LED, LOW);   // Turn off GREEN_LED
    digitalWrite(RED_LED, LOW);     // Turn off GREEN_LED
  }
  
  // Delay each iteration of the loop by some milliseconds.
  delay(sDELAY * 1000);
}

/**
 * A helper function to map a temperature value to an ordinal.
 * There's a possibility to make this function less verbose, but the
 * current implementation is more readable.
 * 
 * @param t - temperature value (in Celsius).
 * @returns - ordinal value (0, 1, 2, or 3).
 */ 
int t_map(const int t) {
  if      (t < -12)            return 0;
  else if (t >= -12 && t < 0)  return 1; // Should 0 be included? Not clear.
  else if (t >= 0 && t <= 20)  return 2;
  else                         return 3;
}

/**
 * A helper function to map a light_intensity value to an ordinal.
 * 
 * @param l - light intensity value (as a percentage).
 * @returns - ordinal value (0, 1, 2, or 3).
 */
int l_map(const int l) {
  if      (l == 0)             return 0;
  else if (l >= 1  && l <= 20) return 1;
  else if (l >= 21 && l <= 60) return 2;
  else                         return 3;
}
