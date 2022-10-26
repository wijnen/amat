// Sleep modes

/** @file
# Sleep modes
Example: Sleep in standby mode in main loop.
```
#define CALL_loop
// Define more macros here.

#include <amat.hh>

void setup() {
	// Do other setup here.

	// Enable interrupts. This is not automatic is CALL_loop is defined.
	sei();
}

void loop() {
	Sleep::standby();
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

#ifndef _AVR_SLEEP_HH
#define _AVR_SLEEP_HH

/// @cond
#ifdef SM2
#define SMCR_REG SMCR
#define SMCR_MASK (_BV(SE) | _BV(SM0) | _BV(SM1) | _BV(SM2))
#else
#define SMCR_REG MCUCR
#define SMCR_MASK (_BV(SE) | _BV(SM0) | _BV(SM1))
#endif
/// @endcond

#ifdef DOXYGEN
#define SLEEP_MODE_IDLE
#define SLEEP_MODE_ADC
#define SLEEP_MODE_PWR_OFF
#define SLEEP_MODE_PWR_SAVE
#define SLEEP_MODE_PWR_DOWN
#define SLEEP_MODE_SENSOR_NOISE_REDUCTION
#define SLEEP_MODE_STANDBY
#define SLEEP_MODE_EXT_STANDBY
#define SLEEP_MODE_EXT_PWR_DOWN
#define SLEEP_MODE_EXT_PWR_SAVE
#define BODS
#endif

// Sleep
/// CPU sleep modes
namespace Sleep {
/// @cond
	static inline void sleep(uint8_t mode) {
		SMCR_REG = (SMCR_REG & ~SMCR_MASK) | _BV(SE) | mode;
		asm volatile("sleep");
		SMCR_REG &= ~_BV(SE);
	}
/// @endcond

#ifdef SLEEP_MODE_IDLE
	/// Wait for the next interrupt.
	static inline void idle()			{ sleep(SLEEP_MODE_IDLE); }
#endif

#ifdef SLEEP_MODE_ADC
	/// Disable I/O clock and wait for next interrupt.
	static inline void adc_noise_reduction()	{ sleep(SLEEP_MODE_ADC); }
#endif

#ifdef SLEEP_MODE_PWR_OFF
	/// Disable all clocks and wait for next interrupt.
	static inline void power_off()			{ sleep(SLEEP_MODE_PWR_OFF); }
#endif

#ifdef SLEEP_MODE_SENSOR_NOISE_REDUCTION
	/// Disable all clocks and wait for next interrupt.
	static inline void sensor_noise_reduction()	{ sleep(SLEEP_MODE_SENSOR_NOISE_REDUCTION); }
#endif

#ifdef SLEEP_MODE_PWR_DOWN
	/// Disable all clocks and wait for next interrupt.
	static inline void power_down()			{ sleep(SLEEP_MODE_PWR_DOWN); }
#endif

#ifdef SLEEP_MODE_PWR_SAVE
	/// Disable all clocks except for counter2 and wait for next interrupt.
	static inline void power_save()			{ sleep(SLEEP_MODE_PWR_SAVE); }
#endif

#ifdef SLEEP_MODE_STANDBY
	/// Same as power_down(), but keep oscillator running.
	static inline void standby()			{ sleep(SLEEP_MODE_STANDBY); }
#endif

#ifdef SLEEP_MODE_EXT_STANDBY
	/// Same as power_save(), but keep oscillator running.
	static inline void extended_standby()		{ sleep(SLEEP_MODE_EXT_STANDBY); }
#endif

#ifdef SLEEP_MODE_EXT_PWR_SAVE
	/// Same as power_save(), but keep oscillator running.
	static inline void extended_power_save()	{ sleep(SLEEP_MODE_EXT_PWR_SAVE); }
#endif

#ifdef SLEEP_MODE_EXT_PWR_DOWN
	/// Same as power_save(), but keep oscillator running.
	static inline void extended_power_down()	{ sleep(SLEEP_MODE_EXT_PWR_DOWN); }
#endif

#ifdef BODS
/// @cond
	static inline void sleep_nobod(uint8_t mode) {
		SMCR_REG = (SMCR_REG & ~SMCR_MASK) | _BV(SE) | (mode << SM0);
		uint8_t current = MCUCR;
		asm volatile(
			"\t"	"out %[mcucr], %[prepare]"		"\n"
			"\t"	"out %[mcucr], %[activate]"		"\n"
			"\t"	"sleep"		"\n"
			::
			[mcucr] "M" (_SFR_IO_ADDR(MCUCR)),
			[prepare] "r" (current | _BV(BODS) | _BV(BODSE)),
			[activate] "r" (current | _BV(BODS))
		);
		SMCR_REG &= ~_BV(SE);
	}
/// @endcond

#ifdef SLEEP_MODE_PWR_DOWN
	/// Disable all clocks and wait for next interrupt, while brown-out detection is disabled.
	static inline void power_down_nobod()			{ sleep_nobod(SLEEP_MODE_PWR_DOWN); }
#endif

#ifdef SLEEP_MODE_PWR_SAVE
	/// Disable all clocks except for counter2 and wait for next interrupt, while brown-out detection is disabled.
	static inline void power_save_nobod()			{ sleep_nobod(SLEEP_MODE_PWR_SAVE); }
#endif

#ifdef SLEEP_MODE_STANDBY
	/// Same as power_down_nobod(), but keep oscillator running, while brown-out detection is disabled.
	static inline void standby_nobod()			{ sleep_nobod(SLEEP_MODE_STANDBY); }
#endif

#ifdef SLEEP_MODE_EXT_STANDBY
	/// Same as power_save_nobod(), but keep oscillator running, while brown-out detection is disabled.
	static inline void extended_standby_nobod()		{ sleep_nobod(SLEEP_MODE_EXT_STANDBY); }
#endif

#endif
}

#ifdef AVR_TEST_SLEEP // {{{

#if AVR_TEST_INDEX == 0

namespace Sleep {
	static const uint8_t testcode = 'Z';
	static volatile uint8_t pending_cmd;

	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('i');
			Test::tx('d');
#ifdef BODS
			Test::tx('I');
			Test::tx('D');
#endif
#ifdef SM2
			Test::tx('s');
			Test::tx('a');
			Test::tx('p');
			Test::tx('e');
#ifdef BODS
			Test::tx('S');
			Test::tx('A');
			Test::tx('P');
			Test::tx('E');
#endif
#endif
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	i	idle
		//	a	adc noise reduction
		//	d	power down
		//	p	power save
		//	s	standby
		//	e	external standby
		//	I	idle (no bod)
		//	A	adc noise reduction (no bod)
		//	D	power down (no bod)
		//	P	power save (no bod)
		//	S	standby (no bod)
		//	E	external standby (no bod)
		if (len != 0)
			return false;
		switch (cmd) {
		case 'i':
		case 'd':
#ifdef BODS
		case 'I':
		case 'D':
#endif
#ifdef SM2
		case 's':
		case 'a':
		case 'p':
		case 'e':
#ifdef BODS
		case 'S':
		case 'A':
		case 'P':
		case 'E':
#endif
#endif
			// Actual sleep command is called from main() in test.hh.
			pending_cmd = cmd;
			break;
		default:
			return false;
		}
		return true;
	}
}

#else 

namespace Sleep {
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
