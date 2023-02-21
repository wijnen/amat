// Clock

#ifndef _AVR_CLOCK_HH
#define _AVR_CLOCK_HH

/** @file
# System clock configuration.
Example of calibrating the clock at startup
```
#define DBG_ENABLE

#include <amat.hh>

void setup() {
	// Disable usart to allow the pin to be used for calibration.
	// It wouldn't work with an uncalibrated clock anyway.
	Usart::disable();

	// Calibrate the clock.
	Clock::calibrate();

	// Enable the usart and send a response to let the host know we are calibrated.
	Usart::enable();
	dbg("Calibrated");
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

namespace Clock {
	// Low level.
	// OSCCAL: Internal RC-oscillator calibration
	// Bit 7 is special (on devices that have it), but the standard calibration algorithm works fine.

	/// Get calibration value for internal RC oscillator.
	/**
	 * @sa calibrate()
	 */
	static inline uint8_t get_calibration() { // {{{
		return OSCCAL;
	} // }}}

	/// Set calibration value for internal RC oscillator.
	/**
	 * @sa calibrate()
	 */
	static inline void set_calibration(uint8_t calibration) { // {{{
		OSCCAL = calibration;
	} // }}}

	// CLKPR: Clock prescaler

	/// Get prescaler value for the system clock.
	/**
	 * If this is not zero, the system clock is running slower than the clock source.
	 * This can be useful when the supply voltage is too low to support the external clock.
	 * The divisor is 2 to the power of the value, so if the value is 3, the divisor is 8.
	 */
	static inline uint8_t get_prescaler() { // {{{
		return (CLKPR & ~_BV(CLKPCE)) >> CLKPS0;
	} // }}}

	/// Set prescaler value for the system clock.
	/**
	 * @sa get_prescaler()
	 */
	static inline void set_prescaler(uint8_t division_log2) { // {{{
		asm volatile(
			"\t"    "in __tmp_reg__, __SREG__"      "\n"
			"\t"	"cli"				"\n"
			"\t"	"sts %[clkpr], %[enable]"	"\n"
			"\t"	"sts %[clkpr], %[value]"	"\n"
			"\t"    "out __SREG__, __tmp_reg__"     "\n"
			::
			[clkpr] "" (_SFR_MEM_ADDR(CLKPR)),
			[enable] "r" (_BV(CLKPCE)),
			[value] "r" (division_log2 << CLKPS0)
		);
	} // }}}

	// Binary search for perfect calibration value.
	/// @cond
	// Internals. {{{
	// bit needs to be defined by the caller. Its value must only be changed by these functions.
	// It's not needed after the calibration is completed.
	static inline void start_calibration(uint8_t &bit) {
		// This function must be called first, to initialize the variables.
#ifdef CAL7
		// There are 8 bits.
		bit = 7;
		set_calibration(0x7f);
#else
		// There are only 7 bits.
		bit = 6;
		set_calibration(0x3f);
#endif
	}

	static inline bool calibrate_iteration(bool faster, uint8_t &bit) {
		// Single step in binary search for perfect calibration.
		// Procedure requires a series of events that signal whether the clock is currently too slow or too fast.
		// Typically calibration is done after 8 events, but it may vary; there is no hard upper bound.
		// After every event, this function is called. It returns true when the calibration is complete.
		// faster: if true, the device needs to be faster (higher clock frequency).
		// If it is exactly right, faster must be set to false.
		uint8_t current = get_calibration();
		if (faster)
			current |= 1 << bit;
		if (bit == 0)
			return true;
		bit -= 1;
		current &= ~(1 << bit);
		return false;
	}
	// }}}
	/// @endcond

	//

#if defined(_AVR_COUNTER1_HH) || defined(DOXYGEN)
	/// Calibrate the oscillator
	/**
	 * Repeatedly measures pulses on RXD pin to calibrate OSCCAL.
	 * F_CPU is used to determine expected pulse length.
	 *
	 * The host must send continuous 8-bit '\0', which is 9 bits (including
	 * start bit) at 4800 baud, so @f$\frac{4800}9 = 533\frac13@f$ pulses per second.
	 *
	 * F_CPU will usually be 8 MHz, which means pulse_duration will be
	 * exactly 15000 clock pulses.
	 *
	 * pin must be in Gpio mode when this function is called, so the
	 * serial port must not be enabled. There is no requirement that
	 * pin is actually connected to the device's serial port, but it is
	 * usually desired because it must be connected to the serial port on
	 * the host, so after calibration the serial port can be used in that
	 * case.
	 *
	 * ### Timing:
	 *
	 * The edge checking code is sbi[cs] followed by rjmp. This loop takes
	 * 3 cycles. The loop exits when the pin changes anywhere in that loop.
	 *
	 * This means that in the extreme case, the start edge happens
	 * immediately after checking and the end edge right before. The
	 * measured duration is then almost 3 cycles too long.
	 *
	 * Similarly it is at most 3 cycles too short when the errors are
	 * reversed.
	 *
	 * The measurement is more accurate when the pulse duration is longer,
	 * because in that case the 3 cycles are divided by a larger number.
	 * But if it is too long, the counter will overflow.
	 *
	 * For this reason, 4800 baud is used. It is the lowest (common) rate
	 * which does not cause an overflow for its full period (10 bits) even
	 * at the highest allowed frequency of 20 MHz. (The actual lowest
	 * allowed rate is \f$\frac{20\cdot10^6\mathrm{pulses/second}\cdot 10\mathrm{~bits/period}}{2^{16}\mathrm{~pulses/period}} = 3052\mathrm{~bits/second}\f$.)
	 */
	static inline void calibrate(uint8_t pin) { // {{{
		// In case they were enabled: disable interrupts.
		uint8_t sreg = SREG;
		cli();

		// Save backup values of registers that are about to be changed.
		uint8_t tccr1a = TCCR1A;
		uint8_t tccr1b = TCCR1B;
		uint8_t port = Gpio::PORT(pin >> 3);
		uint8_t ddr = Gpio::DDR(pin >> 3);

		uint8_t bit;
		uint16_t const pulse_duration = F_CPU * 9 / 4800;
		Counter::enable1(Counter::s1_div1);
		start_calibration(bit);
		Gpio::input(pin, true);
		volatile uint8_t &pinreg = Gpio::PIN((pin >> 3) & 0xf);
		if (&pinreg == &GPIOR0)	// It returns this for invalid ports.
			return;
		uint8_t b = pin & 0x7;
		while (true) {
			// Wait for falling edge.
			// Record counter value.
			// Wait for rising edge.
			// Record counter value.
			// Compute elapsed time.
			// Call calibrate_iteration().
			Counter::write1(0);	// This should ensure that there are no overflows.
			uint8_t h_start, l_start, h_end, l_end, flag;
			asm volatile(
				// Wait for falling edge.
				"1:\t"	"sbis %[pin], %[bit]"	"\n"
				"\t"	"rjmp 1b"		"\n"
				"1:\t"	"sbic %[pin], %[bit]"	"\n"
				"\t"	"rjmp 1b"		"\n"
				// Record counter value.
				"\t"	"lds %[ls], %[tcnt1l]"	"\n"
				"\t"	"lds %[hs], %[tcnt1h]"	"\n"
				// Clear overflow flag.
				"\t"	"out %[tifr1], %[ovf]"	"\n"

				// Wait for rising edge.
				"1:\t"	"sbis %[pin], %[bit]"	"\n"
				"\t"	"rjmp 1b"		"\n"
				// Record counter value.
				"\t"	"lds %[le], %[tcnt1l]"	"\n"
				"\t"	"lds %[he], %[tcnt1h]"	"\n"
				// Check overflow flag.
				"\t"	"in %[flag], %[tifr1]"	"\n"
				:
					[ls]		"=r"	(l_start),
					[hs]		"=r"	(h_start),
					[le]		"=r"	(l_end),
					[he]		"=r"	(h_end),
					[flag]		"=r"	(flag)
				:
					[pin]		"I"	(_SFR_IO_ADDR(pinreg)),
					[bit]		""	(b),
					[tcnt1l]	""	(_SFR_MEM_ADDR(TCNT1L)),
					[tcnt1h]	""	(_SFR_MEM_ADDR(TCNT1H)),
					[ovf]		"r"	(_BV(TOV1)),
					[tifr1]		"I"	(_SFR_IO_ADDR(TIFR1))
			);
			if (flag & _BV(TOV1)) {
				// Ignore pulse when it includes an overflow.
				continue;
			}
			uint16_t start = (h_start << 8) | l_start;
			uint16_t end = (h_end << 8) | l_end;
			uint16_t measured = end - start;
			if (measured == pulse_duration) {
				// No need to calibrate further: we're done.
				break;
			}
			if (measured < pulse_duration / 4 || measured > pulse_duration * 4) {
				// Measurement is too far off; this is noise.
				continue;
			}
			if (calibrate_iteration(measured > pulse_duration, bit))
				break;
		}
		TCCR1A = tccr1a;
		TCCR1B = tccr1b;
		Gpio::PORT(pin >> 3) = port;
		Gpio::DDR(pin >> 3) = ddr;
		SREG = sreg;
	} // }}}

	/// Calibrate the oscillator using pulses on the ICP1 pin.
	/**
	 * This function works the same as Clock::calibrate(), but it uses the
	 * ICP pin to measure the pulse times. This is more accurate, but it
	 * requires the host serial port that is used for calibration to be
	 * connected to the ICP pin.
	 */
	static inline void calibrate_icp1() { // {{{
		// In case they were enabled: disable interrupts.
		uint8_t sreg = SREG;
		cli();

		// Save backup values of registers that are about to be changed.
		uint8_t tccr1a = TCCR1A;
		uint8_t tccr1b = TCCR1B;
		uint8_t port = Gpio::PORT(PIN_ICP1 >> 3);
		uint8_t ddr = Gpio::DDR(PIN_ICP1 >> 3);

		uint8_t bit;
		uint16_t const pulse_duration = F_CPU * 9 / 4800;
		Counter::enable1(Counter::s1_div1);
		start_calibration(bit);
		Gpio::input(PIN_ICP1, true);
		volatile uint8_t &pinreg = Gpio::PIN((PIN_ICP1 >> 3) & 0xf);
		if (&pinreg == &GPIOR0)	// It returns this for invalid ports.
			return;
		uint8_t b = PIN_ICP1 & 0x7;
		while ((pinreg & (1 << b)) == 0) {}
		Counter::setup_capt1(false);
		Counter::clear_ints1();
		while (true) {
			Counter::write1(0);	// This should ensure that there are no overflows.
			while ((pinreg & (1 << b)) != 0) {}
			uint16_t start = Counter::get_icr1();
			if (TIFR1 & _BV(TOV1)) {
				// Ignore pulse when it includes an overflow.
				continue;
			}
			Counter::setup_capt1(true);
			while ((pinreg & (1 << b)) == 0) {}
			uint16_t end = Counter::get_icr1();
			Counter::setup_capt1(false);
			uint16_t measured = end - start;
			if (measured == pulse_duration) {
				// No need to calibrate further: we're done.
				break;
			}
			if (measured < pulse_duration / 4 || measured > pulse_duration * 4) {
				// Measurement is too far off; this is noise.
				continue;
			}
			if (calibrate_iteration(measured > pulse_duration, bit))
				break;
		}
		TCCR1A = tccr1a;
		TCCR1B = tccr1b;
		Gpio::PORT(PIN_ICP1 >> 3) = port;
		Gpio::DDR(PIN_ICP1 >> 3) = ddr;
		SREG = sreg;
	} // }}}
#else
	// Calibrate using counter 0.
	static inline void calibrate(uint8_t pin) { // {{{
		// In case they were enabled: disable interrupts.
		uint8_t sreg = SREG;
		cli();

		// Save backup values of registers that are about to be changed.
		uint8_t tccr0a = TCCR0A;
		uint8_t tccr0b = TCCR0B;
		uint8_t port = Gpio::PORT(pin >> 3);
		uint8_t ddr = Gpio::DDR(pin >> 3);

		uint8_t bit;
		uint16_t const pulse_duration = F_CPU * 9 / 4800 / 256;
		Counter::enable0(Counter::s0_div256);
		start_calibration(bit);
		Gpio::input(pin, true);
		volatile uint8_t &pinreg = Gpio::PIN((pin >> 3) & 0xf);
		if (&pinreg == &GPIOR0)	// It returns this for invalid ports.
			return;
		uint8_t b = pin & 0x7;
		while (true) {
			// Wait for falling edge.
			// Record counter value.
			// Wait for rising edge.
			// Record counter value.
			// Compute elapsed time.
			// Call calibrate_iteration().
			Counter::write0(0);	// This should ensure that there are no overflows.
			uint8_t start, end, flag;
			asm volatile(
				// Wait for falling edge.
				"1:\t"	"sbis %[pin], %[bit]"	"\n"
				"\t"	"rjmp 1b"		"\n"
				"1:\t"	"sbic %[pin], %[bit]"	"\n"
				"\t"	"rjmp 1b"		"\n"
				// Record counter value.
				"\t"	"lds %[s], %[tcnt0]"	"\n"
				// Clear overflow flag.
				"\t"	"out %[tifr0], %[ovf]"	"\n"

				// Wait for rising edge.
				"1:\t"	"sbis %[pin], %[bit]"	"\n"
				"\t"	"rjmp 1b"		"\n"
				// Record counter value.
				"\t"	"lds %[e], %[tcnt0]"	"\n"
				// Check overflow flag.
				"\t"	"in %[flag], %[tifr0]"	"\n"
				:
					[s]		"=r"	(start),
					[e]		"=r"	(end),
					[flag]		"=r"	(flag)
				:
					[pin]		"I"	(_SFR_IO_ADDR(pinreg)),
					[bit]		""	(b),
					[tcnt0]	""	(_SFR_MEM_ADDR(TCNT0)),
					[ovf]		"r"	(_BV(TOV0)),
					[tifr0]		"I"	(_SFR_IO_ADDR(TIFR0))
			);
			if (flag & _BV(TOV1)) {
				// Ignore pulse when it includes an overflow.
				continue;
			}
			uint16_t start = (h_start << 8) | l_start;
			uint16_t end = (h_end << 8) | l_end;
			uint16_t measured = end - start;
			if (measured == pulse_duration) {
				// No need to calibrate further: we're done.
				break;
			}
			if (measured < pulse_duration / 4 || measured > pulse_duration * 4) {
				// Measurement is too far off; this is noise.
				continue;
			}
			if (calibrate_iteration(measured > pulse_duration, bit))
				break;
		}
		TCCR1A = tccr1a;
		TCCR1B = tccr1b;
		Gpio::PORT(pin >> 3) = port;
		Gpio::DDR(pin >> 3) = ddr;
		SREG = sreg;
	} // }}}
#endif
}

#ifdef AVR_TEST_CLOCK // {{{

#if AVR_TEST_INDEX == 0
namespace Clock {
	static const uint8_t testcode = 'O';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('c');
			Test::tx('C');
			Test::tx('s');
			Test::tx('S');
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	c	get OSCCAL value
		//	CXX	set OSCCAL value to XX
		//	s	get prescaler value
		//	SXX	set prescaler value to XX
		if (cmd >= 'A' && cmd <= 'Z') {
			if (len != 2)
				return false;
		}
		else if (len != 0)
			return false;
		switch (cmd) {
		case 'c':
			Test::send_byte(get_calibration());
			Test::tx('\n');
			break;
		case 'C':
		{
			bool ok = true;
			uint8_t b = Test::read_byte(2, ok);
			if (!ok)
				return false;
			set_calibration(b);
			break;
		}
		case 's':
		{
			uint8_t value = get_prescaler();
			Test::tx(testcode);
			Test::send_byte(value);
			Test::tx('\n');
			break;
		}
		case 'S':
		{
			bool ok = true;
			uint8_t b = Test::read_byte(2, ok);
			if (!ok)
				return false;
			set_prescaler(b);
			break;
		}
		default:
			return false;
		}
		return true;
	}
}

#else

namespace Clock {
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
