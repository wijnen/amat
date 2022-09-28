// External interrupts
// Both int* and pcint*.

/** @file
# External interrupts, including pin change interrupts
Example: inform host of state on two pins.
```
#define DBG_ENABLE

#include <avr-ll.hh>

void setup() {
	// Enable Int0 for any change
	Int::set_type(0, Int::TOGGLE);

	// Enable Pcint10
	Pcint::enable_group(1);	// Pcint pins 8 through 15
	Pcint::enable(10);
}

ISR(INT0_vect) {
	// Use pin as defined by MCU to get current state.
	dbg("Int0 pin changed state. Now: #", Gpio::read(INT0_PIN));
}

ISR(PCINT1_vect) {
	// Use pin as defined by MCU to get current state.
	dbg("Pcint10 changed state. Now: #", Gpio::read(Pcint::get_pin(10)));
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

/// @cond
// Convenience definitions. {{{
#ifdef EICRB
#define _AVR_EICR(n) (n < 4 ? EICRA : EICRB)
#elif defined(EICRA)
#define _AVR_EICR(n) EICRA
#else
#define _AVR_EICR(n) MCUCR
#endif

#ifndef EIMSK
#define EIMSK GIMSK
#endif

#ifndef PCINT0_vect
#define PCINT0_vect PCINT_vect
#endif

#if defined(PCMSK2)
#define _AVR_PCMSK(n) (n < 8 ? PCMSK0 : n < 16 ? PCMSK1 : PCMSK2)
#elif defined(PCMSK1)
#define _AVR_PCMSK(n) (n < 8 ? PCMSK0 PCMSK1)
#elif defined(PCMSK0)
#define _AVR_PCMSK(n) PCMSK0
#else
#define _AVR_PCMSK(n) PCMSK
#endif

#ifndef PCICR
#define PCICR GIMSK
#endif

#ifndef PCIFR
#define PCIFR EIFR
#endif
// }}}

/// @endcond

/// External interrupts
namespace Int {

	/// Trigger types
	enum Type { // {{{
		/// Trigger on low level
		LOW = 0,
		/// Trigger on change
		TOGGLE = 1,
		/// Trigger on falling edge
		FALLING = 2,
		/// Trigger on rising edge
		RISING = 3
	}; // }}}

	/// Set the trigger type for an interrupt.
	static inline void set_type(uint8_t num, Type type) { // {{{
		volatile uint8_t *eicr = &_AVR_EICR(num);
		uint8_t shift = 2 * (num & 3) + ISC00;
		*eicr = (*eicr & ~(3 << shift)) | (type << shift);
	} // }}}

	/// Enable the interrupt.
	/**
	 * ISR(INT<num>_vect) must be defined if this is used.
	 */
	static inline void enable(uint8_t num) { EIMSK |= (1 << num) << INT0; }

	/// Disable the interrupt.
	static inline void disable(uint8_t num) { EIMSK &= ~((1 << num) << INT0); }

	/// Clear the interrupt flag.
	static inline void clear(uint8_t num) { EIFR |= (1 << num) << INTF0; }
}

/// Pin change interrupts
/**
 * There are 8 pin change interrupts in a group. Every group has a separate
 * flag for enabling the interrupt, and every pin has one as well.
 *
 * There is no edge selection: if an interrupt is enabled for a pin, it
 * triggers on any change.
 */
namespace Pcint {
	/// Enable the interrupt for a group of pin change interrupts.
	/**
	 * ISR(PCINT<group_num>_vect) must be defined if this is used.
	 */
	static inline void enable_group(uint8_t group_num) { PCICR |= 1 << group_num; }

	/// Disable the interrupt for a group of pin change interrupts.
	static inline void disable_group(uint8_t group_num) { PCICR &= ~(1 << group_num); }

	/// Clear the flags for the interrupt of a group.
	static inline void clear_group(uint8_t group_num) { PCIFR |= 1 << group_num; }

	/// Enable the interrupt for a single pin.
	/**
	 * This can be used to make the interrupt flag of the pin's group
	 * trigger when this pin changes.
	 *
	 * It does not generate an actual interrupt, for that, enable_group()
	 * must be called.
	 */
	static inline void enable(uint8_t num) { _AVR_PCMSK(num) |= 1 << (num & 0x7); }

	/// Disable the interrupt for a single pin.
	static inline void disable(uint8_t num) { _AVR_PCMSK(num) &= ~(1 << (num & 0x7)); }

	/// Get pin id for Pcint pin, or 0xff if it does not exist.
	static inline uint8_t get_pin(uint8_t pcint_id) { return pcint_id >= _AVR_NUM_PCINT_PINS ? 0xff : ((uint8_t [])_AVR_PCINT_IDS)[pcint_id]; }
}

#ifdef AVR_TEST_INT // {{{

#if AVR_TEST == 0
namespace Int {
	static const uint8_t testcode = 'I';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) { // {{{
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('t');
			Test::tx('e');
			Test::tx('d');
			Test::tx('c');
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	tNX	Set type for N to X
		//	eN	Enable N
		//	dN	Disable N
		//	cN	Clear flag N
		if (len != (cmd == 't' ? 2 : 1))
			return false;
		uint8_t N = Test::read_digit(0);
		if (!(_AVR_INT_MASK & (1 << N)))
			return false;
		switch (cmd) {
			case 't':
			{
				Type T = static_cast <Type>(Test::read_digit(1));
				if (T > 3)
					return false;
				set_type(N, T);
				break;
			}
			case 'e':
				enable(N);
				break;
			case 'd':
				disable(N);
				break;
			case 'c':
				clear(N);
				break;
			default:
				return false;
		}
		return true;
	} // }}}
}

// Define ISRs. {{{
#define _AVR_INT_ISR(N) \
	ISR(INT ## N ## _vect) { \
		Test::tx(Int::testcode); \
		Test::tx('0' + N); \
		Test::tx('\n'); \
	}

#ifdef INT0_vect
_AVR_INT_ISR(0)
#endif

#ifdef INT1_vect
_AVR_INT_ISR(1)
#endif

#ifdef INT2_vect
_AVR_INT_ISR(2)
#endif

#ifdef INT3_vect
_AVR_INT_ISR(3)
#endif

#ifdef INT4_vect
_AVR_INT_ISR(4)
#endif

#ifdef INT5_vect
_AVR_INT_ISR(5)
#endif

#ifdef INT6_vect
_AVR_INT_ISR(6)
#endif

#ifdef INT7_vect
_AVR_INT_ISR(7)
#endif
// }}}

#else 

namespace Int {
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

#ifdef AVR_TEST_PCINT // {{{

#if AVR_TEST == 0
namespace Pcint {
	static const uint8_t testcode = 'V';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) { // {{{
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('E');
			Test::tx('D');
			Test::tx('C');
			Test::tx('e');
			Test::tx('e');
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	EG	Enable group G.
		//	DG	Disable group G.
		//	CG	Clear flag for group G.
		//	eN	Enable pcint N.
		//	dN	Disable pcint N.
		if (len != 1)
			return false;
		uint8_t N = Test::read_digit(0);
		switch (cmd) {
		case 'E':
			if (N * 8 >= _AVR_NUM_PCINT_PINS)
				return false;
			enable_group(N);
			break;
		case 'D':
			if (N * 8 >= _AVR_NUM_PCINT_PINS)
				return false;
			disable_group(N);
			break;
		case 'C':
			if (N * 8 >= _AVR_NUM_PCINT_PINS)
				return false;
			clear_group(N);
			break;
		case 'e':
			if (N >= _AVR_NUM_PCINT_PINS)
				return false;
			enable(N);
			break;
		case 'd':
			if (N >= _AVR_NUM_PCINT_PINS)
				return false;
			disable(N);
			break;
		default:
			return false;
		}
		return true;
	} // }}}
}

// Define ISRs. {{{
#define _AVR_PCINT_ISR(N) \
	ISR(PCINT ## N ## _vect) { \
		Test::tx(Pcint::testcode); \
		Test::tx('0' + N); \
		Test::tx('\n'); \
	}

_AVR_PCINT_ISR(0)

#ifdef PCIE1
_AVR_PCINT_ISR(1)
#endif

#ifdef PCIE2
_AVR_PCINT_ISR(2)
#endif
// }}}

#else 

namespace Pcint {
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

// vim: set foldmethod=marker :
