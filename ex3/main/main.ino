// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 6
// Exercise 3

// TODO: finish commenting this code

#include <Keypad.h>
#include <Adafruit_NeoPixel.h>

#define RING_PIN      2 // Digital pin of the input of NeoPixel
#define ADDRESS_LEN   7 // Number of digits in an address
#define ADDRESS_COUNT 8 // # of valid addresses
#define RING_LED_NUM 24 // Number of pixels (i.e., small LEDs) on the ring

#define SERIAL_PORT 9600

const byte ROW_COUNT = 4; // # of rows of the keypad
const byte COL_COUNT = 4; // # of columns of the keypad

// Pins that hold each individual row of the keypad
byte PIN_ROWS[ROW_COUNT] = { 11, 10, 9, 8 };

// Pins that hold each individual columns of the keypad
byte PIN_COLS[COL_COUNT] = { 7,  6,  5, 4 };

// The custom layout of the keypad that is used (based on the instructions of
// the assignment).
const char KEYS[ROW_COUNT][COL_COUNT] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

// Make a new instance of `Keypad`. Populate the constructor with the required
// attributes.
Keypad keypad = Keypad(
  makeKeymap(KEYS),    // custom key layout
  PIN_ROWS, PIN_COLS,  // pins for rows, cols
  ROW_COUNT, COL_COUNT // #rows, #cols
);

// Initialize a NeoPixel Ring
Adafruit_NeoPixel ring(
  RING_LED_NUM,        // 24 LEDs variant (that is in use in this exercise)
  RING_PIN,            // at pin 2
  NEO_RGB + NEO_KHZ800 // RGB mode with a correct frequency
);

const char ADDRESS[ADDRESS_COUNT][ADDRESS_LEN + 1] = {
  "1AC91BD",
  "342824A",
  "6D1732B",
  "52A64AC",
  "413553A",
  "34246CC",
  "23B393B",
  "9999999"
};

struct COLOR {
  uint8_t g;
  uint8_t r;
  uint8_t b;
};

const struct COLOR DIGIT_LED_COL = (struct COLOR) {
  155, // Green
  100, // Red
  120  // Blue
};

uint8_t l_idx = 0,
        led_agenda[ADDRESS_LEN * 2];

/* Set up the components of the system */
void setup() {
  Serial.begin(SERIAL_PORT); // Begin Serial Monitor

  ring.begin();              // Begin NeoPixel Ring
  ring.setBrightness(15);    // Set the brightness to some sensible value
}


void loop() {
  Serial.println("=~= StarGate =~=");

  // FIXME: Extract to function
  char address[ADDRESS_LEN + 1]; // +1 for null terminator
  uint8_t char_count = 0;
  char key;

  while (char_count < ADDRESS_LEN) {
    key = keypad.getKey();
    if (key) {
      address[char_count++] = key;
      print_key(key, char_count);
    }
    delay(100);
  }

  // Null terminate the character
  address[char_count] = '\0';

  // Start dialing...
  Serial.println("=~============~=");
  Serial.println("Dialing...");
  
  int i = 0;
  while (i < 6) { dial_wrapper(i); i++; }

  Serial.print("\nStatus message\n> ");
  if (!is_valid_address(address)) {
    Serial.println("ERROR");
    for (;;) error(); // Show the error sequence indefinitely
  } else {
    Serial.println("OK");
    dial_wrapper(6);
    for (;;); // Block the sequence
  }
}

/**
 * A helper function that maps a digit index (0-6) to respective LED ring
 * indices (0-23). The event of assignment is rather sporadic and the values
 * are not consecutive, hence the following switch statement.
 * 
 * @param idx - index digit
 * @returns   - LED ring index (-1 for invalid)
 */
uint8_t map_idx(uint8_t idx) {
  switch (idx) { // Switch based on the digit index
    case 0: return 20;
    case 1: return 23;
    case 2: return  2;
    case 3: return  8;
    case 4: return 11;
    case 5: return 14;
    case 6: return 17;
    defualt:       -1; // Invalid event
  }
}

/**
 * A wrapper routine around the `dial` routine. It calls the `dial` routine,
 * remaps the index value, and updates the 'agenda' (of LEDs) with two new
 * LEDs. Ultimately, the changes are reflected using the `toggle_digit_LEDs`
 * routine. To make this change visible to the user, the process is delay
 * by 1.5 seconds.
 */
void dial_wrapper(uint8_t idx) {
  dial(); idx = map_idx(idx);
  
  // 1. digit
  led_agenda[l_idx++] = idx;
  
  // 2. digit
  if (idx >= RING_LED_NUM-1) idx = -1;
  led_agenda[l_idx++] = ++idx;
  
  // Show changes, delay the execution by 1.5s
  toggle_digit_LEDs(true); delay(1500);
}

/**
 * Subroutine to turn on/off all LEDs that represent individual digits of the
 * address.
 * @param to_turn_on - boolean flag to indicate whether all are turned off or on
 */
void toggle_digit_LEDs(bool to_turn_on) {
  for (uint8_t i = 0; i < l_idx; i++) {
    to_turn_on ? ring.setPixelColor(led_agenda[i],
                                    DIGIT_LED_COL.g,
                                    DIGIT_LED_COL.r,
                                    DIGIT_LED_COL.b)
               : ring.setPixelColor(led_agenda[i], 0, 0, 0);
    ring.show();
  }
}

// This takes 0.125s * 24 = 3s to complete a circle
void dial() {
  // All digit LEDs are to be turned off.
  toggle_digit_LEDs(false);

  // Iterate over all pixel LEDs
  for (uint8_t i = 0; i < RING_LED_NUM; i++) {
    // Dim the trailing LED (ignore for the first iteration)
    if (i != 0) ring.setPixelColor(i - 1, 0, 0, 0);
    
    // FIXME: Make this config part of the macros/ config
    #if 1
    struct COLOR c = map_to_color(i);
    #else
    struct COLOR c = random_col();
    #endif

    // Turn on the current pixel LED, show the changes and wait for 0.125s
    ring.setPixelColor(i, c.r, c.g, c.b);
    ring.show(); delay(125);
  }

  ring.setPixelColor(RING_LED_NUM - 1, 0, 0, 0);
}

/**
 * A subroutine to indicate an error by flashing the ring with red color.
 * This is to indicate an invalid address.
 */
void error() {
  // Turn on all LEDs to RED
  for (uint8_t i = 0; i < RING_LED_NUM; i++) {
    ring.setPixelColor(i, 0, 255, 0);
  }

  // Show the changes and wait for 0.5s
  ring.show(); delay(500);

  // Dim all LEDs 
  for (uint8_t i = 0; i < RING_LED_NUM; i++) {
    ring.setPixelColor(i, 0, 0, 0);
  }

  // Show the changes and wait for 0.5s
  ring.show(); delay(500);
}

// TODO: document this
struct COLOR map_to_color(uint8_t idx) {
  struct COLOR c = (struct COLOR) { 0, 0, 0 };
  int i = idx / 8; // 24 / 3 (three channels)  
  float color = ((idx % 8) +1) * (255.0 / 8);

  switch (i) {
    case 0: c.g = color; break;
    case 1: c.r = color; break;
    case 2: c.b = color; break;
  }
  
  return c;
}

// TODO: document this
struct COLOR random_col() {
  struct COLOR c = (struct COLOR) {
    rand() % 255,
    rand() % 255,
    rand() % 255
  };
  
  return c;
}

/**
 * Subroutine to print the following:
 * ```
 * Pressed: "X" [Y more]
 * ```
 * X is the pressed key and Y is the number of more digits to type
 *
 * @param key   - pressed key
 * @param count - number of more digits to type
 */
void print_key(char key, uint8_t count) {
  Serial.print("Pressed: ");
  Serial.print("\""); Serial.print(key); Serial.print("\" [");
  Serial.print(ADDRESS_LEN - count); Serial.println(" left]");
}

/**
 * Helper function to verify if an address is a valid address.
 *
 * @param address - address to be verified
 * @returns       - boolean value
 */
bool is_valid_address(char* address) {
  // Iterate over all possible addresses
  for (uint8_t i = 0; i < ADDRESS_COUNT; i++) {
    // If the address matches, return true
    if ((strcmp(address, ADDRESS[i])) == 0) return true;
  }

  // No such address exists, return false
  return false;
}

