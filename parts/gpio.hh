// Gpio support.

#ifndef _AVR_GPIO_HH
#define _AVR_GPIO_HH

/** @file
# General purpose input/output
Example: Blink
```
#define SYSTEM_CLOCK1_ENABLE_CAPT
#define CALL_system_clock_interrupt

#include <avr-ll.hh>

// Set output pin to built-in LED on atmega328p-based devices.
uint8_t out = Gpio::make_pin(PB, 5);

// Use an input pin to select fast or slow blinking.
uint8_t in = Gpio::make_pin(PD, 2);

void setup() {
	// Set pin to output (low)
	Gpio::write(out, false);

	// Set other pin to input with pullup.
	Gpio::input(in, true);

	// Schedule interrupt.
	Counter::set_timeout(1);
}

void system_clock_interrupt() {
	// Set next event based on input pin.
	Counter::set_timeout(Gpio::read(in) ? 1000 : 250);

	// Flip the output state.
	Gpio::write(out, !Gpio::state(out));
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

// Interface:
// void write(pin, is_high)
// bool read(pin)
// void input(pin, pullup_enabled = true)

// Create macros to handle ports. {{{
/// @cond

#ifdef PORTL
	// PL exists.
#	define _AVR_USE_PORTL(...) __VA_ARGS__
#	define PL 11
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PL
#	endif
// Make bitmask. {{{
#ifdef PORTL0
#define _AVR_PL0 (1 << 0)
#else
#define _AVR_PL0 0
#endif

#ifdef PORTL1
#define _AVR_PL1 (1 << 1)
#else
#define _AVR_PL1 0
#endif

#ifdef PORTL2
#define _AVR_PL2 (1 << 2)
#else
#define _AVR_PL2 0
#endif

#ifdef PORTL3
#define _AVR_PL3 (1 << 3)
#else
#define _AVR_PL3 0
#endif

#ifdef PORTL4
#define _AVR_PL4 (1 << 4)
#else
#define _AVR_PL4 0
#endif

#ifdef PORTL5
#define _AVR_PL5 (1 << 5)
#else
#define _AVR_PL5 0
#endif

#ifdef PORTL6
#define _AVR_PL6 (1 << 6)
#else
#define _AVR_PL6 0
#endif

#ifdef PORTL7
#define _AVR_PL7 (1 << 7)
#else
#define _AVR_PL7 0
#endif

#define _AVR_GPIO_MASK_PORTL (_AVR_PL0 | _AVR_PL1 | _AVR_PL2 | _AVR_PL3 | _AVR_PL4 | _AVR_PL5 | _AVR_PL6 | _AVR_PL7)
#define _AVR_PIN_COUNT_PORTL (!!_AVR_PL0 + !!_AVR_PL1 + !!_AVR_PL2 + !!_AVR_PL3 + !!_AVR_PL4 + !!_AVR_PL5 + !!_AVR_PL6 + !!_AVR_PL7)
// }}}
#else
	// PL does not exist.
#	define _AVR_USE_PORTL(...)
#endif

#ifdef PORTK
	// PK exists.
#	define _AVR_USE_PORTK(...) __VA_ARGS__
#	define PK 10
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PK
#	endif
// Make bitmask. {{{
#ifdef PORTK0
#define _AVR_PK0 (1 << 0)
#else
#define _AVR_PK0 0
#endif

#ifdef PORTK1
#define _AVR_PK1 (1 << 1)
#else
#define _AVR_PK1 0
#endif

#ifdef PORTK2
#define _AVR_PK2 (1 << 2)
#else
#define _AVR_PK2 0
#endif

#ifdef PORTK3
#define _AVR_PK3 (1 << 3)
#else
#define _AVR_PK3 0
#endif

#ifdef PORTK4
#define _AVR_PK4 (1 << 4)
#else
#define _AVR_PK4 0
#endif

#ifdef PORTK5
#define _AVR_PK5 (1 << 5)
#else
#define _AVR_PK5 0
#endif

#ifdef PORTK6
#define _AVR_PK6 (1 << 6)
#else
#define _AVR_PK6 0
#endif

#ifdef PORTK7
#define _AVR_PK7 (1 << 7)
#else
#define _AVR_PK7 0
#endif

#define _AVR_GPIO_MASK_PORTK (_AVR_PK0 | _AVR_PK1 | _AVR_PK2 | _AVR_PK3 | _AVR_PK4 | _AVR_PK5 | _AVR_PK6 | _AVR_PK7)
#define _AVR_PIN_COUNT_PORTK (!!_AVR_PK0 + !!_AVR_PK1 + !!_AVR_PK2 + !!_AVR_PK3 + !!_AVR_PK4 + !!_AVR_PK5 + !!_AVR_PK6 + !!_AVR_PK7)
// }}}
#else
	// PK does not exist.
#	define _AVR_USE_PORTK(...)
#endif

#ifdef PORTJ
	// PJ exists.
#	define _AVR_USE_PORTJ(...) __VA_ARGS__
#	define PJ 9
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PJ
#	endif
// Make bitmask. {{{
#ifdef PORTJ0
#define _AVR_PJ0 (1 << 0)
#else
#define _AVR_PJ0 0
#endif

#ifdef PORTJ1
#define _AVR_PJ1 (1 << 1)
#else
#define _AVR_PJ1 0
#endif

#ifdef PORTJ2
#define _AVR_PJ2 (1 << 2)
#else
#define _AVR_PJ2 0
#endif

#ifdef PORTJ3
#define _AVR_PJ3 (1 << 3)
#else
#define _AVR_PJ3 0
#endif

#ifdef PORTJ4
#define _AVR_PJ4 (1 << 4)
#else
#define _AVR_PJ4 0
#endif

#ifdef PORTJ5
#define _AVR_PJ5 (1 << 5)
#else
#define _AVR_PJ5 0
#endif

#ifdef PORTJ6
#define _AVR_PJ6 (1 << 6)
#else
#define _AVR_PJ6 0
#endif

#ifdef PORTJ7
#define _AVR_PJ7 (1 << 7)
#else
#define _AVR_PJ7 0
#endif

#define _AVR_GPIO_MASK_PORTJ (_AVR_PJ0 | _AVR_PJ1 | _AVR_PJ2 | _AVR_PJ3 | _AVR_PJ4 | _AVR_PJ5 | _AVR_PJ6 | _AVR_PJ7)
#define _AVR_PIN_COUNT_PORTJ (!!_AVR_PJ0 + !!_AVR_PJ1 + !!_AVR_PJ2 + !!_AVR_PJ3 + !!_AVR_PJ4 + !!_AVR_PJ5 + !!_AVR_PJ6 + !!_AVR_PJ7)
// }}}
#else
	// PJ does not exist.
#	define _AVR_USE_PORTJ(...)
#endif

#ifdef PORTH
	// PH exists.
#	define _AVR_USE_PORTH(...) __VA_ARGS__
#	define PH 7
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PH
#	endif
// Make bitmask. {{{
#ifdef PORTH0
#define _AVR_PH0 (1 << 0)
#else
#define _AVR_PH0 0
#endif

#ifdef PORTH1
#define _AVR_PH1 (1 << 1)
#else
#define _AVR_PH1 0
#endif

#ifdef PORTH2
#define _AVR_PH2 (1 << 2)
#else
#define _AVR_PH2 0
#endif

#ifdef PORTH3
#define _AVR_PH3 (1 << 3)
#else
#define _AVR_PH3 0
#endif

#ifdef PORTH4
#define _AVR_PH4 (1 << 4)
#else
#define _AVR_PH4 0
#endif

#ifdef PORTH5
#define _AVR_PH5 (1 << 5)
#else
#define _AVR_PH5 0
#endif

#ifdef PORTH6
#define _AVR_PH6 (1 << 6)
#else
#define _AVR_PH6 0
#endif

#ifdef PORTH7
#define _AVR_PH7 (1 << 7)
#else
#define _AVR_PH7 0
#endif

#define _AVR_GPIO_MASK_PORTH (_AVR_PH0 | _AVR_PH1 | _AVR_PH2 | _AVR_PH3 | _AVR_PH4 | _AVR_PH5 | _AVR_PH6 | _AVR_PH7)
#define _AVR_PIN_COUNT_PORTH (!!_AVR_PH0 + !!_AVR_PH1 + !!_AVR_PH2 + !!_AVR_PH3 + !!_AVR_PH4 + !!_AVR_PH5 + !!_AVR_PH6 + !!_AVR_PH7)
// }}}
#else
	// PH does not exist.
#	define _AVR_USE_PORTH(...)
#endif

#ifdef PORTG
	// PG exists.
#	define _AVR_USE_PORTG(...) __VA_ARGS__
#	define PG 6
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PG
#	endif
// Make bitmask. {{{
#ifdef PORTG0
#define _AVR_PG0 (1 << 0)
#else
#define _AVR_PG0 0
#endif

#ifdef PORTG1
#define _AVR_PG1 (1 << 1)
#else
#define _AVR_PG1 0
#endif

#ifdef PORTG2
#define _AVR_PG2 (1 << 2)
#else
#define _AVR_PG2 0
#endif

#ifdef PORTG3
#define _AVR_PG3 (1 << 3)
#else
#define _AVR_PG3 0
#endif

#ifdef PORTG4
#define _AVR_PG4 (1 << 4)
#else
#define _AVR_PG4 0
#endif

#ifdef PORTG5
#define _AVR_PG5 (1 << 5)
#else
#define _AVR_PG5 0
#endif

#ifdef PORTG6
#define _AVR_PG6 (1 << 6)
#else
#define _AVR_PG6 0
#endif

#ifdef PORTG7
#define _AVR_PG7 (1 << 7)
#else
#define _AVR_PG7 0
#endif

#define _AVR_GPIO_MASK_PORTG (_AVR_PG0 | _AVR_PG1 | _AVR_PG2 | _AVR_PG3 | _AVR_PG4 | _AVR_PG5 | _AVR_PG6 | _AVR_PG7)
#define _AVR_PIN_COUNT_PORTG (!!_AVR_PG0 + !!_AVR_PG1 + !!_AVR_PG2 + !!_AVR_PG3 + !!_AVR_PG4 + !!_AVR_PG5 + !!_AVR_PG6 + !!_AVR_PG7)
// }}}
#else
	// PG does not exist.
#	define _AVR_USE_PORTG(...)
#endif

#ifdef PORTF
	// PF exists.
#	define _AVR_USE_PORTF(...) __VA_ARGS__
#	define PF 5
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PF
#	endif
// Make bitmask. {{{
#ifdef PORTF0
#define _AVR_PF0 (1 << 0)
#else
#define _AVR_PF0 0
#endif

#ifdef PORTF1
#define _AVR_PF1 (1 << 1)
#else
#define _AVR_PF1 0
#endif

#ifdef PORTF2
#define _AVR_PF2 (1 << 2)
#else
#define _AVR_PF2 0
#endif

#ifdef PORTF3
#define _AVR_PF3 (1 << 3)
#else
#define _AVR_PF3 0
#endif

#ifdef PORTF4
#define _AVR_PF4 (1 << 4)
#else
#define _AVR_PF4 0
#endif

#ifdef PORTF5
#define _AVR_PF5 (1 << 5)
#else
#define _AVR_PF5 0
#endif

#ifdef PORTF6
#define _AVR_PF6 (1 << 6)
#else
#define _AVR_PF6 0
#endif

#ifdef PORTF7
#define _AVR_PF7 (1 << 7)
#else
#define _AVR_PF7 0
#endif

#define _AVR_GPIO_MASK_PORTF (_AVR_PF0 | _AVR_PF1 | _AVR_PF2 | _AVR_PF3 | _AVR_PF4 | _AVR_PF5 | _AVR_PF6 | _AVR_PF7)
#define _AVR_PIN_COUNT_PORTF (!!_AVR_PF0 + !!_AVR_PF1 + !!_AVR_PF2 + !!_AVR_PF3 + !!_AVR_PF4 + !!_AVR_PF5 + !!_AVR_PF6 + !!_AVR_PF7)
// }}}
#else
	// PF does not exist.
#	define _AVR_USE_PORTF(...)
#endif

#ifdef PORTE
	// PE exists.
#	define _AVR_USE_PORTE(...) __VA_ARGS__
#	define PE 4
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PE
#	endif
// Make bitmask. {{{
#ifdef PORTE0
#define _AVR_PE0 (1 << 0)
#else
#define _AVR_PE0 0
#endif

#ifdef PORTE1
#define _AVR_PE1 (1 << 1)
#else
#define _AVR_PE1 0
#endif

#ifdef PORTE2
#define _AVR_PE2 (1 << 2)
#else
#define _AVR_PE2 0
#endif

#ifdef PORTE3
#define _AVR_PE3 (1 << 3)
#else
#define _AVR_PE3 0
#endif

#ifdef PORTE4
#define _AVR_PE4 (1 << 4)
#else
#define _AVR_PE4 0
#endif

#ifdef PORTE5
#define _AVR_PE5 (1 << 5)
#else
#define _AVR_PE5 0
#endif

#ifdef PORTE6
#define _AVR_PE6 (1 << 6)
#else
#define _AVR_PE6 0
#endif

#ifdef PORTE7
#define _AVR_PE7 (1 << 7)
#else
#define _AVR_PE7 0
#endif

#define _AVR_GPIO_MASK_PORTE (_AVR_PE0 | _AVR_PE1 | _AVR_PE2 | _AVR_PE3 | _AVR_PE4 | _AVR_PE5 | _AVR_PE6 | _AVR_PE7)
#define _AVR_PIN_COUNT_PORTE (!!_AVR_PE0 + !!_AVR_PE1 + !!_AVR_PE2 + !!_AVR_PE3 + !!_AVR_PE4 + !!_AVR_PE5 + !!_AVR_PE6 + !!_AVR_PE7)
// }}}
#else
	// PE does not exist.
#	define _AVR_USE_PORTE(...)
#endif

#ifdef PORTD
	// PD exists.
#	define _AVR_USE_PORTD(...) __VA_ARGS__
#	define PD 3
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PD
#	endif
// Make bitmask. {{{
#ifdef PORTD0
#define _AVR_PD0 (1 << 0)
#else
#define _AVR_PD0 0
#endif

#ifdef PORTD1
#define _AVR_PD1 (1 << 1)
#else
#define _AVR_PD1 0
#endif

#ifdef PORTD2
#define _AVR_PD2 (1 << 2)
#else
#define _AVR_PD2 0
#endif

#ifdef PORTD3
#define _AVR_PD3 (1 << 3)
#else
#define _AVR_PD3 0
#endif

#ifdef PORTD4
#define _AVR_PD4 (1 << 4)
#else
#define _AVR_PD4 0
#endif

#ifdef PORTD5
#define _AVR_PD5 (1 << 5)
#else
#define _AVR_PD5 0
#endif

#ifdef PORTD6
#define _AVR_PD6 (1 << 6)
#else
#define _AVR_PD6 0
#endif

#ifdef PORTD7
#define _AVR_PD7 (1 << 7)
#else
#define _AVR_PD7 0
#endif

#define _AVR_GPIO_MASK_PORTD (_AVR_PD0 | _AVR_PD1 | _AVR_PD2 | _AVR_PD3 | _AVR_PD4 | _AVR_PD5 | _AVR_PD6 | _AVR_PD7)
#define _AVR_PIN_COUNT_PORTD (!!_AVR_PD0 + !!_AVR_PD1 + !!_AVR_PD2 + !!_AVR_PD3 + !!_AVR_PD4 + !!_AVR_PD5 + !!_AVR_PD6 + !!_AVR_PD7)
// }}}
#else
	// PD does not exist.
#	define _AVR_USE_PORTD(...)
#endif

#ifdef PORTC
	// PC exists.
#	define _AVR_USE_PORTC(...) __VA_ARGS__
#	define PC 2
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PC
#	endif
// Make bitmask. {{{
#ifdef PORTC0
#define _AVR_PC0 (1 << 0)
#else
#define _AVR_PC0 0
#endif

#ifdef PORTC1
#define _AVR_PC1 (1 << 1)
#else
#define _AVR_PC1 0
#endif

#ifdef PORTC2
#define _AVR_PC2 (1 << 2)
#else
#define _AVR_PC2 0
#endif

#ifdef PORTC3
#define _AVR_PC3 (1 << 3)
#else
#define _AVR_PC3 0
#endif

#ifdef PORTC4
#define _AVR_PC4 (1 << 4)
#else
#define _AVR_PC4 0
#endif

#ifdef PORTC5
#define _AVR_PC5 (1 << 5)
#else
#define _AVR_PC5 0
#endif

#ifdef PORTC6
#define _AVR_PC6 (1 << 6)
#else
#define _AVR_PC6 0
#endif

#ifdef PORTC7
#define _AVR_PC7 (1 << 7)
#else
#define _AVR_PC7 0
#endif

#define _AVR_GPIO_MASK_PORTC (_AVR_PC0 | _AVR_PC1 | _AVR_PC2 | _AVR_PC3 | _AVR_PC4 | _AVR_PC5 | _AVR_PC6 | _AVR_PC7)
#define _AVR_PIN_COUNT_PORTC (!!_AVR_PC0 + !!_AVR_PC1 + !!_AVR_PC2 + !!_AVR_PC3 + !!_AVR_PC4 + !!_AVR_PC5 + !!_AVR_PC6 + !!_AVR_PC7)
// }}}
#else
	// PC does not exist.
#	define _AVR_USE_PORTC(...)
#endif

#ifdef PORTB
	// PB exists.
#	define _AVR_USE_PORTB(...) __VA_ARGS__
#	define PB 1
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PB
#	endif
// Make bitmask. {{{
#ifdef PORTB0
#define _AVR_PB0 (1 << 0)
#else
#define _AVR_PB0 0
#endif

#ifdef PORTB1
#define _AVR_PB1 (1 << 1)
#else
#define _AVR_PB1 0
#endif

#ifdef PORTB2
#define _AVR_PB2 (1 << 2)
#else
#define _AVR_PB2 0
#endif

#ifdef PORTB3
#define _AVR_PB3 (1 << 3)
#else
#define _AVR_PB3 0
#endif

#ifdef PORTB4
#define _AVR_PB4 (1 << 4)
#else
#define _AVR_PB4 0
#endif

#ifdef PORTB5
#define _AVR_PB5 (1 << 5)
#else
#define _AVR_PB5 0
#endif

#ifdef PORTB6
#define _AVR_PB6 (1 << 6)
#else
#define _AVR_PB6 0
#endif

#ifdef PORTB7
#define _AVR_PB7 (1 << 7)
#else
#define _AVR_PB7 0
#endif

#define _AVR_GPIO_MASK_PORTB (_AVR_PB0 | _AVR_PB1 | _AVR_PB2 | _AVR_PB3 | _AVR_PB4 | _AVR_PB5 | _AVR_PB6 | _AVR_PB7)
#define _AVR_PIN_COUNT_PORTB (!!_AVR_PB0 + !!_AVR_PB1 + !!_AVR_PB2 + !!_AVR_PB3 + !!_AVR_PB4 + !!_AVR_PB5 + !!_AVR_PB6 + !!_AVR_PB7)
// }}}
#else
	// PB does not exist.
#	define _AVR_USE_PORTB(...)
#endif

#ifdef PORTA
	// PA exists.
#	define _AVR_USE_PORTA(...) __VA_ARGS__
#	define PA 0
#	ifndef _AVR_LAST_PORT
		// This is the first existing port.
#		define _AVR_LAST_PORT PA
#	endif
// Make bitmask. {{{
#ifdef PORTA0
#define _AVR_PA0 (1 << 0)
#else
#define _AVR_PA0 0
#endif

#ifdef PORTA1
#define _AVR_PA1 (1 << 1)
#else
#define _AVR_PA1 0
#endif

#ifdef PORTA2
#define _AVR_PA2 (1 << 2)
#else
#define _AVR_PA2 0
#endif

#ifdef PORTA3
#define _AVR_PA3 (1 << 3)
#else
#define _AVR_PA3 0
#endif

#ifdef PORTA4
#define _AVR_PA4 (1 << 4)
#else
#define _AVR_PA4 0
#endif

#ifdef PORTA5
#define _AVR_PA5 (1 << 5)
#else
#define _AVR_PA5 0
#endif

#ifdef PORTA6
#define _AVR_PA6 (1 << 6)
#else
#define _AVR_PA6 0
#endif

#ifdef PORTA7
#define _AVR_PA7 (1 << 7)
#else
#define _AVR_PA7 0
#endif

#define _AVR_GPIO_MASK_PORTA (_AVR_PA0 | _AVR_PA1 | _AVR_PA2 | _AVR_PA3 | _AVR_PA4 | _AVR_PA5 | _AVR_PA6 | _AVR_PA7)
#define _AVR_PIN_COUNT_PORTA (!!_AVR_PA0 + !!_AVR_PA1 + !!_AVR_PA2 + !!_AVR_PA3 + !!_AVR_PA4 + !!_AVR_PA5 + !!_AVR_PA6 + !!_AVR_PA7)
// }}}
#else
	// PA does not exist.
#	define _AVR_USE_PORTA(...)
#endif
#define NUM_DIGITAL_PINS (_AVR_PINT_COUNT_PORTA + _AVR_PINT_COUNT_PORTB + _AVR_PINT_COUNT_PORTC + _AVR_PINT_COUNT_PORTD + _AVR_PINT_COUNT_PORTE + _AVR_PINT_COUNT_PORTF + _AVR_PINT_COUNT_PORTG + _AVR_PINT_COUNT_PORTH + _AVR_PINT_COUNT_PORTJ + _AVR_PINT_COUNT_PORTK + _AVR_PINT_COUNT_PORTL)

#define _AVR_PORTREG(OLD, NEW, MOD, REF, DEF) \
	static inline MOD uint8_t REF NEW(uint8_t port) { \
		switch (port) { \
		_AVR_USE_PORTA( case PA: return OLD ## A; ) \
		_AVR_USE_PORTB( case PB: return OLD ## B; ) \
		_AVR_USE_PORTC( case PC: return OLD ## C; ) \
		_AVR_USE_PORTD( case PD: return OLD ## D; ) \
		_AVR_USE_PORTE( case PE: return OLD ## E; ) \
		_AVR_USE_PORTF( case PF: return OLD ## F; ) \
		_AVR_USE_PORTG( case PG: return OLD ## G; ) \
		_AVR_USE_PORTH( case PH: return OLD ## H; ) \
		_AVR_USE_PORTJ( case PJ: return OLD ## J; ) \
		_AVR_USE_PORTK( case PK: return OLD ## K; ) \
		_AVR_USE_PORTL( case PL: return OLD ## L; ) \
		default: return DEF; \
		} \
	}
/// @endcond
// }}}

/// General purpose input/output pins
namespace Gpio {
	// Register access. {{{
	_AVR_PORTREG(DDR, DDR, volatile, &, GPIOR0)
	_AVR_PORTREG(PORT, PORT, volatile, &, GPIOR0)
	_AVR_PORTREG(PIN, PIN, volatile, &, GPIOR0)
	_AVR_PORTREG(_AVR_GPIO_MASK_PORT, MASK,,, 0)
#ifdef DOXYGEN
	/// Access to data direction registers.
	/**
	 * Returns &GPIOR0 if port does not exist.
	 */
	volatile static inline uint8_t &DDR(uint8_t port);

	/// Access to port output registers.
	/**
	 * Returns &GPIOR0 if port does not exist.
	 */
	volatile static inline uint8_t &PORT(uint8_t port);

	/// Access to pin input registers.
	/**
	 * Returns &GPIOR0 if port does not exist.
	 */
	volatile static inline uint8_t &PIN(uint8_t port);

	/// Mask of available pins.
	/**
	 * Returns a bitmask, where a 1 in the returned value means that pin
	 * exists.
	 *
	 * For example, on the atmega32u4, in port E only pins 2 and 6 exist.
	 * On that device, Gpio::MASK(PE) == 0x44.
	 */
	static inline uint8_t MASK(uint8_t port);
#endif
	// }}}

/// Internal version of Gpio::make_pin(), usable for constants that need to be initialized in PROGMEM.
#define GPIO_MAKE_PIN(port, bit) (bit | (port << 3))

	/// Function for creating a pin identifier.
	/**
	 * @param port: A port constant such as PA or PB should be passed here.
	 * @param bit: The bit in the port, range 0 through 7.
	 * The returned value can be used in the other Gpio functions.
	 */
	static inline uint8_t make_pin(uint8_t port, uint8_t bit) { return GPIO_MAKE_PIN(port, bit); }

	/// Check if a pin is present in the device.
	static inline bool check_pin(uint8_t pin) { // {{{
		uint8_t port = (pin >> 3) & 0xf;
		volatile uint8_t &portreg = PORT(port);
		return &portreg != &GPIOR0 && (MASK(port) & (1 << (pin & 0x7)));
	} // }}}

	/// Set the pin to high or low output.
	static inline void write(uint8_t pin, bool is_high) { // {{{
		uint8_t mask = 1 << (pin & 0x7);
		uint8_t port = (pin >> 3) & 0xf;
		volatile uint8_t &portreg = PORT(port);
		volatile uint8_t &ddrreg = DDR(port);
		if (is_high)
			portreg |= mask;
		else
			portreg &= ~mask;
		ddrreg |= mask;
	} // }}}

	/// Read the current input value of a digital pin.
	static inline bool read(uint8_t pin) { // {{{
		uint8_t mask = 1 << (pin & 0x7);
		uint8_t port = (pin >> 3) & 0xf;
		return PIN(port) & mask;
	} // }}}

	/// Read the current output state of a digital pin.
	/**
	 * This returns true if the pin is set to output high, or to input with
	 * pullup enabled, false if the pin is set to output low or input
	 * without pullup enabled.
	 *
	 * The return value does not reflect the actual voltage on an input pin.
	 *
	 * @sa read
	 */
	static inline bool state(uint8_t pin) { // {{{
		uint8_t mask = 1 << (pin & 0x7);
		uint8_t port = (pin >> 3) & 0xf;
		return PORT(port) & mask;
	} // }}}

	/// Set the pin to input, with or without a pullup.
	static inline void input(uint8_t pin, bool pullup_enabled) { // {{{
		uint8_t mask = 1 << (pin & 0x7);
		uint8_t port = (pin >> 3) & 0xf;
		DDR(port) &= ~mask;
		if (pullup_enabled)
			PORT(port) |= mask;
		else
			PORT(port) &= ~mask;
	} // }}}
}

#ifdef AVR_TEST_GPIO // {{{

#if AVR_TEST == 0
namespace Gpio {
	static const uint8_t testcode = 'P';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			// TODO? Add available ports here.
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	Pbh	Set pin of port P bit b to high output
		//	Pbl	Set pin of port P bit b to low output
		//	Pbi	Set pin of port P bit b to pullup input
		//	Pbx	Set pin of port P bit b to no-pullup input
		//	Pbr	Read pin of port P bit b
		if (len != 2)
			return false;
		// Port is in cmd.
		uint8_t b = Test::read_digit(0);
		uint8_t op = Test::rx_read(1);
		uint8_t p = make_pin(cmd - 'A', b);
		if (!check_pin(p))
			return false;
		switch (op) {
		case 'h':
			write(p, true);
			break;
		case 'l':
			write(p, false);
			break;
		case 'i':
			input(p, true);
			break;
		case 'x':
			input(p, false);
			break;
		case 'r':
		{
			bool d = read(p);
			Test::tx(testcode);
			Test::tx(d ? '1' : '0');
			Test::tx('\n');
			break;
		}
		default:
			return false;
		}
		return true;
	}
}

#else 

namespace Gpio {
	static const uint8_t testcode = 0;
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		(void)&part;
		(void)&cmd;
		(void)&len;
		return false;
	}
}
#endif

#endif // }}}

#endif

// vim: set foldmethod=marker :
