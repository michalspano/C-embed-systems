/*
 *  Interrupt and PWM utilities for 16 bit Timer1 on ATmega168/328
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *  Modified June 2011 by Lex Talionis to add a function to read the timer
 *  Modified Oct 2011 by Andrew Richards to avoid certain problems:
 *  - Add (long) assignments and casts to TimerOne::read() to ensure calculations involving tmp, ICR1 and TCNT1 aren't truncated
 *  - Ensure 16 bit registers accesses are atomic - run with interrupts disabled when accessing
 *  - Remove global enable of interrupts (sei())- could be running within an interrupt routine)
 *  - Disable interrupts whilst TCTN1 == 0.  Datasheet vague on this, but experiment shows that overflow interrupt 
 *    flag gets set whilst TCNT1 == 0, resulting in a phantom interrupt.  Could just set to 1, but gets inaccurate
 *    at very short durations
 *  - startBottom() added to start counter at 0 and handle all interrupt enabling.
 *  - start() amended to enable interrupts
 *  - restart() amended to point at startBottom()
 * Modiied 7:26 PM Sunday, October 09, 2011 by Lex Talionis
 *  - renamed start() to resume() to reflect it's actual role
 *  - renamed startBottom() to start(). This breaks some old code that expects start to continue counting where it left off
 *
 *  This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See Google Code project http://code.google.com/p/arduino-timerone/ for latest
 */
#ifndef TIMERONE_h
#define TIMERONE_h

#include <avr/io.h>
#include <avr/interrupt.h>

#define RESOLUTION 65536    // Timer1 is 16 bit

class TimerOne
{
  public:
  
    // properties
    unsigned int pwmPeriod;
    unsigned char clockSelectBits;
	char oldSREG;					// To hold Status Register while ints disabled

    // methods
    void initialize(long microseconds=1000000);
    void start();
    void stop();
    void restart();
	void resume();
	unsigned long read();
    void pwm(char pin, int duty, long microseconds=-1);
    void disablePwm(char pin);
    void attachInterrupt(void (*isr)(), long microseconds=-1);
    void detachInterrupt();
    void setPeriod(long microseconds);
    void setPwmDuty(char pin, int duty);
    void (*isrCallback)();
};

extern TimerOne Timer1;
#endif

/*
 *  Interrupt and PWM utilities for 16 bit Timer1 on ATmega168/328
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *  Modified June 2011 by Lex Talionis to add a function to read the timer
 *  Modified Oct 2011 by Andrew Richards to avoid certain problems:
 *  - Add (long) assignments and casts to TimerOne::read() to ensure calculations involving tmp, ICR1 and TCNT1 aren't truncated
 *  - Ensure 16 bit registers accesses are atomic - run with interrupts disabled when accessing
 *  - Remove global enable of interrupts (sei())- could be running within an interrupt routine)
 *  - Disable interrupts whilst TCTN1 == 0.  Datasheet vague on this, but experiment shows that overflow interrupt 
 *    flag gets set whilst TCNT1 == 0, resulting in a phantom interrupt.  Could just set to 1, but gets inaccurate
 *    at very short durations
 *  - startBottom() added to start counter at 0 and handle all interrupt enabling.
 *  - start() amended to enable interrupts
 *  - restart() amended to point at startBottom()
 * Modiied 7:26 PM Sunday, October 09, 2011 by Lex Talionis
 *  - renamed start() to resume() to reflect it's actual role
 *  - renamed startBottom() to start(). This breaks some old code that expects start to continue counting where it left off
 *
 *  This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See Google Code project http://code.google.com/p/arduino-timerone/ for latest
 */
#ifndef TIMERONE_cpp
#define TIMERONE_cpp

// #include "TimerOne.h"

TimerOne Timer1;              // preinstatiate

ISR(TIMER1_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  Timer1.isrCallback();
}


void TimerOne::initialize(long microseconds)
{
  TCCR1A = 0;                 // clear control register A 
  TCCR1B = _BV(WGM13);        // set mode 8: phase and frequency correct pwm, stop the timer
  setPeriod(microseconds);
}


void TimerOne::setPeriod(long microseconds)		// AR modified for atomic access
{
  
  long cycles = (F_CPU / 2000000) * microseconds;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
  
  oldSREG = SREG;				
  cli();							// Disable interrupts for 16 bit register access
  ICR1 = pwmPeriod = cycles;                                          // ICR1 is TOP in p & f correct pwm mode
  SREG = oldSREG;
  
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= clockSelectBits;                                          // reset clock select register, and starts the clock
}

void TimerOne::setPwmDuty(char pin, int duty)
{
  unsigned long dutyCycle = pwmPeriod;
  
  dutyCycle *= duty;
  dutyCycle >>= 10;
  
  oldSREG = SREG;
  cli();
  if(pin == 1 || pin == 9)       OCR1A = dutyCycle;
  else if(pin == 2 || pin == 10) OCR1B = dutyCycle;
  SREG = oldSREG;
}

void TimerOne::pwm(char pin, int duty, long microseconds)  // expects duty cycle to be 10 bit (1024)
{
  if(microseconds > 0) setPeriod(microseconds);
  if(pin == 1 || pin == 9) {
    DDRB |= _BV(PORTB1);                                   // sets data direction register for pwm output pin
    TCCR1A |= _BV(COM1A1);                                 // activates the output pin
  }
  else if(pin == 2 || pin == 10) {
    DDRB |= _BV(PORTB2);
    TCCR1A |= _BV(COM1B1);
  }
  setPwmDuty(pin, duty);
  resume();			// Lex - make sure the clock is running.  We don't want to restart the count, in case we are starting the second WGM
					// and the first one is in the middle of a cycle
}

void TimerOne::disablePwm(char pin)
{
  if(pin == 1 || pin == 9)       TCCR1A &= ~_BV(COM1A1);   // clear the bit that enables pwm on PB1
  else if(pin == 2 || pin == 10) TCCR1A &= ~_BV(COM1B1);   // clear the bit that enables pwm on PB2
}

void TimerOne::attachInterrupt(void (*isr)(), long microseconds)
{
  if(microseconds > 0) setPeriod(microseconds);
  isrCallback = isr;                                       // register the user's callback with the real ISR
  TIMSK1 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
	// might be running with interrupts disabled (eg inside an ISR), so don't touch the global state
//  sei();
  resume();												
}

void TimerOne::detachInterrupt()
{
  TIMSK1 &= ~_BV(TOIE1);                                   // clears the timer overflow interrupt enable bit 
															// timer continues to count without calling the isr
}

void TimerOne::resume()				// AR suggested
{ 
  TCCR1B |= clockSelectBits;
}

void TimerOne::restart()		// Depricated - Public interface to start at zero - Lex 10/9/2011
{
	start();				
}

void TimerOne::start()	// AR addition, renamed by Lex to reflect it's actual role
{
  unsigned int tcnt1;
  
  TIMSK1 &= ~_BV(TOIE1);        // AR added 
  GTCCR |= _BV(PSRSYNC);   		// AR added - reset prescaler (NB: shared with all 16 bit timers);

  oldSREG = SREG;				// AR - save status register
  cli();						// AR - Disable interrupts
  TCNT1 = 0;                	
  SREG = oldSREG;          		// AR - Restore status register
	resume();
  do {	// Nothing -- wait until timer moved on from zero - otherwise get a phantom interrupt
	oldSREG = SREG;
	cli();
	tcnt1 = TCNT1;
	SREG = oldSREG;
  } while (tcnt1==0); 
 
//  TIFR1 = 0xff;              		// AR - Clear interrupt flags
//  TIMSK1 = _BV(TOIE1);              // sets the timer overflow interrupt enable bit
}

void TimerOne::stop()
{
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));          // clears all clock selects bits
}

unsigned long TimerOne::read()		//returns the value of the timer in microseconds
{									//rember! phase and freq correct mode counts up to then down again
  	unsigned long tmp;				// AR amended to hold more than 65536 (could be nearly double this)
  	unsigned int tcnt1;				// AR added

	oldSREG= SREG;
  	cli();							
  	tmp=TCNT1;    					
	SREG = oldSREG;

	char scale=0;
	switch (clockSelectBits)
	{
	case 1:// no prescalse
		scale=0;
		break;
	case 2:// x8 prescale
		scale=3;
		break;
	case 3:// x64
		scale=6;
		break;
	case 4:// x256
		scale=8;
		break;
	case 5:// x1024
		scale=10;
		break;
	}
	
	do {	// Nothing -- max delay here is ~1023 cycles.  AR modified
		oldSREG = SREG;
		cli();
		tcnt1 = TCNT1;
		SREG = oldSREG;
	} while (tcnt1==tmp); //if the timer has not ticked yet

	//if we are counting down add the top value to how far we have counted down
	tmp = (  (tcnt1>tmp) ? (tmp) : (long)(ICR1-tcnt1)+(long)ICR1  );		// AR amended to add casts and reuse previous TCNT1
	return ((tmp*1000L)/(F_CPU /1000L))<<scale;
}

#endif

/* ==================== Source Code ==================== */

// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 5
// Exercise 1
// Submission code: 939466

// Macros section
#define f_BAUD    9600  // Frequency of the serial port
#define LED_COUNT 6     // Number of LEDs in this exericse, 5 + 1 Extra

// Represent lower and upper bounds as an enum to enhance readability.
// These bounds represent a state when the red LED is ON.
enum {
  LED_LOWER_BOUND = -1,
  LED_UPPER_BOUND = LED_COUNT -1
} LED_BOUNDS;

// Function prototypes
int8_t     t_map(const int t); // Map temperature value to an ordinal of LED
float parse_temp(const int r); // Parse raw temperature

// Digital pins of the LEDs in use
const uint8_t LED[] = { 13, 12, 8, 7, 4, 2 };

// Global state of the LEDs shared in the ICR
int8_t led_state = 0;

/* ==================== Sensor Ranges ==================== */
// Herein, the developer (or user) of the system can specify the
// individual intervals of the temperature readings for the LEDs.
//
// Note: the ranges are inclusive from both sides.

const uint8_t LED1_MIN =  0, // <- Start editing ranges here...
              LED1_MAX = 10,
              LED2_MIN = 11,
			        LED2_MAX = 20,
              LED3_MIN = 21,
			        LED3_MAX = 30,
              LED4_MIN = 31,
              LED4_MAX = 40,
              LED5_MIN = 41,
              LED5_MAX = 50;

/* ==================== Sensor Ranges ==================== */

/**
 * Initialize all components, begin a serial monitor at 9600.
 */
void setup() {
  Serial.begin(f_BAUD); // Serial monitor
  pinMode(A0, INPUT);   // Temperature sensor
  
  // Initialize all LEDs
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    pinMode(LED[i], OUTPUT);
  }
  
  // Declare TimerOne for the interrupt
  Timer1.initialize(1000000);  // <- One timer cycle (default: 1s)
  Timer1.attachInterrupt(isr); // Attach isr to timer1
}

/* Interrupt Service Routine to check the temperature periodically. */
void isr() {
  int raw_temp    = analogRead(A0);             // Get raw temperature reading
  int temperature = (int) parse_temp(raw_temp); // Parse the value to Celsius
  LED_routine(temperature);                     // Only when a new temp is read are the LEDs changed.
}

/**
 * This function is run continually while the board is turned on. Perform the
 * purpose of the program. The current implementation of this function is empty;
 * instead, the interrupt is utilized.
 */
void loop() {/* void */}

/* Routine served after isr service to handle the status of LEDs */
void LED_routine(const int temp) {
  int8_t led_idx = t_map(temp); // Assign the ordinal that the temp corresponds to

  // Detect a change in the number of LEDs to be on or off. This ensures that writings
  // to the LEDs are not carried out exhaustively and that resources are properly managed.
  if (led_state != led_idx) {
    
    // Detect if the current reading crossed any of the interval bounds,
    // if so, turn on the red LED. Otherwise, turn it off.
    led_idx == LED_LOWER_BOUND || led_idx == LED_UPPER_BOUND
      ? digitalWrite(LED[LED_COUNT - 1], HIGH)
      : digitalWrite(LED[LED_COUNT - 1], LOW);
	
    // Some LEDs are to be dimmed (i.e., turned off)
    if (led_state > led_idx) {
      // Take the difference of led_state and led_idx that determines
      // how many LEDs are to be dimmed. Iterate that many times.
      for (uint8_t i = 0; i < led_state - led_idx; i++) {
        uint8_t idx = led_state - i; // Index of the current LED to dim
        
        // Note: this is a corner case. When led_state is 5 and led_idx is
        // -1, this loop would mistakenly dim the red LED, even though that
        // should not happen, hence the following:
        if (idx == LED_UPPER_BOUND) continue;
        
        // Dim the LED at the current index.
        digitalWrite(LED[idx], LOW);
      }
    // Otherwise, some LEDs are to be turned on.
    } else {
      // Take the difference of led_idx and led_state to determine how many
      // more LEDs are to be turned on. Then, iterate that many times.
      for (uint8_t i = 0; i <= led_idx - led_state; i++) {
        // Light up an LED at the current index
        digitalWrite(LED[led_state + i], HIGH);
      }
    }
  } 
  
  // Assign the global state variable with the current reading index.
  led_state = led_idx;
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

  // Include 500 mV (0.5V) offset, convert to temperature in Celsius.
  return (voltage - 0.5) * 100;
}

/**
 * A helper function to map a temperature value to an ordinal. This value is
 * then used to light up particular LEDs from the sequence. -1 denotes that the
 * value is too low (red LED is turned on) and 5 denotes that the value is too
 * high (red LED is too turned on).
 *
 * @param t - temperature value (in Celsius).
 * @returns - ordinal value (-1 [LED_LOW_BOUND], 0 through 4 or 5 [LED_UPPER_BOUND])
 */
int8_t t_map(const int t) {
  if      (t <  LED1_MIN)                  return LED_LOWER_BOUND; // too low
  else if (t >= LED1_MIN && t <= LED1_MAX) return 0; // range1 satisfied
  else if (t >= LED2_MIN && t <= LED2_MAX) return 1; // range2 satisfied
  else if (t >= LED3_MIN && t <= LED3_MAX) return 2; // ...
  else if (t >= LED4_MIN && t <= LED4_MAX) return 3;
  else if (t >= LED5_MIN && t <= LED5_MAX) return 4;
  else                                     return LED_UPPER_BOUND; // too high
}
