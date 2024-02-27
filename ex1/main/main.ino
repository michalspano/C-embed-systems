// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 6
// Exercise 1
// Submission code: <XXXYYY>

// Note: Task 1 should be implemented as a separate source code file (i.e.,
// Sketch).

// Defines section
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
int u_out = 0; // output of controller

int e = 0, // Globally used error value
    a = 0, // A-encoder signal
    b = 0; // A-encoder signal

/* The setup function of the program */
void setup() {
  Serial.begin(SERIAL_PORT);  // Begin Serial Monitor
  pinMode(ENCA,INPUT_PULLUP); // Channel A
  pinMode(ENCB,INPUT_PULLUP); // Channel B
  pinMode(PWM1,OUTPUT);       // I'm actually not sure what this congigures.
  pinMode(PWM2,OUTPUT);       // I'm actually not sure what this congigures (yet again).

  // Attach the interrupt; note: it should be sufficient to attacht the
  // interrupt to only one of the encoder ports.
  attachInterrupt(
                  digitalPinToInterrupt(ENCB), // Digital type at `ENCB`
                  ISR_readEncoder,             // Use the ISR
                  CHANGE                       // On event `CHANGE`
  );
  
  // Start the motor, just a tiny little bit because otherwise TinkerCad
  // dies... avoid the TinkerCad bug.
  analogWrite(PWM2, 10); delay(1000); analogWrite(PWM1, 10);
}

/* The loop function of the program */
void loop() {
  // Stop the motor, but not to zero because then TinkerCad dies... avoid the
  // TinkerCad bug.
  analogWrite(PWM1, 10); delay(1000); analogWrite(PWM2, 10);
  
  // Check if motor rotated all the way around, and reset the state
  if (pos > POS_MAX) {  // For any spins greater than one full circle
    deg = deg - FULL_CIRCLE_MAX;
    pos = pos - POS_MAX;
  } else if (pos < 0) { // For any spins lower than one full circle
    deg =  FULL_CIRCLE_MAX + deg;
    pos = POS_MAX + pos;
  }
  
  // Print current position (in degrees)
  Serial.print("The current position is: "); Serial.println(deg);
   
  // Get input from the user (from the Serial port)
  degtarget = getInput();
  
  // Calculate (initial) error
  // $\to e = pos_{is} - pos_{desired}$
  e = degtarget - deg;

  // Output the error onto the Serial Monitor. Can be disabled by replacing 1
  // by 0.
#if 1
  Serial.print("Current error: "); Serial.println(e);
#endif
    
  // Loop until error is zero
  while(e) {
    
    // Map current position into degrees
    deg = map(pos, 0, POS_MAX, 0, FULL_CIRCLE_MAX);
       
  	// Get necessary speed signal
  	speed = getAction(e);
    
  	// Send speed signal to motor (clockwise rotation)
    if (speed >= 0) {
      // The motor does not react with too low inputs
    	if (speed < 100) speed = 100;

      // Write to the motor
      analogWrite(PWM2, 0);
      analogWrite(PWM1, speed);
  	}
    // Counter-clockwise rotation
  	else {
      // The motor does not react with too low inputs
    	if (-speed < 100) speed = -100; 

      // Write to the motor
      analogWrite(PWM1, 0);
      analogWrite(PWM2, -speed); 
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
int getInput(){
  char buf[3];    // To store the raw value
  int ready =  0, // To indicate the state
      input = -1; // To store the parsed value

  Serial.print("Please enter the desired position: \n");
 
  // While the user is typing the value, block the exeuction
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
 */
int getAction(int error) {
  // Calculate u_out as function of the error and the kp (tuning parameter).
  // This relation is described in Task 3: A Position Controller.
  u_out = kp * e;

  // Note: u_out cannot be more than 255 or less than -254.
  // Otherwise, return the value. 
  if      (u_out >  254) return   255;
  else if (u_out < -254) return  -255;
  else                   return u_out; // OK
}

/* An interrupt service routine for A, B encoders */
void ISR_readEncoder(){
  int encoder_A = digitalRead(ENCA), // Read the value of the A encoder
      encoder_B = digitalRead(ENCB); // Read the value of the B encoder

  // Update the position based on the obtained signal values:
  pos += (encoder_A == encoder_B)
          ?  1  // clockwise,
          : -1; // counter-clockwise.
}
