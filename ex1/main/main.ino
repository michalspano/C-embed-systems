// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 6
// Exercise 1, tasks 2, 3
// Submission code: <XXXYYY>

// Defines section
// DC motor pins
#define ENCA 2
#define ENCB 3
#define PWM1 5
#define PWM2 6

#define POS_MAX        2299 // Position value (maximum)
#define SERIAL_PORT    9600 // Serial monitor
#define FULL_CIRCLE_MAX 359 // 0-359, 360 == 0.

int pos = 0, // Position in ticks
    deg = 0; // Position in degrees

int degtarget = 0, // Target position in degrees
    speed     = 0; // Desired motor speed

// Note: $K_{p}$ has been tweaked with different values, albeit =4 seems to provide
// the 'best' result.
int kp    = 4; // Proportional constant for controller (tuning parameter)
int u_out = 0; // Output of controller

int e = 0, // Globally used error value
    a = 0, // A-encoder signal
    b = 0; // A-encoder signal

/* The setup function of the program */
void setup() {
  Serial.begin(SERIAL_PORT);  // Begin Serial Monitor
  pinMode(ENCA,INPUT_PULLUP); // Channel A
  pinMode(ENCB,INPUT_PULLUP); // Channel B
  pinMode(PWM1,OUTPUT);       // I'm actually not sure what this configures.
  pinMode(PWM2,OUTPUT);       // I'm actually not sure what this configures (yet again).

  // Attach the interrupt; note: it should be sufficient to attache the
  // interrupt to only one of the encoder ports. This part is related to task 2
  // of this exercise.
  attachInterrupt(
                  digitalPinToInterrupt(ENCB), // Digital type at `ENCB`
                  ISR_readEncoder,             // Use the ISR
                  CHANGE                       // On event `CHANGE`
  );
  
  // Avoid TinkerCad bug. Replace 1 with 0 to disable the following.
#if 1
  analogWrite(PWM2, 10); delay(1000); analogWrite(PWM1, 10);
#endif
}

/* The loop function of the program */
void loop() {
  // Avoid TinkerCad bug. Replace 1 with 0 to disable the following.
#if 1
  analogWrite(PWM2, 10); delay(1000); analogWrite(PWM1, 10);
#endif
  
  // Check if motor rotated all the way around and reset the state
  if (pos > POS_MAX) {  // For any spins greater than one full circle
    deg = deg - FULL_CIRCLE_MAX;
    pos = pos - POS_MAX;
  } else if (pos < 0) { // For any spins lower than one full circle
    deg =  FULL_CIRCLE_MAX + deg;
    pos = POS_MAX + pos;
  }

  // Log the current position
  Serial.print("The current position is: "); Serial.println(deg);
   
  // Get input from the user (from the Serial port)
  degtarget = get_int();
  
  // Calculate (initial) error
  // $\to e = pos_{is} - pos_{desired}$
  e = degtarget - deg;

  // Loop until error is zero
  while (e != 0) {
    
    // Map current position into degrees
    deg = map(pos, 0, POS_MAX, 0, FULL_CIRCLE_MAX);

    // Log the current value of `deg` and output the error value onto the
    // Serial Monitor. These are disabled by default, because they seem to obfuscate
    // the Serial Monitor output and the motor's behavior. Replace 0 with 1 to enable.
#if 0
    Serial.print("Current error: "); Serial.println(e);
    Serial.print("The current angle (deg) is: "); Serial.println(deg);
#endif
       
  	// Get necessary speed signal
  	speed = getAction(e);
    
  	// Send speed signal to motor (clockwise rotation)
    if (speed >= 0) {
      // The motor does not react with too low inputs
    	if (speed < 100) speed = 100;

      // Write to the motor
      analogWrite(PWM1, speed); // Write `speed` to PWM1
      analogWrite(PWM2, 0);     // Write 0 to PWM2
  	} else { // Counter-clockwise rotation
      // The motor does not react with too low inputs
    	if (-speed < 100) speed = -100; 

      // Write to the motor
      analogWrite(PWM1, 0);      // Write 0 to PWM1
      analogWrite(PWM2, -speed); // Write the negated `speed` value to PWM2
    }

    // Calculate the new error and assign it to the global variable.
    // Reapply: $\to e = pos_{is} - pos_{desired}$.
  	e = degtarget - deg;
  }
}

/**
 * A helper function to read a three digit integer from the Serial port.
 *
 * @returns - a three digit integer read from the Serial port.
 */
int get_int() {
  char buf[3];    // To store the raw value
  int ready =  0, // To indicate the state
      input = -1; // To store the parsed value
  
  // Prompt the user via the Serial Monitor
  Serial.println("Please enter the desired position:");
 
  // While the user is typing the value, block the execution
  while (!ready) {
    ready = Serial.readBytes(buf, 3); // Read 3 chars
    input = atoi(&buf[0]);            // Attempt to parse as an int
  }

  // Return the obtained value from the user.
  return input;
}  

/**
 * Helper function to drive the motor to a desired position (in degrees).
 * This function return the control signal $u_{out}$ that is provided to the motor.
 * Part of task 3 of Exercise 1.
 */
int getAction(int error) {
  // Calculate u_out as function of the error and the kp (tuning parameter).
  // This relation is described in Task 3: A Position Controller.
  u_out = kp * e;

  // Note: `u_out` cannot be more than 254 or less than -254.
  // Otherwise, return the value. 
  if      (u_out >  254) return   255;
  else if (u_out < -254) return  -255;
  else                   return u_out; // OK
}

/**
 * An interrupt service routine for A, B encoders triggered on a CHANGE event.
 * Part of task 2 of Exercise 1.
 */
void ISR_readEncoder(){
  int encoder_A = digitalRead(ENCA), // Read the value of the A encoder
      encoder_B = digitalRead(ENCB); // Read the value of the B encoder

  // Update the position based on the obtained signal values:
  pos += (encoder_A == encoder_B)
          ?  1  // clockwise,
          : -1; // counter-clockwise.
}
