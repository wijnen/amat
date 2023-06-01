// Counter 2

// Options:
// SYSTEM_CLOCK2_ENABLE
// CALL_system_clock2_interrupt
// SYSTEM_CLOCK2_DIVIDER
// SYSTEM_CLOCK2_TICKS_PER_UNIT
// SYSTEM_CLOCK2_TYPE

#ifndef _AVR_COUNTER2_HH
#define _AVR_COUNTER2_HH

#ifdef AVR_TEST

//#define SYSTEM_CLOCK2_ENABLE

#if AVR_TEST_INDEX == 1

#define SYSTEM_CLOCK2_DIVIDER 64
#define SYSTEM_CLOCK2_TICKS_PER_UNIT 42
#define SYSTEM_CLOCK2_TYPE uint32_t

#endif
#endif

/** @file
# Timer/Counter 2
Example of two system clocks; timer1 counting seconds and timer2 counting milliseconds
```
#define SYSTEM_CLOCK1_ENABLE_CAPT
#define SYSTEM_CLOCK1_DIVIDER 256
#define SYSTEM_CLOCK1_TICKS_PER_UNIT F_CPU / SYSTEM_CLOCK1_DIVIDER
#define CALL_system_clock1_interrupt

#define SYSTEM_CLOCK2_ENABLE
#define CALL_system_clock2_interrupt

#define DBG_ENABLE

#include <amat.hh>

static SYSTEM_CLOCK1_TYPE next1;
static SYSTEM_CLOCK2_TYPE next2;

void setup() {
	// 1 s
	next1 = Counter::get_time1() + 1;
	Counter::set_timeout1(next1);
	// 100 ms
	next2 = Counter::get_time2() + 100;
	Counter::set_timeout2(next2);
}

void system_clock1_interrupt() {
	dbg("1 s");
	next1 += 1;
	Counter::set_timeout1(next1);
}

void system_clock2_interrupt() {
	dbg("100 ms");
	next2 += 100;
	Counter::set_timeout2(next2);
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

// System clock Macros {{{
#ifdef SYSTEM_CLOCK2_ENABLE

#if defined(CALL_system_clock_interrupt) && !defined(CALL_system_clock2_interrupt)
#define CALL_system_clock2_interrupt
#endif

#ifndef system_clock_interrupt
/// Alias for the first system_clock*_interrupt
#define system_clock_interrupt system_clock2_interrupt
#endif

#endif

// }}}

#ifdef CALL_system_clock2_interrupt
/// Callback function for set_interrupt2() and set_timeout2()
/**
 * When CALL_system_clock2_interrupt is defined, user code must also define this
 * function.
 */
static void system_clock2_interrupt();
#endif

namespace Counter {
	/// @name Timer/counter 2
	/// @{

	/// Counter source.
	enum Source2 { // {{{
		/// Disable counter
		s2_off = 0,
		/// System clock
		s2_div1 = 1,
		/// System clock / 8
		s2_div8 = 2,
		/// System clock / 32
		s2_div32 = 3,
		/// System clock / 64
		s2_div64 = 4,
		/// System clock / 128
		s2_div128 = 5,
		/// System clock / 256
		s2_div256 = 6,
		/// System clock / 1024
		s2_div1024 = 7
	}; // }}}
	// Convert between divider and source values {{{
/// @cond
#define _AVR_COUNTER2_D2S(div, x) div == x ? Counter::s2_div ## x :
#define _AVR_COUNTER2_S2D(src, x) src == Counter::s2_div ## x ? x :
/// @endcond
	/// Convert numerical divider to Source2
#define COUNTER2_DIV_TO_SOURCE(div) ( \
	_AVR_COUNTER2_D2S(div, 1024) \
	_AVR_COUNTER2_D2S(div, 256) \
	_AVR_COUNTER2_D2S(div, 128) \
	_AVR_COUNTER2_D2S(div, 64) \
	_AVR_COUNTER2_D2S(div, 32) \
	_AVR_COUNTER2_D2S(div, 8) \
	_AVR_COUNTER2_D2S(div, 1) \
	Counter::s2_off )
	/// Convert Source2 to numerical divider
#define COUNTER2_SOURCE_TO_DIV(src) ( \
	_AVR_COUNTER2_S2D(src, 1024) \
	_AVR_COUNTER2_S2D(src, 256) \
	_AVR_COUNTER2_S2D(src, 64) \
	_AVR_COUNTER2_S2D(src, 8) \
	_AVR_COUNTER2_S2D(src, 1) \
	0 )
	// }}}

	/// Counter mode.
	enum Mode2 { // {{{
		/// Normal mode
		m2_normal = 0,
		/// Phase correct pwm
		m2_pwm_pc = 1,
		/// Clear timer on compare match
		m2_ctc = 2,
		/// Fast pwm
		m2_pwm_fast = 3,
		/// Phase correct pwm, OCR2A is TOP
		m2_pwm_pc_ocra = 5,
		/// Fast pwm, OCR2A is TOP
		m2_pwm_fast_ocra = 7
	}; // }}}

	/// Enable timer/counter2.
	static inline void enable2(Source2 source = s2_div1, Mode2 mode = m2_normal) {
		TCCR2A = (mode & 3) << WGM20;
		TCCR2B = ((mode & 4) << (WGM22 - 2)) | (source << CS20);
	}

	/// Disable timer/counter2.
	static inline void disable2() { enable2(s2_off); }

	/// Enable output compare on pin OC2A.
	static inline void enable_oc2a(uint8_t type = 2) { TCCR2A = (TCCR2A & ~(_BV(COM2A1) | _BV(COM2A0))) | (type << COM2A0); }

	/// Enable output compare on pin OC2B.
	static inline void enable_oc2b(uint8_t type = 2) { TCCR2A = (TCCR2A & ~(_BV(COM2B1) | _BV(COM2B0))) | (type << COM2B0); }

	/// Disable output compare on pin OC2A.
	static inline void disable_oc2a() { enable_oc2a(0); }

	/// Disable output compare on pin OC2B.
	static inline void disable_oc2b() { enable_oc2b(0); }

	/// Enable overflow interrupt.
	/**
	 * ISR(TIMER2_OVF_vect) must be defined when using this.
	 */
	static inline void enable_ovf2() { TIMSK2 |= _BV(TOIE2); }

	/// Disable overflow interrupt.
	static inline void disable_ovf2() { TIMSK2 &= ~_BV(TOIE2); }

	/// Enable interrupt on compare match for OCR2A.
	/**
	 * ISR(TIMER2_COMPA_vect) must be defined when using this.
	 */
	static inline void enable_compa2() { TIMSK2 |= _BV(OCIE2A); }

	/// Disable interrupt on compare match for OCR2A.
	static inline void disable_compa2() { TIMSK2 &= ~_BV(OCIE2A); }

	/// Enable interrupt on compare match for OCR2B.
	/**
	 * ISR(TIMER2_COMPB_vect) must be defined when using this.
	 */
	static inline void enable_compb2() { TIMSK2 |= _BV(OCIE2B); }

	/// Disable interrupt on compare match for OCR2B.
	static inline void disable_compb2() { TIMSK2 &= ~_BV(OCIE2B); }

	/// Return OCR2A value.
	static inline uint8_t get_ocr2a() { return OCR2A; }

	/// Set OCR2A value.
	static inline void set_ocr2a(uint8_t value) { OCR2A = value; }

	/// Return OCR2B value.
	static inline uint8_t get_ocr2b() { return OCR2B; }

	/// Set OCR2B value.
	static inline void set_ocr2b(uint8_t value) { OCR2B = value; }

	/// Return current counter 2 value.
	static inline uint8_t read2() { return TCNT2; }

	/// Set current counter 2 value.
	static inline void write2(uint8_t d) { TCNT2 = d; }

	/// Return state of overflow flag.
	static inline bool has_ovf2() { return TIFR2 & _BV(TOV2); }

	/// Return state of oc2a interrupt flag.
	static inline bool has_ocf2a() { return TIFR2 & _BV(OCF2A); }

	/// Return state of oc2b interrupt flag.
	static inline bool has_ocf2b() { return TIFR2 & _BV(OCF2B); }

	/// Clear all counter 2 interrupt flags.
	static inline void clear_ints2() { TIFR2 = _BV(TOIE2) | _BV(OCIE2A) | _BV(OCIE2B); }

#ifdef PRR0
	/// Disable power to counter 2.
	static inline void off2() { PRR0 |= _BV(PRTIM2); }

	/// Enable power to counter 2.
	static inline void on2() { PRR0 &= ~_BV(PRTIM2); }
#endif

	// @}

// Functions for using counter2 as a system clock. {{{
// Features:
// - busy wait
// - trigger interrupt at absolute or relative time
// - read current time

#ifdef DOXYGEN
/// Activate a system clock using Counter 2.
#define SYSTEM_CLOCK2_ENABLE

/// Enable callback system_clock2_interrupt(), called when Counter::set_interrupt2() or Counter::set_timeout2() is called.
#define CALL_system_clock2_interrupt
#endif

#ifdef SYSTEM_CLOCK2_ENABLE

	/// @name System Clock 2
	/// @{

#if defined(SYSTEM_CLOCK_DIVIDER) && !defined(SYSTEM_CLOCK2_DIVIDER)
#define SYSTEM_CLOCK2_DIVIDER SYSTEM_CLOCK_DIVIDER
#endif

#if defined(SYSTEM_CLOCK_TICKS_PER_UNIT) && !defined(SYSTEM_CLOCK2_TICKS_PER_UNIT)
#define SYSTEM_CLOCK2_TICKS_PER_UNIT SYSTEM_CLOCK_TICKS_PER_UNIT
#endif

#if defined(SYSTEM_CLOCK_TYPE) && !defined(SYSTEM_CLOCK2_TYPE)
#define SYSTEM_CLOCK2_TYPE SYSTEM_CLOCK_TYPE
#endif

#ifndef SYSTEM_CLOCK2_DIVIDER
/// Clock prescaler value when Counter2 is used as a system clock.
/**
 * When SYSTEM_CLOCK2_ENABLE is defined, Counter2 is running as a system clock.
 *
 * The default setup sets the source to Counter::s2_div64, which works well
 * for counting milliseconds on a 16 MHz crystal.
 *
 * Note that the only allowed values are those for which Counter::s2_div* are
 * defined in Counter::Source2.
 *
 * @sa SYSTEM_CLOCK2_TICKS_PER_UNIT
 */
#define SYSTEM_CLOCK2_DIVIDER 64
#endif

#ifndef SYSTEM_CLOCK2_TICKS_PER_UNIT
/// Prescaled clock ticks per time unit when Counter2 is used as a system clock.
/**
 * When SYSTEM_CLOCK2_ENABLE is defined, Counter2 is running as a system clock.
 *
 * This value defines how many prescaled clock ticks make up a time unit.
 *
 * The default value uses milliseconds as the unit. With a 16MHz crystal and
 * the default prescaler of 64, that makes this value 250.
 *
 * @sa SYSTEM_CLOCK2_DIVIDER
 */
#define SYSTEM_CLOCK2_TICKS_PER_UNIT (F_CPU / SYSTEM_CLOCK2_DIVIDER / 1000)
#endif

#ifndef SYSTEM_CLOCK2_TYPE
/// Type of variable for time values.
/**
 * When SYSTEM_CLOCK2_ENABLE is defined, Counter2 is running as a system clock.
 *
 * This macro defines the type that is used for storing the time value and the
 * time interval argument to set_timeout.
 *
 * Larger types cost more memory (both SRAM to store them and flash to do
 * computations on them), but allow for longer timeouts.
 */
#define SYSTEM_CLOCK2_TYPE uint16_t
#endif

/// @cond
	static volatile SYSTEM_CLOCK2_TYPE counter2_time = 0;
#ifdef CALL_system_clock2_interrupt
	static volatile SYSTEM_CLOCK2_TYPE counter2_target;
	static volatile bool counter2_target_active = false;
#endif
#define _AVR_SETUP_COUNTER2 \
	Counter::set_ocr2a(SYSTEM_CLOCK2_TICKS_PER_UNIT); \
	Counter::enable2(COUNTER2_DIV_TO_SOURCE(SYSTEM_CLOCK2_DIVIDER), Counter::m2_ctc); \
	Counter::enable_compa2();

ISR(TIMER2_COMPA_vect) {
	++Counter::counter2_time;
#ifdef CALL_system_clock2_interrupt
	if (Counter::counter2_target_active && (Counter::counter2_target == Counter::counter2_time)) {
		Counter::counter2_target_active = false;
		system_clock2_interrupt();
	}
#endif
}

/// @endcond

	/// Get current time (in units as stored, default is milliseconds) from counter 2.
	static inline SYSTEM_CLOCK2_TYPE get_time2() {
		return counter2_time;
	}

	/// Busy wait for a number of time units (default is milliseconds) using counter 2.
	static inline void busy_wait2(SYSTEM_CLOCK2_TYPE interval) {
		SYSTEM_CLOCK2_TYPE time = get_time2();
		SYSTEM_CLOCK2_TYPE target = time + interval;
		while (time != target) {
			// This may be called with interrupts disabled, so handle overflow events.
			// If interrupts are enabled, this code will never see the flag as set.
			if (!Counter::has_ocf2a()) {
				time = get_time2();
				continue;
			}
			TIFR2 |= _BV(OCF2A);
			++counter2_time;
#ifdef CALL_system_clock2_interrupt
			if (counter2_target_active && (counter2_time == counter2_target)) {
				counter2_target_active = false;
				system_clock2_interrupt();
			}
#endif
			time = get_time2();
		}
	}

#ifdef CALL_system_clock2_interrupt
	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Call system_clock2_interrupt at the specified time. Only one such
	 * interrupt can be scheduled at a time; if this function is called
	 * before the function is called, it will replace the previous setting.
	 *
	 * If check is true, a time in the past results in an immediate call.
	 *
	 * This function is only available if CALL_system_clock2_interrupt is
	 * defined.
	 *
	 * system_clock2_interrupt() must be defined by user code.
	 */
	static inline void set_interrupt2(SYSTEM_CLOCK2_TYPE time) {
		counter2_target = time;
		counter2_target_active = true;
	}

	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Identical to set_interrupt2(), but the argument is a relative time.
	 *
	 * This function is only available if CALL_system_clock2_interrupt is
	 * defined.
	 */
	static inline void set_timeout2(SYSTEM_CLOCK2_TYPE interval) {
		set_interrupt2(get_time2() + interval);
	}

#endif

	/// @}

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
	// Macro is checked again and defined below.
	/// Alias for first available get_time*()
	static inline SYSTEM_CLOCK2_TYPE get_time() { return get_time2(); }
	/// Alias for first available busy_wait*()
	static inline void busy_wait(SYSTEM_CLOCK2_TYPE interval) { return busy_wait2(interval); }

#ifdef CALL_system_clock2_interrupt
	/// Alias for first available set_interrupt*()
	static inline void set_interrupt(SYSTEM_CLOCK2_TYPE time) { return set_interrupt2(time); }
	/// Alias for first available set_timeout*()
	static inline void set_timeout(SYSTEM_CLOCK2_TYPE interval) { return set_timeout2(interval); }
#endif
#endif

/// @cond
#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
#define _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
#endif
/// @endcond

#endif

	// }}}
}

#ifdef AVR_TEST_COUNTER2 // {{{

#if AVR_TEST_INDEX == 0

namespace Counter {
	static const uint8_t testcode2 = '2';
	static bool test2(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode2)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode2);
			Test::tx('e');
			Test::tx('d');
			Test::tx('a');
			Test::tx('b');
			Test::tx('o');
			Test::tx('O');
			Test::tx('r');
			Test::tx('w');
			Test::tx('F');
			Test::tx('-');
			Test::tx('+');
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	eXY	enable with source X in mode Y
		//	d	disable
		//	a+T	enable oc2a with type T
		//	a-	disable oc2a
		//	a!	enable compa interrupt
		//	a.	disable compa interrupt
		//	a=XX	set OCR2A register
		//	ar	get OCR2A register
		//	b+T	enable oc2b with type T
		//	b-	disable oc2b
		//	b!	enable compb interrupt
		//	b.	disable compb interrupt
		//	b=XX	set OCR2B register
		//	br	get OCR2B register
		//	o	enable ovf
		//	O	disable ovf
		//	r	read value
		//	wXX	write value
		//	F	clear interrupt flags
		//	-	switch off module
		//	+	switch on module
		switch (cmd) {
		case 'e':
		{
			if (len != 2)
				return false;
			uint8_t source = Test::read_digit(0);
			uint8_t mode = Test::read_digit(1);
			enable2(static_cast <Source2>(source), static_cast <Mode2>(mode));
			break;
		}
		case 'd':
			if (len != 0)
				return false;
			disable2();
			break;
		case 'a':
		case 'b':
			if (len < 1)
				return false;
			switch (Test::rx_read(0)) {
			case '+':
			{
				if (len != 2)
					return false;
				uint8_t type = Test::read_digit(1);
				if (cmd == 'a')
					enable_oc2a(type);
				else
					enable_oc2b(type);
				break;
			}
			case '-':
				if (len != 1)
					return false;
				if (cmd == 'a')
					disable_oc2a();
				else
					disable_oc2b();
				break;
			case '!':
				if (len != 1)
					return false;
				if (cmd == 'a')
					enable_compa2();
				else
					enable_compb2();
				break;
			case '.':
				if (len != 1)
					return false;
				if (cmd == 'a')
					disable_compa2();
				else
					disable_compb2();
				break;
			case '=':
			{
				if (len != 3)
					return false;
				bool ok = true;
				uint8_t value = Test::read_byte(1, ok);
				if (!ok)
					return false;
				if (cmd == 'a')
					set_ocr2a(value);
				else
					set_ocr2b(value);
				break;
			}
			case 'r':
			{
				if (len != 1)
					return false;
				uint8_t value = (cmd == 'a' ? get_ocr2a() : get_ocr2b());
				Test::tx(testcode2);
				Test::send_byte(value);
				Test::tx('\n');
				break;
			}
			default:
				return false;
			}
			break;
		case 'o':
			if (len != 0)
				return false;
			enable_ovf2();
			break;
		case 'O':
			if (len != 0)
				return false;
			disable_ovf2();
			break;
		case 'r':
		{
			if (len != 0)
				return false;
			uint8_t value = read2();
			Test::tx(testcode2);
			Test::send_byte(value);
			Test::tx('\n');
			break;
		}
		case 'w':
		{
			if (len != 2)
				return false;
			bool ok = true;
			uint8_t value = Test::read_byte(0, ok);
			if (!ok)
				return false;
			write2(value);
			break;
		}
		case 'F':
			if (len != 0)
				return false;
			clear_ints2();
			break;
#ifdef PRR0
		case '-':
			if (len != 0)
				return false;
			off2();
			break;
		case '+':
			if (len != 0)
				return false;
			on2();
			break;
#endif
		default:
			return false;
		}
		return true;
	}
}

ISR(TIMER2_COMPA_vect) {
	Test::tx(Counter::testcode2);
	Test::tx('A');
	Test::tx('\n');
}

ISR(TIMER2_COMPB_vect) {
	Test::tx(Counter::testcode2);
	Test::tx('B');
	Test::tx('\n');
}

ISR(TIMER2_OVF_vect) {
	Test::tx(Counter::testcode2);
	Test::tx('O');
	Test::tx('\n');
}
#else

namespace Counter {
	static const uint8_t testcode2 = 0;
	static bool test2(uint8_t part, uint8_t cmd, uint8_t len) {
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
