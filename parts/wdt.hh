// Watchdog timer

#ifndef _AVR_WDT_HH
#define _AVR_WDT_HH

/**
 * @file
 * Watchdog Timer
 *
 * @author Bas Wijnen <wijnen@debian.org>
 */

/// @cond
#ifndef WDT_vect
#define WDT_vect WDT_OVERFLOW_vect
#endif
/// @endcond

/// Watchdog Timer
namespace Wdt {
	/// Enable the watchdog timer
	/**
	 * ISR(WDT_vect) must be defined if this is called with interrupt == true.
	 *
	 * The user is responsible for resetting the watchdog timer periodically.
	 */
	static inline void enable(bool interrupt, bool reset, uint8_t time = 9) { // {{{
		uint8_t value =
			_BV(WDIF)
			| (interrupt ? _BV(WDIE) : 0)
			| (time >> 3) << WDP3
			| _BV(WDCE)
			| (reset ? _BV(WDE) : 0)
			| (time & 0x7) << WDP0;
		uint8_t prepare = WDTCSR | _BV(WDCE);
		MCUSR &= ~_BV(WDRF);
		asm volatile(
			"\t"	"in __tmp_reg__, __SREG__"	"\n"
			"\t"	"cli"				"\n"
			"\t"	"sts %[wdtcsr], %[prepare]"	"\n"
			"\t"	"sts %[wdtcsr], %[value]"	"\n"
			"\t"	"out __SREG__, __tmp_reg__"	"\n"
			::
			[prepare] "r" (prepare),
			[value] "r" (value),
			[wdtcsr] "" (_SFR_MEM_ADDR(WDTCSR))
		);
	} // }}}

	/// Disable the watchdog timer
	static inline void disable() { enable(false, false); }

	/// Reset the watchdog timer
	static inline void reset() { asm volatile ("wdr"); }
}

#ifdef AVR_TEST_WDT // {{{

#if AVR_TEST == 0
namespace Wdt {
	static const uint8_t testcode = 'W';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('e');
			Test::tx('d');
			Test::tx('r');
			Test::tx('s');
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	eIRT	enable (interrupt, reset, time)
		//	d	disable
		//	r	reset
		//	s	report state
		switch (cmd) {
		case 'e':
		{
			if (len != 3)
				return false;
			uint8_t I = Test::read_digit(0);
			uint8_t R = Test::read_digit(1);
			uint8_t T = Test::read_digit(2);
			if (I > 1 || R > 1 || T > 9)
				return false;
			enable(I, R, T);
			break;
		}
		case 'd':
			if (len != 6)
				return false;
			disable();
			break;
		case 'r':
			if (len != 0)
				return false;
			asm volatile ("wdr");
		case 's':
			if (len != 0)
				return false;
			Test::tx(testcode);
			Test::send_byte(MCUSR);
			Test::send_byte(WDTCSR);
			Test::tx('\n');
			break;
		default:
			return false;
		}
		return true;
	}
}

ISR(WDT_vect) {
	Test::tx(Wdt::testcode);
	Test::tx('\n');
}

#else 

namespace Wdt {
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
