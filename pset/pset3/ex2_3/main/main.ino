// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 3
// Exercise 2, 3
// Submission code: 0173549
//
// Note: the following source code contains implementation for both the 2nd and
// the 3rd exercises. For the 2nd exercise, the `DHT.h` library was not used, so
// the solution is applicable to the 3rd exercise as well. The circuits remain the
// same, merely a multimeter is added to display the voltage (in exercise 3).

// Macros section
#define f_BAUD    9600 // Frequency of the serial port
#define LIGHT_MAX 67.0 // Max raw value of the light intensity sensor
#define BLUE_LED  2    // Pin of blue LED
#define RED_LED   4    // Pin of red LED
#define GREEN_LED 7    // Pin of green LED

// Function prototypes
int l_map(const int l);         // Map light value to an ordinal
int t_map(const int t);         // Map temperature value to an ordinal
float parse_temp(const int r);  // Parse raw temperature
float parse_light(const int r); // Parse raw light intensity

const float sDELAY = 1; // Span of each iteration (in seconds)

/**
 * Initialize all components, begin a serial monitor at 9600.
 */
void setup() {
  Serial.begin(f_BAUD);       // Serial monitor
  pinMode(A0, INPUT);         // Temperature sensor
  pinMode(A1, INPUT);         // Light sensor
  pinMode(BLUE_LED, OUTPUT);  // Blue LED
  pinMode(RED_LED, OUTPUT);   // Red LED
  pinMode(GREEN_LED, OUTPUT); // Green LED
}

/**
 * This function is run continually while the board is turned on. Perform the
 * purpose of the program.
 */
void loop() {
  int raw_temp  = analogRead(A0); // Read the raw temperature value.
  int raw_light = analogRead(A1); // Read the raw light intensity value.
  
  // Parse the raw temperature and light intensity values to the conventional
  // units (Celsius and percentage, respectively). Observe that the values are
  // cast to integers, as we are not interested in the decimal part.
  int temperature     = (int) parse_temp(raw_temp);
  int light_intensity = (int) parse_light(raw_light);
  
  // Log the values to the serial monitor
  Serial.print(temperature); Serial.println(" C");
  Serial.print(light_intensity); Serial.println("%");
  
  // Obtain the mapped ordinal values based on the current temperature and
  // light intensity values.
  int m_t = t_map(temperature);
  int m_l = l_map(light_intensity);
  
  // When the values are within a range of the same degree, the green LED is on.
  if (m_t == m_l) {
    digitalWrite(GREEN_LED, HIGH); // Turn on GREEN_LED
    digitalWrite(RED_LED, LOW);    // Turn off RED_LED
    digitalWrite(BLUE_LED, LOW);   // Turn off BLUE_LED
  // When the temperature range is of a higher degree than the one of the light
  // intensity, the red LED is on.
  } else if (m_t > m_l) {
    digitalWrite(RED_LED, HIGH);  // Turn on RED_LED
    digitalWrite(GREEN_LED, LOW); // Turn off GREEN_LED
    digitalWrite(BLUE_LED, LOW);  // Turn off BLUE_LED
  // Otherwise, the blue LED is on.
  } else {
    digitalWrite(BLUE_LED, HIGH); // Turn on BLUE_LED
    digitalWrite(GREEN_LED, LOW); // Turn off GREEN_LED
    digitalWrite(RED_LED, LOW);   // Turn off RED_LED
  }
  
  // Delay each iteration of the loop by some milliseconds.
  delay(sDELAY * 1000);
}

/**
 * A helper function to compute temperature value (in Celsius) from an analog
 * reading. The function is made more generic, such that it returns a float
 * even though we operate with integers in this exercise. This is to promote
 * future extensibility of the function (the same applies to `parse_light`).
 *
 * The following solution is adapted from the linked article.
 * @see https://learn.adafruit.com/tmp36-temperature-sensor/using-a-temp-sensor
 *
 * @param r - current raw value (i.e., reading) from the analog port.
 * @returns - temperature value in Celsius (as a float).
 */
float parse_temp(const int r) {
  // Convert the value to voltages (V).
  float voltage = (r * 5.0195f) / 1024.0f;

  // Replace 0 by 1 to allow the following line of code (to print the obtained value
  // in Voltages. This is helpful when debugging or demonstrating that the solution works.
#if 0
  // Print the parsed V value.
  Serial.print(voltage); Serial.println("V");
#endif

  // Include 500 mV (0.5V) offset, convert to temperature in Celsius.
  return (voltage - 0.5) * 100;
}

/**
 * A helper function to compute relative light intensity value based on an
 * analog reading.
 *
 * @param r - current raw value (i.e., reading) from the analog port.
 * @returns - light intensity percentage value (as a float).
 */
float parse_light(const int r) {
  // The sensor in use produces a max value of 67. Hence, we map each reading
  // as a relative percentage of the maximum value. 
  return (r / LIGHT_MAX) * 100.0f;
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
 * Note: we assume that l_map is used with `parse_light` function, so
 * that the input value is a percentage (i.e., in the range [0, 100]).
 * There's a possibility to make this function less verbose, but the
 * current implementation is more readable.
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
