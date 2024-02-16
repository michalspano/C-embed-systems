#include <Adafruit_NeoPixel.h>

// Macros section
#define f_BAUD    9600  // Serial monitor BAUD rate
#define RED_LED   2     // Digital pin 2
#define LIGHT_MAX 67    // Maximum value of the light sensor
#define RING_PIN     4  // Digital pin 4
#define RING_LED_NUM 16 // Number of pixels (i.e., small LEDs) on the ring

// Initialize a NeoPixel Ring
Adafruit_NeoPixel ring(
  RING_LED_NUM,        // 16 LEDs variant
  RING_PIN,            // at pin 4
  NEO_RGB + NEO_KHZ800 // RGB mode with a correct frequency
);

// The RGB color that is to be used for each LED in the pixel ring.
const uint8_t RGB[3] = {
  220, // <- Red
  100, // <- Green
  150  // <- Blue
};

const float sDELAY = 0.5; // Span of each iteration (in seconds)

// Global variable state for the pixel ring (i.e., how many LEDs
// are turned ON).
// Note: the `setPixelColor` is 0 indexed. However, this variable
// stores values 1 through 16 (so, using 0 in its declaration is
// a "safe" option.
uint8_t ring_state = 0;

/**
 * Initialize all components, begin a serial monitor at 9600.
 */
void setup() {
  Serial.begin(f_BAUD);     // Serial monitor
  pinMode(A0, INPUT);       // Light sensor
  pinMode(RED_LED, OUTPUT); // Red LED
  ring.begin();             // Start NeoPixel Ring              
}

/**
 * This function is run continually while the board is turned on. Perform the
 * purpose of the program.
 */
void loop() {
  // Read the raw reading of the light sensor
  int light_intensity = analogRead(A0);
  
  // Obtain the "actual" light intensity (as a percentage)
  light_intensity     = map(light_intensity,
                            0, LIGHT_MAX,
                            0, 100);
	
  // Make the light intensity to an ordinal to determine how many LEDs of the ring
  // are to be turned on.
  uint8_t r_idx = map(light_intensity,
                      0, 100,
                      1, RING_LED_NUM + 1);
  
  // Update the pixel ring iff a change in the LEDs is to be carried out.
  // This enhances the performance of the program to a great extent.
  if (r_idx != ring_state) {
    // If the red LED is to be turned on (i.e., all LEDs from the ring are on).
    bool is_signal_max = r_idx == RING_LED_NUM + 1;
    is_signal_max ? digitalWrite(RED_LED, HIGH) // Turn on the LED
                  : digitalWrite(RED_LED, LOW); // Dim the LED
      
    // More LEDs are to be added
    if (r_idx > ring_state) {
      
      // The current reading indicated that more LEDs are to be turned on.
      // Detect how many and iterate that many times.
      for (int i = 0; i < r_idx - ring_state; i++) {
        
        // Light up the LED with the RGB values at the current position
        ring.setPixelColor(ring_state + i,
                           RGB[0], RGB[1], RGB[2]);
        ring.show(); // Show the update in the ring immediately
      }
    // Less LEDs are to be shown
    } else {
      // The difference between the current state and the mapped value represents
      // how many LEDs are to be turned off.
      for (int i = 1; i <= ring_state - r_idx; i++) {
        ring.setPixelColor(ring_state - i, 0, 0, 0); // Dim LED at the current position
        ring.show(); // Show the update in the ring immediately
      }
    }
  }
  
  // Update the state of the pixel ring with the current value
  ring_state = r_idx;
  
  // Delay each iteration of the loop by some milliseconds
  delay(sDELAY * 1000);
}
