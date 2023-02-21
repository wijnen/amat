// Main function

// Options: TODO: handle test option settings.
// CALL_loop
// NO_setup
// NO_main

#ifndef _AVR_MAIN_HH
#define _AVR_MAIN_HH

/** @file
# Main function
Example: Blink without interrupts
```
#define CALL_loop
#define SYSTEM_CLOCK1_ENABLE_CAPT

#include <amat.hh>

// Set output pin to built-in LED on atmega328p-based devices.
uint8_t out = Gpio::make_pin(PB, 5);

// Use an input pin to select fast or slow blinking.
uint8_t in = Gpio::make_pin(PD, 2);

void setup() {
	// Set pin to output (low)
	Gpio::write(out, false);

	// Set other pin to input with pullup.
	Gpio::input(in, true);
}

void loop() {
	// Set next event based on input pin.
	Counter::busy_wait(Gpio::read(in) ? 1000 : 250);

	// Flip the output state.
        Gpio::write(out, !Gpio::state(out));
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

#ifdef DOXYGEN
/// If this is defined, a main function must be defined by the user. @ingroup usemacros
#define NO_main
#undef NO_main

/// Defining this has the same effect as defining an empty setup() function. @ingroup usemacros
/**
 * If NO_main is defined, this macro has no effect.
 */
#define NO_setup
#undef NO_setup

/// Define this to use a custom loop function.
/**
 * If NO_main is defined, this macro has no effect.
 *
 * The default loop function will sleep in an endless loop.
 *
 * Note that interrupts are enabled before the endless loop, but they are not
 * enabled for a custom loop() function. This means that interrupts must be
 * enabled by the user (presumably in setup()) if this macro is defined and
 * interrupts should be enabled while the system is running.
 */
#define CALL_loop
#endif

#ifndef NO_main

#ifndef NO_setup
/// If the default main function is used, this is called to set up the system.
static void setup();
#endif

#ifdef CALL_loop
/// If the default main function is used and CALL_loop is defined, this is called to run the system. @ingroup usemacros
/**
 * If the default main function is used and CALL_loop is not defined, interrupts
 * are enabled and the main loop only calls Sleep::idle(). All functionality
 * must be provided from interrupts. This is the default.
 */
static void loop();
#endif

/// @cond
// Define everything that was requested to be set up.

// First make sure all the parts are defined. {{{

#ifndef _AVR_SETUP_COUNTER0
#define _AVR_SETUP_COUNTER0
#endif

#ifndef _AVR_SETUP_COUNTER1
#define _AVR_SETUP_COUNTER1
#endif

#ifndef _AVR_SETUP_COUNTER2
#define _AVR_SETUP_COUNTER2
#endif

#ifndef _AVR_SETUP_COUNTER3
#define _AVR_SETUP_COUNTER3
#endif

#ifndef _AVR_SETUP_COUNTER4
#define _AVR_SETUP_COUNTER4
#endif

#ifndef _AVR_SETUP_COUNTER5
#define _AVR_SETUP_COUNTER5
#endif

#ifndef _AVR_SETUP_USART
#define _AVR_SETUP_USART
#endif

#ifndef _AVR_SETUP_DBG
#define _AVR_SETUP_DBG
#endif

#ifndef _AVR_SETUP_USI
#define _AVR_SETUP_USI
#endif

#ifndef _AVR_SETUP_USB
#define _AVR_SETUP_USB
#endif

// @todo Add more setup from other parts.
// }}}

/// @endcond

// Finally, define AVR_SETUP which contains all parts.
/// This variable contains all setup that is requested through macro definitions
/**
 * It is placed in the default main function just before calling setup().
 *
 * The user only needs to use it when NO_main is defined. In that case, it
 * should be put in the main function to set everything up.
 */
#define AVR_SETUP \
	_AVR_SETUP_COUNTER0 \
	_AVR_SETUP_COUNTER1 \
	_AVR_SETUP_COUNTER2 \
	_AVR_SETUP_COUNTER3 \
	_AVR_SETUP_COUNTER4 \
	_AVR_SETUP_COUNTER5 \
	_AVR_SETUP_USART \
	_AVR_SETUP_DBG \
	_AVR_SETUP_USI \
	_AVR_SETUP_USB

/// The main function is defined if NO_main is not defined.
int main() {
	AVR_SETUP
#ifndef NO_setup
	setup();
#endif
#ifdef CALL_loop
	while (true)
		loop();
#else
	sei();
	while (true)
		Sleep::idle();
#endif
}

#endif

#endif

// vim: set foldmethod=marker :
