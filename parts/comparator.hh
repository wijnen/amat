// Analog comparator

#ifndef _AVR_COMPARATOR_HH
#define _AVR_COMPARATOR_HH

/** @file
@author Bas Wijnen <wijnen@debian.org>
# Analog Comparator
Example code using the comparator and its link to the input capture.
Notice that their interrupts are independent, so it is possible to trigger each
on a different edge. However, the input capture interrupt does not support to
trigger on every toggle, so if that is needed, setup_capt1() must be called for
every interrupt to flip the selected edge.
```
#define DBG_ENABLE

#include <avr-ll.hh>

void setup() {
	Counter::enable1();
	// Capture falling edge.
	Counter::setup_capt1(false);
	// Enable capture interrupt.
	Counter::enable_capt1();
	// Enable comparator and its interrupt on the rising edge.
	Comparator::enable(Comparator::RISING);
	Comparator::enable_interrupt();
}

ISR(ANALOG_COMP_vect) {
	// Rising edge, so AIN0 (positive input) is now greater.
	dbg("AIN1 < AIN0");
}

ISR(TIMER1_CAPT_vect) {
	// Falling edge, so AIN0 (positive input) is now smaller.
	dbg("AIN1 > AIN0 at *", Counter::get_icr1());
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

/// @cond

#ifndef ANALOG_COMP_vect
#define ANALOG_COMP_vect ANA_COMP_vect
#endif

#ifndef DIDR1
#define DIDR1 DIDR
#endif

/// @endcond

#ifdef DOXYGEN
#define _AVR_ADC_HH
#endif

/// Analog comparator
namespace Comparator {

	/// Trigger method
	enum Edge {
		/// Trigger on any edge
		TOGGLE = 0,
		/// Trigger on falling edge
		FALLING = 2,
		/// Trigger on rising edge
		RISING = 3
	};

	// Reference is AIN0 or bandgap.
	/// Select reference voltage source
	/**
	 * The reference voltage is either taken from the AIN0 pin, or from the internal bandgap voltage.
	 */
	static inline void bandgap(bool enable) { // {{{
		// The bandgap reference voltage is 1.1 V.
		if (enable)
			ACSR |= _BV(ACBG);
		else
			ACSR &= ~_BV(ACBG);
	} // }}}
#ifdef _AVR_ADC_HH
	// Measurement is AIN1 or ADC input.
	/// Set measurement voltage source to ADC and select source pin.
	/**
	 * The measurement voltage is either taken from the AIN1 pin, or from the ADC muxer.
	 * This function enables the ADC muxer source and selects the pin.
	 */
	static inline void enable_adc(uint8_t src) { // {{{
		ADCSRA &= ~_BV(ADEN);
		ADCSRB |= _BV(ACME);
		ADMUX = src;
	} // }}}

	/// Set measurement voltage source to AIN1 pin.
	/**
	 * The measurement voltage is either taken from the AIN1 pin, or from the ADC muxer.
	 * This function enables the AIN1 pin as the source.
	 */
	static inline void disable_adc() { // {{{
		ADCSRB &= ~_BV(ACME);
	} // }}}
#endif

	/// Enable the analog comparator
	/**
	 * @param edge: trigger source
	 */
	static inline void enable(Edge edge = TOGGLE) { // {{{
		ACSR &= ~(_BV(ACD) | _BV(ACIS1) | _BV(ACIS0));
		ACSR |= edge;
		DIDR1 = _BV(AIN0D)
#ifdef AIN1D
			| _BV(AIN1D)
#endif
			;
	} // }}}

	/// Disable the analog comparator
	static inline void disable() { // {{{
		ACSR = _BV(ACD);
		DIDR1 = 0;
#ifdef _AVR_ADC_HH
		ADCSRB &= ~_BV(ACME);
#endif
	} // }}}

	/// Use comparator output to trigger the input capture of counter 1.
	static inline void icp(bool enable) { // {{{
		if (enable)
			ACSR |= _BV(ACIC);
		else
			ACSR &= ~_BV(ACIC);
	} // }}}

	/// Return current output of analog comparator.
	static inline bool read() { // {{{
		return ACSR & _BV(ACO);
	} // }}}

	/// Enable the interrupt.
	/**
	 * ISR(ANALOG_COMP_vect) must be defined if this is used.
	 */
	static inline void enable_interrupt() { ACSR |= _BV(ACIE) | _BV(ACI); }

	/// Disable the interrupt.
	static inline void disable_interrupt() { ACSR &= ~_BV(ACIE); }
}

#ifdef AVR_TEST_COMPARATOR // {{{

#if AVR_TEST == 0
namespace Comparator {
	static const uint8_t testcode = 'C';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('r');
			Test::tx('C');
			Test::tx('x');
			Test::tx('E');
			Test::tx('B');
			Test::tx('A');
			Test::tx('n');
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	r	read state
		//	CX	enable (1) or disable (0) icp
		//	x	disable comparator
		//	EX	enable comparator for edge X
		//	BX	enable (1) or disable (0) bandgap reference
		//	AX	enable adc with source X
		//	n	disable adc source (use AIN1)
		if (cmd >= 'a' && cmd <= 'z') {
			if (len != 0)
				return false;
		}
		else if (len != 1)
			return false;
		switch (cmd) {
		case 'r':
		{
			bool state = read();
			Test::tx(testcode);
			Test::tx(state ? '1' : '0');
			Test::tx('\n');
			break;
		}
		case 'x':
			disable();
			break;
#ifdef _AVR_ADC_HH
		case 'n':
			disable_adc();
			break;
#endif
		default:
		{
			uint8_t arg = Test::read_digit(0);
			if (arg == 0xff)
				return false;
			switch (cmd) {
			case 'C':
				if (arg > 1)
					return 0;
				icp(arg > 0);
				break;
			case 'E':
				enable(static_cast <Edge>(arg));
				break;
			case 'B':
				if (arg > 1)
					return 0;
				bandgap(arg > 0);
				break;
#ifdef _AVR_ADC_HH
			case 'A':
				enable_adc(arg);
				break;
#endif
			default:
				return false;
			}
		}
		}
		return true;
	}
}

ISR(ANALOG_COMP_vect) {
	Test::tx(Comparator::testcode);
	Test::tx('\n');
}

#else

namespace Comparator {
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
