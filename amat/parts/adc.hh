// Analog to digital converter

/** @file
# Analog to digital converter.

Example of interrupt-based measurement, triggered by a command on the serial port:
```
#define USART_ENABLE_RX 10
#define DBG_ENABLE
#include <amat.hh>

void setup() {
	Adc::set_source(Adc::SRC_A0);
	Adc::enable_int();
}

void usart_rx(uint8_t c, uint8_t len) {
	// Ignore actual input, just start a measurement.
	(void)&c;
	Usart::rx_pop(len);
	Adc::single();
}

ISR(ADC_vect) {
	uint16_t value = Adc::read(false);
	dbg("*", value);
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

#ifndef _AVR_ADC_HH
#define _AVR_ADC_HH

#ifdef DOXYGEN
#define _AVR_HAVE_REF_2_56V
#endif

/// @cond
// Find number of pins and build pin id array for test. {{{
#ifdef PIN_ADC15
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 15
#		define _AVR_ADC15 PIN_ADC15
#	else
#		define _AVR_ADC15 PIN_ADC15,
#	endif
#	define _AVR_ADC15_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC15 0xff,
#	else
#		define _AVR_ADC15
#	endif
#	define _AVR_ADC15_N 0
#endif

#ifdef PIN_ADC14
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 14
#		define _AVR_ADC14 PIN_ADC14
#	else
#		define _AVR_ADC14 PIN_ADC14,
#	endif
#	define _AVR_ADC14_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC14 0xff,
#	else
#		define _AVR_ADC14
#	endif
#	define _AVR_ADC14_N 0
#endif

#ifdef PIN_ADC13
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 13
#		define _AVR_ADC13 PIN_ADC13
#	else
#		define _AVR_ADC13 PIN_ADC13,
#	endif
#	define _AVR_ADC13_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC13 0xff,
#	else
#		define _AVR_ADC13
#	endif
#	define _AVR_ADC13_N 0
#endif

#ifdef PIN_ADC12
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 12
#		define _AVR_ADC12 PIN_ADC12
#	else
#		define _AVR_ADC12 PIN_ADC12,
#	endif
#	define _AVR_ADC12_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC12 0xff,
#	else
#		define _AVR_ADC12
#	endif
#	define _AVR_ADC12_N 0
#endif

#ifdef PIN_ADC11
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 11
#		define _AVR_ADC11 PIN_ADC11
#	else
#		define _AVR_ADC11 PIN_ADC11,
#	endif
#	define _AVR_ADC11_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC11 0xff,
#	else
#		define _AVR_ADC11
#	endif
#	define _AVR_ADC11_N 0
#endif

#ifdef PIN_ADC10
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 10
#		define _AVR_ADC10 PIN_ADC10
#	else
#		define _AVR_ADC10 PIN_ADC10,
#	endif
#	define _AVR_ADC10_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC10 0xff,
#	else
#		define _AVR_ADC10
#	endif
#	define _AVR_ADC10_N 0
#endif

#ifdef PIN_ADC9
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 9
#		define _AVR_ADC9 PIN_ADC9
#	else
#		define _AVR_ADC9 PIN_ADC9,
#	endif
#	define _AVR_ADC9_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC9 0xff,
#	else
#		define _AVR_ADC9
#	endif
#	define _AVR_ADC9_N 0
#endif

#ifdef PIN_ADC8
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 8
#		define _AVR_ADC8 PIN_ADC8
#	else
#		define _AVR_ADC8 PIN_ADC8,
#	endif
#	define _AVR_ADC8_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC8 0xff,
#	else
#		define _AVR_ADC8
#	endif
#	define _AVR_ADC8_N 0
#endif

#ifdef PIN_ADC7
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 7
#		define _AVR_ADC7 PIN_ADC7
#	else
#		define _AVR_ADC7 PIN_ADC7,
#	endif
#	define _AVR_ADC7_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC7 0xff,
#	else
#		define _AVR_ADC7
#	endif
#	define _AVR_ADC7_N 0
#endif

#ifdef PIN_ADC6
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 6
#		define _AVR_ADC6 PIN_ADC6
#	else
#		define _AVR_ADC6 PIN_ADC6,
#	endif
#	define _AVR_ADC6_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC6 0xff,
#	else
#		define _AVR_ADC6
#	endif
#	define _AVR_ADC6_N 0
#endif

#ifdef PIN_ADC5
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 5
#		define _AVR_ADC5 PIN_ADC5
#	else
#		define _AVR_ADC5 PIN_ADC5,
#	endif
#	define _AVR_ADC5_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC5 0xff,
#	else
#		define _AVR_ADC5
#	endif
#	define _AVR_ADC5_N 0
#endif

#ifdef PIN_ADC4
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 4
#		define _AVR_ADC4 PIN_ADC4
#	else
#		define _AVR_ADC4 PIN_ADC4,
#	endif
#	define _AVR_ADC4_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC4 0xff,
#	else
#		define _AVR_ADC4
#	endif
#	define _AVR_ADC4_N 0
#endif

#ifdef PIN_ADC3
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 3
#		define _AVR_ADC3 PIN_ADC3
#	else
#		define _AVR_ADC3 PIN_ADC3,
#	endif
#	define _AVR_ADC3_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC3 0xff,
#	else
#		define _AVR_ADC3
#	endif
#	define _AVR_ADC3_N 0
#endif

#ifdef PIN_ADC2
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 2
#		define _AVR_ADC2 PIN_ADC2
#	else
#		define _AVR_ADC2 PIN_ADC2,
#	endif
#	define _AVR_ADC2_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC2 0xff,
#	else
#		define _AVR_ADC2
#	endif
#	define _AVR_ADC2_N 0
#endif

#ifdef PIN_ADC1
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 1
#		define _AVR_ADC1 PIN_ADC1
#	else
#		define _AVR_ADC1 PIN_ADC1,
#	endif
#	define _AVR_ADC1_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC1 0xff,
#	else
#		define _AVR_ADC1
#	endif
#	define _AVR_ADC1_N 0
#endif

#ifdef PIN_ADC0
#	ifndef ADC_LAST_PIN
#		define ADC_LAST_PIN 0
#		define _AVR_ADC0 PIN_ADC0
#	else
#		define _AVR_ADC0 PIN_ADC0,
#	endif
#	define _AVR_ADC0_N 1
#else
#	ifdef ADC_LAST_PIN
#		define _AVR_ADC0 0xff,
#	else
#		define _AVR_ADC0
#	endif
#	define _AVR_ADC0_N 0
#endif

#define ADC_NUM_PINS (_AVR_ADC0_N + _AVR_ADC1_N + _AVR_ADC2_N + _AVR_ADC3_N + _AVR_ADC4_N + _AVR_ADC5_N + _AVR_ADC6_N + _AVR_ADC7_N + _AVR_ADC8_N + _AVR_ADC9_N + _AVR_ADC10_N + _AVR_ADC11_N + _AVR_ADC12_N + _AVR_ADC13_N + _AVR_ADC14_N + _AVR_ADC15_N)

#define ADC_PIN_MASK ( \
		(_AVR_ADC0_N << 0) | \
		(_AVR_ADC1_N << 1) | \
		(_AVR_ADC2_N << 2) | \
		(_AVR_ADC3_N << 3) | \
		(_AVR_ADC4_N << 4) | \
		(_AVR_ADC5_N << 5) | \
		(_AVR_ADC6_N << 6) | \
		(_AVR_ADC7_N << 7) | \
		(_AVR_ADC8_N << 8) | \
		(_AVR_ADC9_N << 9) | \
		(_AVR_ADC10_N << 10) | \
		(_AVR_ADC11_N << 11) | \
		(_AVR_ADC12_N << 12) | \
		(_AVR_ADC13_N << 13) | \
		(_AVR_ADC14_N << 14) | \
		(_AVR_ADC15_N << 15) \
	)

#ifdef ADC_ENABLE_PIN_LIST
uint8_t adc_pin_list[ADC_NUM_PINS] = {
#ifdef PIN_ADC0
	PIN_ADC0,
#endif
#ifdef PIN_ADC1
	PIN_ADC1,
#endif
#ifdef PIN_ADC2
	PIN_ADC2,
#endif
#ifdef PIN_ADC3
	PIN_ADC3,
#endif
#ifdef PIN_ADC4
	PIN_ADC4,
#endif
#ifdef PIN_ADC5
	PIN_ADC5,
#endif
#ifdef PIN_ADC6
	PIN_ADC6,
#endif
#ifdef PIN_ADC7
	PIN_ADC7,
#endif
#ifdef PIN_ADC8
	PIN_ADC8,
#endif
#ifdef PIN_ADC9
	PIN_ADC9,
#endif
#ifdef PIN_ADC10
	PIN_ADC10,
#endif
#ifdef PIN_ADC11
	PIN_ADC11,
#endif
#ifdef PIN_ADC12
	PIN_ADC12,
#endif
#ifdef PIN_ADC13
	PIN_ADC13,
#endif
#ifdef PIN_ADC14
	PIN_ADC14,
#endif
#ifdef PIN_ADC15
	PIN_ADC15,
#endif
};
#endif

#define _AVR_ADC_IDS { \
	_AVR_ADC0 \
	_AVR_ADC1 \
	_AVR_ADC2 \
	_AVR_ADC3 \
	_AVR_ADC4 \
	_AVR_ADC5 \
	_AVR_ADC6 \
	_AVR_ADC7 \
	_AVR_ADC8 \
	_AVR_ADC9 \
	_AVR_ADC10 \
	_AVR_ADC11 \
	_AVR_ADC12 \
	_AVR_ADC13 \
	_AVR_ADC14 \
	_AVR_ADC15 \
}
// }}}
/// @endcond

/// Analog to Digital Converter
namespace Adc {
	/// @cond
	uint8_t const _ps = (F_CPU < 200000 * 2 ? 1 : F_CPU < 200000 * 4 ? 2 : F_CPU < 200000 * 8 ? 3 : F_CPU < 200000 * 16 ? 4 : F_CPU < 200000 * 32 ? 5 : F_CPU < 200000 * 64 ? 6 : 7) << ADPS0;
	/// @endcond

	enum Source;	// defined in the mcu definition.
#ifdef DOXYGEN
	/// Source options for ADC.
	/**
	 * Actual members depend on the MCU. Values shown here are only examples.
	 */
	enum Source {
		/// Single ended input
		SRC_A0,
		/// Differential input
		SRC_A1_A0,
		/// Differential input with a multiplier
		SRC_A1_A0_200x,
		/// 1.1 V bandgap reference voltage
		SRC_1V1,
		/// 0V reference voltage
		SRC_GND,
		/// Internal temperature sensor
		SRC_TEMP
	};
#endif
	/// Reference options for ADC.
	enum Ref { // {{{
		/// External AREF pin
		REF_AREF = 0,
		/// External AVCC pin
		REF_AVCC = 1,
#ifdef _AVR_HAVE_REF_2_56V
		/// 1.1V internal bandgap voltage
		REF_1_1V = 2,
		/// 2.56V internal bandgap voltage
		REF_2_56V = 3
#else
		REF_1_1V = 3
#endif
	}; // }}}
	/// Trigger options for continuous ADC measurements.
	enum Trigger { // {{{
		/// Free running; continuous measurements.
		TRIGGER_FREE = 0,
		/// Analog comparator
		TRIGGER_AC = 1,
		/// External interrupt 0
		TRIGGER_INT0 = 2,
		/// Output compare A of Counter 0
		TRIGGER_OC0A = 3,
		/// Overflow of Counter 0
		TRIGGER_OVF0 = 4,
		/// Output compare B of Counter 1
		TRIGGER_OC1B = 5,
		/// Overflow of Counter 1
		TRIGGER_OVF1 = 6,
		/// Input capture of Counter 1
		TRIGGER_CAPT = 7
	}; // }}}

	/// Set measurement pins and reference voltage.
	/**
	 * Set measurement pins and reference voltage. Available sources are
	 * defined in the mcu source file and are of the form SRC_Ax for single
	 * ended source x, SRC_Ap_An for differential source p-n and
	 * SRC_Ap_An_fx for a differential source with a factor f. Additionally,
	 * if available, there are SRC_TEMP for a temperature measurement,
	 * SRC_1V1 for the bandgap reference voltage and SRC_GND for the ground.
	 * The options for ref are REF_AREF, REF_AVCC, REF_1_1V, and (if
	 * available) REF_2_56V. The last two are internal voltage references.
	 */
	static inline void set_source(Source target = SRC_A0, Ref ref = REF_AVCC) { // {{{
		ADMUX = (ADMUX & _BV(ADLAR)) | (ref << REFS0) | (target << MUX0);
	} // }}}

	/// Left- (or right-) adjust measurement result.
	/**
	 * If set, the result of a measurement is left adjusted. If not, it is
	 * right adjusted.
	 */
	static inline void left_adjust(bool left = true) { // {{{
		ADMUX = (ADMUX & ~_BV(ADLAR)) | (left ? _BV(ADLAR) : 0);
	} // }}}

	/// Start a single measurement, then stop measuring.
	/**
	 * This does not wait for the measurement to complete.
	 */
	static inline void single() { // {{{
		ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIF) | _ps | (ADCSRA & _BV(ADIE));
	} // }}}

	/// Read the result of the last measurement.
	/**
	 * If wait is true, first wait for the current measurement to complete.
	 */
	static inline uint16_t read(bool wait = true) { // {{{
		if (wait)
			while (ADCSRA & _BV(ADSC)) {}
		uint8_t l = ADCL;
		uint8_t h = ADCH;
		return (h << 8) | l;
	} // }}}

	/// Do a single measurement, wait for completion and return result.
	/**
	 * Shorter notation to first run single(), then read(true).
	 */
	static inline uint16_t single_block() { // {{{
		single();
		return read();
	} // }}}

	/// Set the ADC for automatic measuring.
	/**
	 * Autmatic triggering stops when single(), stop() or disable() is called.
	 */
	static inline void continuous(Trigger trigger) { // {{{
		if (trigger == TRIGGER_FREE) {
			ADCSRB = (ADCSRB & _BV(ACME)) | (trigger << ADTS0);
			ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIF) | _ps | (ADCSRA & _BV(ADIE));
		}
		else {
			ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIF) | _ps | (ADCSRA & _BV(ADIE));
			ADCSRB = (ADCSRB & _BV(ACME)) | (trigger << ADTS0);
		}
	} // }}}

	/// Stop automatic triggering of the ADC.
	/**
	 * After continuous(), stop triggering the ADC. If a measurement is
	 * currently running, it is completed.
	 */
	static inline void stop() { // {{{
		ADCSRA = _BV(ADEN) | _BV(ADIF) | _ps;
	} // }}}

	/// Disable the ADC.
	/**
	 * A currently running measurement is aborted.
	 */
	static inline void disable() { // {{{
		ADCSRA = _BV(ADIF) | _ps;
	} // }}}

	/// Enable the interrupt.
	/**
	 * This also clears the pending interrupt.
	 *
	 * ISR(ADC_vect) needs to be defined if this is called.
	 */
	static inline void enable_int() { ADCSRA |= _BV(ADIE); }

	/// Disable the interrupt.
	/**
	 * This also clears the pending interrupt.
	 */
	static inline void disable_int() { ADCSRA &= ~_BV(ADIE); }

#if defined(PRR0) || defined(DOXYGEN)

	/// Disable power to the ADC.
	static inline void off() { PRR0 |= _BV(PRADC); }

	/// Enable power to the ADC.
	static inline void on() { PRR0 &= ~_BV(PRADC); }
#endif
}

#ifdef AVR_TEST_ADC // {{{

#if AVR_TEST_INDEX == 0
namespace Adc {
	static const uint8_t testcode = 'A';
	struct ADC_Source_info {
		uint8_t pos;
		uint8_t neg;
		uint8_t factor;
	};
	static const ADC_Source_info adc_map[_AVR_ADC_MAP_SIZE] = _AVR_ADC_MAP;

	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('s');
			Test::tx('n');
			Test::tx('m');
			Test::tx('l');
			Test::tx('i');
			Test::tx('r');
			Test::tx('b');
			Test::tx('c');
			Test::tx('x');
			Test::tx('z');
			Test::tx('\n');
			return true;
		}
		// Commands:
		// 	sXXY	set source to XX (hardward code, decoded with m) and ref to Y.
		//	n	return number of sources.
		//	mXX	return map info for source XX: positive source, negative source, factor.
		//	lX	left adjust (0 = false, 1 = true).
		//	i	single measurement with interrupt response.
		//	r	wait for data to be ready, then return it; does not start measurement.
		//	b	start single measurement, then read result (blocking).
		//	cT	start continuous measurement triggered by T; respond on interrupt.
		//	x	stop continuous (or other pending) measurements.
		//	z	disable adc (and interrupt).
		switch (cmd) {
		case 's':
		{
			if (len != 3)
				return false;
			bool ok = true;
			uint8_t src = Test::read_byte(0, ok);
			uint8_t ref = Test::read_digit(2);
			if (!ok || ref == 0xff)
				return false;
			set_source(static_cast <Adc::Source>(src), static_cast <Adc::Ref>(ref));
			break;
		}
		case 'm':
		{
			if (len != 2)
				return false;
			bool ok = true;
			uint8_t src = Test::read_byte(0, ok);
			if (!ok || src >= _AVR_ADC_MAP_SIZE)
				return false;
			Test::tx(testcode);
			Test::send_byte(adc_map[src].pos);
			Test::send_byte(adc_map[src].neg);
			Test::send_byte(adc_map[src].factor);
			Test::tx('\n');
			break;
		}
		case 'l':
		{
			if (len != 1)
				return false;
			uint8_t value = Test::rx_read(0);
			if (value != '0' && value != '1')
				return false;
			left_adjust(value != '0');
			break;
		}
		case 'c':
		{
			if (len != 1)
				return false;
			uint8_t trigger = Test::read_digit(0);
			if (trigger == 0xff)
				return false;
			enable_int();
			continuous(static_cast <Adc::Trigger>(trigger));
			break;
		}
		default:
			if (len != 0)
				return false;
			switch (cmd) {
			case 'n':
				Test::tx(testcode);
				Test::send_byte(_AVR_ADC_MAP_SIZE);
				Test::tx('\n');
				break;
			case 'i':
				enable_int();
				single();
				break;
			case 'r':
			{
				uint16_t data = read();
				Test::tx(testcode);
				Test::send_byte(data >> 8);
				Test::send_byte(data & 0xff);
				Test::tx('\n');
				break;
			}
			case 'b':
			{
				uint16_t data = single_block();
				Test::tx(testcode);
				Test::send_byte(data >> 8);
				Test::send_byte(data & 0xff);
				Test::tx('\n');
				break;
			}
			case 'x':
				stop();
				break;
			case 'z':
				disable();
				break;
			default:
				return false;
			}
			break;
		}
		return true;
	}
}

ISR(ADC_vect) {
	uint16_t data = Adc::read(false);
	Test::tx(Adc::testcode);
	Test::send_byte(data >> 8);
	Test::send_byte(data & 0xff);
	Test::tx('\n');
}

#else

namespace Adc {
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
