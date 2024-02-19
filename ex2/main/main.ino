// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 5
// Exercise 2
// Submission code: <XXXYYY>
//
// Note: the following implementation is largely based on this article:
// - https://projecthub.arduino.cc/Marcazzan_M/internal-timers-of-arduino-6c0f66

// Includes section
#include <Servo.h>

// Macros section
#define SERVO_PIN        3    // PWM (digital) signal port for the servo motor
#define ANGLE_INCR       3    // 180 deg, 60 sec = 3 unit increment every 1 sec
#define SECOND_MAX       60   // 60 seconds in 1 minute
#define ANGLE_MAX        180  // The servo motor can be adjusted up to at most
                              // 180 deg (1 rad)
#define CLOCK_INTERVAL   1000 // 1 second cycle
#define BAUD_SERIAL_RATE 9600 // Port of serial monitor

Servo servo_motor; // Servo motor instance

long timer    = 0; // Timer initial declaration
uint8_t angle = 0; // The initial angle of the servo motor

// Function prototypes
void print_sec(const uint8_t angle);   // Print seconds based on the angle
uint8_t incr_angle(const uint8_t deg); // Increment the current value of the angle

/* Subroutine to set up the timer that is used */
void setup_timer() {
  cli();                 // Disable interrupts before config
  TCCR0A = (1<<WGM01);   // Set the CTC mode   
  OCR0A  = 0xF9;         // Value for ORC0A - 1ms 

  TIMSK0 |= (1<<OCIE0A); // Set the interrupt request
  TCCR0B |= (1<<CS01)
         |  (1<<CS00);   // Set pre-scaler to 1/64 a clock unit
  sei();                 // Enable interrupts after config
}

/* Set up the components of the system */
void setup() {
  Serial.begin(BAUD_SERIAL_RATE); // Begin the serial monitor (default: 9600)
  setup_timer();                  // Call the routine to set up the timer
  
  servo_motor.attach(SERVO_PIN);  // Attach the servo motor to a pin of the board
  servo_motor.write(angle);       // 0 deg (initial call)
}

/* Program's main loop */
void loop() {
  if (timer >= CLOCK_INTERVAL) { // Detect an elapsed interval (i.e., 1s)
    timer = 0;                   // Reset the timer
    angle = incr_angle(angle);   // Modify the angle
    servo_motor.write(angle);    // Indicate that a second has passed (via the motor)
    print_sec(angle);            // Print how many seconds have elapsed (Serial monitor)
  }
}

// Utilize the interrupt service routine, increase the timer by 1.
// Using the `COMPA` vector to trigger the interrupt.
ISR(TIMER0_COMPA_vect) { timer++; }

/**
 * A helper function to increase the angle of the servo motor by `ANGLE_INCR`
 * until it reaches the maximum value of `ANGLE_MAX`. If the maximum value is
 * reached, the angle is reset to 0.
 *
 * @param deg - the current angle of the servo motor
 * @returns   - the new angle of the servo motor
 */
uint8_t incr_angle(const uint8_t deg) {
  angle += ANGLE_INCR;              // Increase the angle by `ANGLE_INCR`
  if (angle > ANGLE_MAX) angle = 0; // Reset the angle to 0 if it exceeds the maximum
  return angle;                     // Return the new angle
}

/**
 * A helper function to print the number of seconds that have elapsed based on
 * the angle of the servo motor. We have a 180 deg servo motor, and we want to
 * map the angle to the number of seconds that have elapsed. We can do this by
 * dividing the angle by `ANGLE_INCR` and subtracting the result from `SECOND_MAX`.
 *
 * @param angle - the angle of the servo motor
 */
void print_sec(const uint8_t angle) {
  Serial.print(SECOND_MAX - angle / ANGLE_INCR);
  Serial.println(" s");
}
