// This solution is largely based on (this has been provided by the TAs):
// - https://projecthub.arduino.cc/Marcazzan_M/internal-timers-of-arduino-6c0f66

#include <Servo.h>

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

void print_sec(const uint8_t angle);
uint8_t incr_angle(const uint8_t deg);

void setup_timer() {
  cli();                 // Disable interrupts before config.
  TCCR0A  = (1<<WGM01);  // Set the CTC mode   
  OCR0A   = 0xF9;        // Value for ORC0A for 1ms 
  TIMSK0 |= (1<<OCIE0A); // Set the interrupt request
  TCCR0B |= (1<<CS01)
         |  (1<<CS00);   // Set prescaler to 1/64 a clock unit
  sei();                 // Enable interrupts after config.
}

/* Set up the components of the system */
void setup() {
  Serial.begin(BAUD_SERIAL_RATE);
  setup_timer();
  
  servo_motor.attach(SERVO_PIN); // Attach servo motor to a pin of the board
  servo_motor.write(angle);      // 0 deg
}

void loop() {
  if (timer >= CLOCK_INTERVAL) { // Detect an elapsed interval (1s)
    timer = 0;                   // Reset the timer
    angle = incr_angle(angle);   // Alter the value of the angle
    servo_motor.write(angle);    // Indicate that a second has passed
    print_sec(angle);            // Print how many seconds have elapsed to the Serial monitor
  }
}

// Utilize the interrupt service routine, increase the timer.
ISR(TIMER0_COMPA_vect){ timer++; }

// Increate the angle per in a one second time unit. Make sure that
// the maximum angle is not exceeded.
uint8_t incr_angle(const uint8_t deg) {
  angle += ANGLE_INCR;
  if (angle > ANGLE_MAX) angle = 0;
  return angle;
}

// Routine to map the current angle to time (in seconds). Each second
// the angle is increased by `ANGLE_INCR` units. Hence, to get the number
// of seconds, we must divide the value by the increment. Furthermore, a
// minute is counted down from 60, 59, and so forth.
void print_sec(const uint8_t angle) {
  Serial.print(SECOND_MAX - angle / ANGLE_INCR);
  Serial.println(" s");
}
