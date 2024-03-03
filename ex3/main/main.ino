// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 6
// Exercise 3
// Submission code: 389372

// Includes section
#include <Keypad.h>
#include <Adafruit_NeoPixel.h>

// Defines section
#define RING_PIN      2 // Digital pin of the input of NeoPixel
#define ADDRESS_LEN   7 // Number of digits in an address
#define ADDRESS_COUNT 8 // # of valid addresses
#define RING_LED_NUM 24 // Number of pixels (i.e., small LEDs) on the ring

#define SERIAL_PORT 9600 // Serial monitor port

// A custom type to represent a color (RGB). Perhaps expand this for each unit to be float?
struct COLOR { uint8_t r, g, b; };

/* ============================ CONFIG ====================================== */

// Replace the `#if 0` with `#if 1` to enable the advanced animation. Otherwise,
// the basic animation will be used.
#if 1
  #define ANIMATION_ADVANCED
#endif

// A color to use when highlighting the LEDs that represent the digits. This is an
// easily customizable 'interface' and the values can be adjusted here:
const struct COLOR DIGIT_LED_COL = (struct COLOR) {
  100, // <-- Red
  155, // <-- Green
  120  // <-- Blue
};

/* ============================ CONFIG ====================================== */

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

// Initialize a NeoPixel Ring. Populate the constructor with the following
// attributes:
Adafruit_NeoPixel ring(
  RING_LED_NUM,        // 24 LEDs variant (that is in use in this exercise)
  RING_PIN,            // at pin 2
  NEO_RGB + NEO_KHZ800 // RGB mode with a correct frequency
);

// An Array containing the valid addresses for this program
const char ADDRESS[ADDRESS_COUNT][ADDRESS_LEN + 1] = {
  "1AC91BD", "342824A",
  "6D1732B", "52A64AC",
  "413553A", "34246CC",
  "23B393B", "9999999"
};

// A buffer that stores which LEDs have been 'locked in' accompanied with a cumulatively
// incremented index. This is used to keep track of the LEDs that represent the digits.
uint8_t l_idx = 0,
        led_agenda[ADDRESS_LEN * 2];

/* Set up the components of the system */
void setup() {
  Serial.begin(SERIAL_PORT); // Begin Serial Monitor

  ring.begin();              // Begin NeoPixel Ring
  ring.setBrightness(15);    // Set the brightness to some sensible value
}

/* The main body of the program */
void loop() {
  Serial.println("=~= StarGate =~=");

  // Read the address from the Keypad (call the helper function)
  char* address = read_address();

  // Start dialing...
  Serial.println("=~============~=");
  Serial.println("Dialing...");

  // Proceed with the dialing, call the dial wrapper to "lock in" the first 6
  // LEDs. This functionality has been factored out to make the code more
  // readable and maintainable.
  uint8_t i = 0;
  while (i < 6) { dial_wrapper(i); i++; }

  // Decision based on the validity of the address: if the address is valid,
  // lock in the last LED and block the sequence. Otherwise, show an error
  // sequence indefinitely.
  Serial.print("\nStatus message\n> "); // Message placeholder
  if (!is_valid_address(address)) {     // Invalid address
    Serial.println("ERROR");            // Error message
    free(address);                      // Free the address from the heap
    for (;;) error();                   // Show the error sequence indefinitely
  } else {                              // Valid address
    Serial.println("OK");               // Success message
    free(address);                      // Free the address from the heap
    dial_wrapper(6);                    // Dial the last LED digit
    for (;;);                           // Block the sequence indefinitely
  }

  // Note: the `for (;;)` is used to block the sequence indefinitely. This is
  // done to prevent the program from exiting the loop and to keep the LEDs
  // turned on. This behavior may be adjusted based on the requirements, albeit
  // is left as is for the sake of simplicity.
}

/**
 * A helper function that maps a digit index (0-6) to respective LED ring
 * indices (0-23). The event of assignment is rather sporadic and the values
 * are not consecutive, hence the following switch statement.
 *
 * Note: the index is 0-based.
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
    default:       -1; // Invalid event
  }
}

/**
 * A wrapper routine around the `dial` routine. It calls the `dial` routine,
 * remaps the index value, and updates the 'agenda' (of LEDs) with two new
 * LEDs. Ultimately, the changes are reflected using the `toggle_digit_LEDs`
 * routine. To make this change visible to the user, the process is delay
 * by 1.5 seconds.
 *
 * @param idx - current NeoPixel's LED index value
 */
void dial_wrapper(uint8_t idx) {
  // Start to dial, obtain the mapped index based on the given value.
  dial(); idx = map_idx(idx);

  // First LED from the subgroup
  led_agenda[l_idx++] = idx;

  // Second LED from the subgroup. Ensure the value is rebound to the start
  // if the maximum value is reached.
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
  // Traverse the agenda and turn on/off the LEDs
  for (uint8_t i = 0; i < l_idx; i++) {
    to_turn_on ? ring.setPixelColor(led_agenda[i],
                                    DIGIT_LED_COL.g,
                                    DIGIT_LED_COL.r,
                                    DIGIT_LED_COL.b)
               : ring.setPixelColor(led_agenda[i], 0, 0, 0);
    ring.show(); // Show the changes immediately
  }
}

/**
 * The `dial` subroutine is responsible for turning on the LEDs in a sequence
 * and dimming the trailing LED. The sequence is delayed by 0.125s to make the
 * changes visible to the user.
 */
void dial() {
  // All digit LEDs are to be turned off as the initial state
  toggle_digit_LEDs(false);

  // Iterate over all pixel LEDs
  for (uint8_t i = 0; i < RING_LED_NUM; i++) {
    // Dim the trailing LED (ignore for the first iteration)
    if (i != 0) ring.setPixelColor(i - 1, 0, 0, 0);

// The following conditional block is used to determine which animation to use.
#if defined(ANIMATION_ADVANCED)
    struct COLOR c = map_to_color(i);
#else
    struct COLOR c = random_col();
#endif

    // Turn on the current pixel LED, show the changes and wait for 0.125s
    ring.setPixelColor(i, c.g, c.r, c.b); // Uses GRB notion (not RGB)
    ring.show(); delay(125);
  }

  // Dim the last trailing LED pixel
  ring.setPixelColor(RING_LED_NUM - 1, 0, 0, 0);
}

/**
 * A subroutine to indicate an error by flashing the ring with red color.
 * This is to indicate an invalid address.
 */
void error() {
  // Turn on all LEDs to RED
  for (uint8_t i = 0; i < RING_LED_NUM; i++) {
    ring.setPixelColor(i, 0, 255, 0); // RED color at i-th position
  }

  // Show the changes and wait for 0.5s
  ring.show(); delay(500);

  // Dim all LEDs
  for (uint8_t i = 0; i < RING_LED_NUM; i++) {
    ring.setPixelColor(i, 0, 0, 0); // 0, 0, 0 (rgb val.)
  }

  // Show the changes and wait for 0.5s
  ring.show(); delay(500);
}

/* The 'simple' animation routine: a random color is generated for each LED. */
struct COLOR random_col() {
  struct COLOR c = (struct COLOR) {
    rand() % 255 + 1, // 1-255
    rand() % 255 + 1, // 1-255
    rand() % 255 + 1  // 1-255
  };

  // Return the random color
  return c;
}

/**
 * The 'advanced' animation routine: the circle is split to 3 segments and each
 * segment is assigned shades of a color (in a gradient manner). The color is
 * then mapped to the LED index.
 *
 * @param idx - index of the LED
 */
struct COLOR map_to_color(uint8_t idx) {
  // Start with a black color
  struct COLOR c = (struct COLOR) { 0, 0, 0 };

  // Calculate the color based on the index. There's 24 LEDs and 3 segments
  // (8 LEDs per segment). The formula is basically:
  // ```
  // color = (i + 1) * (255 / 8)
  // ```
  // where `i` is the segment index (0-7). 255/8 implies the gradient
  // increment.

  uint8_t color = ((idx % 8) + 1) * (255.0 / 8);

  // Switch based on the segment index
  switch (idx / 8) {
    case 0: c.r = color; break; // Segment 1 (red)
    case 1: c.g = color; break; // Segment 2 (green)
    case 2: c.b = color; break; // Segment 3 (blue)
  }

  // Return the color
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
 * Read a 7-digit address from the Keypad from the user.
 *
 * @returns - the gathered seven digits.
 */
char* read_address() {
  // Dynamically allocate 8 chars (7 + null terminator)
  char *address = (char*) malloc(sizeof(char) * ADDRESS_LEN + 1);

  // Keep a pointer to the start of the address
  char *pAddress = address;

  char key;      // Store the current key
  uint8_t i = 0; // A local index

  while (i < ADDRESS_LEN) { // Attempt to read 7 keys
    key = keypad.getKey();  // Attempt to get the key
    if (key) {              // If a key is detected
      *address++ = key;     // Register the key in the digit
      print_key(key, ++i);  // Print the obtained key (and increment i)
    }
    delay(100);             // Delay for the ease of usability
  }

  // Add the null terminator to the end of the address
  *address = '\0';

  // Return the pointer of the char sequence
  return pAddress;
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

