// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 6
// Exercise 1, task 1
// Submission code: 389372

// Defines section
#define PWM1 5
#define PWM2 6

#define SPEED_MAX    255 // Maximum speed value
#define SERIAL_PORT 9600 // Serial monitor

/* The setup function of the program */
void setup() {
  Serial.begin(SERIAL_PORT); // Begin Serial Monitor
  pinMode(PWM1,OUTPUT);      // DC motor config 1
  pinMode(PWM2,OUTPUT);      // DC motor config 2
  
  // To avoid a bug with TinkerCad. Replace 1 with 0 to disable the following.
#if 1
  analogWrite(PWM2, 10); delay(1000); analogWrite(PWM1, 10);
#endif
}

/* The loop function of the program */
void loop() {
  // To avoid a bug with TinkerCad. Replace 1 with 0 to disable the following.
#if 1
  analogWrite(PWM1, 10); delay(1000); analogWrite(PWM2, 10);
#endif  

  // Prompt message to enter a speed value
  Serial.println("Enter speed (1-100):");

  // Read the value from the user (3 digits)
  int speed = get_int(3);

  // Remap the value to 0-SPEED_MAX. We desire to make the UX intuitive for the
  // user and they merely input a value that is 1-100 (which is easy to understand).
  speed = map(speed, 1, 100, 0, SPEED_MAX);
 
  // Prompt message to enter a direction
  Serial.println("Enter direction (+/-):");

  // Read one character
  char dir = get_char();

  // If '-' (negative) is passed, invert the `speed` value.
  if (dir == '-') speed = -speed;

  for (;;) { // the same as `while(1)`; do in an indefinite execution block
    if (speed >= 0) { // Clockwise rotation

      // The motor will not react with too little values, hence remap the speed
      // to >= 100.
      if (speed < 100) speed = 100;
      analogWrite(PWM2, 0);     // Write 0 to PWM2
      analogWrite(PWM1, speed); // Write the speed
    } else { // Counter-clockwise rotation
      // The motor will not react with too little values, hence remap the speed
      // to <= -100.
      if (-speed < 100) speed = -100;
      analogWrite(PWM1, 0);      // Write 0 to PWM1
      analogWrite(PWM2, -speed); // Write the speed in negative direction
    }
  }
}

/**
 * A helper function to read a `size` digit integer from the Serial port.
 *
 * @param size - how many digits to read.
 * @returns    - a three digit integer read from the Serial port.
 */
int get_int(const uint8_t size){
  char buf[size]; // To store the raw value
  int ready =  0, // To indicate the state
      input = -1; // To store the parsed value

  // While the user is typing the value, block the exeuction
  while (!ready) {
    ready = Serial.readBytes(buf, size); // Read 'size' chars
    input = atoi(&buf[0]);               // Attempt to parse as an int
  }

  // Return the obtained value from the user.
  return input;
}

/**
 * A helper functon to read a single character from the Serial port.
 *
 * @returns - the read character value.
 */
char get_char(){
  char buf;      // Store the character
  int ready = 0; // Indicate the state of reading
  
  // While the user is typing, block the execution and read one Byte (i.e., 1
  // char).
  while (!ready) ready = Serial.readBytes((char*)&buf, 1);
  
  // Return the read value
  return buf;
}
