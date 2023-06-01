// Counter 0

// Options:
// SYSTEM_CLOCK0_ENABLE
// CALL_system_clock0_interrupt
// CALL_system_clock_interrupt
// SYSTEM_CLOCK0_DIVIDER
// SYSTEM_CLOCK_DIVIDER
// SYSTEM_CLOCK0_TICKS_PER_UNIT
// SYSTEM_CLOCK_TICKS_PER_UNIT
// SYSTEM_CLOCK0_TYPE
// SYSTEM_CLOCK_TYPE

#ifndef _AVR_COUNTER0_HH
#define _AVR_COUNTER0_HH

#ifdef AVR_TEST

//#define SYSTEM_CLOCK0_ENABLE

#if AVR_TEST_INDEX == 1

#define SYSTEM_CLOCK0_DIVIDER 64
#define SYSTEM_CLOCK0_TICKS_PER_UNIT 42
#define SYSTEM_CLOCK0_TYPE uint32_t

#endif
#endif

// File documentation. {{{
/** @file
# Timer/Counter 0 and System Clock 0
Example of using timer/counter 0 as a pwm source and system clock.
```
// Enable the system clock on timer/counter 0. This enables it and uses OC0A as TOP.
// The TOP value is SYSTEM_CLOCK_TICKS_PER_UNIT.
// If only one counter is enabled as system clock, all system clock macros and
// functions can be used without referencing the counter index.
#define SYSTEM_CLOCK0_ENABLE

// Enable system clock interrupts.
#define CALL_system_clock_interrupt

// Enable debug output on the usart.
#define DBG_ENABLE

#include <amat.hh>

// Event time for next event.
// Using absolute event times is more accurate than scheduling a relative event
// every interrupt.
SYSTEM_CLOCK_TYPE next;

void setup() {
	// Set pwm on OC0B to 1/4 duty cycle.
	Counter::set_ocr0b(SYSTEM_CLOCK_TICKS_PER_UNIT / 4);
	// Enable pwm output (with default type: clear on compare match).
	Counter::enable_oc0b();

	// Generate a timer interrupt every 500 ms.
	next = Counter::get_time() + 500;
	Counter::set_interrupt(next);
}

static void system_clock_interrupt() {
	// Schedule next interrupt.
	next += 500;
	Counter::set_interrupt(next);

	// Inform host that time is ticking.
	dbg("Time is ticking!");
}
```

@author Bas Wijnen <wijnen@debian.org>
*/
// }}}

// System clock Macros {{{
#ifdef DOXYGEN

/// Alias for first available SYSTEM_CLOCK*_DIVIDER
#define SYSTEM_CLOCK_DIVIDER
#undef SYSTEM_CLOCK_DIVIDER

/// Alias for first available SYSTEM_CLOCK*_TICKS_PER_UNIT
#define SYSTEM_CLOCK_TICKS_PER_UNIT
#undef SYSTEM_CLOCK_TICKS_PER_UNIT

/// Alias for first available SYSTEM_CLOCK*_TYPE
#define SYSTEM_CLOCK_TYPE
#undef SYSTEM_CLOCK_TYPE

/// Alias for first available CALL_system_clock*_interrupt
#define CALL_system_clock_interrupt
#undef CALL_system_clock_interrupt

#endif

#ifdef SYSTEM_CLOCK0_ENABLE

#if defined(CALL_system_clock_interrupt) && !defined(CALL_system_clock0_interrupt)
#define CALL_system_clock0_interrupt
#endif

#ifndef system_clock_interrupt
/// Alias for the first system_clock*_interrupt
#define system_clock_interrupt system_clock0_interrupt
#endif

#endif

// }}}

#ifdef CALL_system_clock0_interrupt
/// Callback function for set_interrupt0() and set_timeout0()
/**
 * When CALL_system_clock0_interrupt is defined, user code must also define this
 * function.
 */
static void system_clock0_interrupt();
#endif

// This file only handles counter 0. This comment documents the whole namespace.
/// Timer/counter 0, 1, 2, 3, 4 and 5 (but not high speed counter 4)
namespace Counter {

	/// @name Timer/counter 0
	/// @{

	// Counter 0 functions. {{{

	/// Counter source.
	enum Source0 { // {{{
		/// Disable counter
		s0_off = 0,
		/// System clock
		s0_div1 = 1,
		/// System clock / 8
		s0_div8 = 2,
		/// System clock / 64
		s0_div64 = 3,
		/// System clock / 256
		s0_div256 = 4,
		/// System clock / 1024
		s0_div1024 = 5,
		/// External pin T0, falling edge
		s0_T_falling = 6,
		/// External pin T0, rising edge
		s0_T_rising = 7
	}; // }}}
	// Convert between divider and source values {{{
/// @cond
#define _AVR_COUNTER0_D2S(div, x) div == x ? Counter::s0_div ## x :
#define _AVR_COUNTER0_S2D(src, x) src == Counter::s0_div ## x ? x :
/// @endcond
	/// Convert numerical divider to Source0
#define COUNTER0_DIV_TO_SOURCE(div) ( \
	_AVR_COUNTER0_D2S(div, 1024) \
	_AVR_COUNTER0_D2S(div, 256) \
	_AVR_COUNTER0_D2S(div, 64) \
	_AVR_COUNTER0_D2S(div, 8) \
	_AVR_COUNTER0_D2S(div, 1) \
	Counter::s0_off )
	/// Convert Source0 to numerical divider
#define COUNTER0_SOURCE_TO_DIV(src) ( \
	_AVR_COUNTER0_S2D(src, 1024) \
	_AVR_COUNTER0_S2D(src, 256) \
	_AVR_COUNTER0_S2D(src, 64) \
	_AVR_COUNTER0_S2D(src, 8) \
	_AVR_COUNTER0_S2D(src, 1) \
	0 )
	// }}}

	/// Counter mode.
	enum Mode0 { // {{{
		/// Normal mode
		m0_normal = 0,
		/// Phase correct pwm
		m0_pwm_pc = 1,
		/// Clear timer on compare match
		m0_ctc = 2,
		/// Fast pwm
		m0_pwm_fast = 3,
		/// Phase correct pwm with OCR0A as TOP
		m0_pwm_pc_ocra = 5,
		/// Fast pwm with OCR0A as TOP
		m0_pwm_fast_ocra = 7
	}; // }}}

	/// Enable timer/counter 0.
	static inline void enable0(Source0 source = s0_div1, Mode0 mode = m0_normal) { // {{{
		TCCR0A = (mode & 3) << WGM00;
		TCCR0B = ((mode & 4) << (WGM02 - 2)) | (source << CS00);
	} // }}}

	/// Disable timer/counter 0.
	static inline void disable0() { enable0(s0_off); }

	/// Enable output compare on pin OC0A.
	static inline void enable_oc0a(uint8_t type = 2) { TCCR0A = (TCCR0A & ~(_BV(COM0A1) | _BV(COM0A0))) | (type << COM0A0); }

	/// Enable output compare on pin OC0B.
	static inline void enable_oc0b(uint8_t type = 2) { TCCR0A = (TCCR0A & ~(_BV(COM0B1) | _BV(COM0B0))) | (type << COM0B0); }

	/// Disable output compare on pin OC0A.
	static inline void disable_oc0a() { enable_oc0a(0); }

	/// Disable output compare on pin OC0B.
	static inline void disable_oc0b() { enable_oc0b(0); }

	/// Enable overflow interrupt.
	/**
	 * ISR(TIMER0_OVF_vect) must be defined when using this.
	 */
	static inline void enable_ovf0() { TIMSK0 |= _BV(TOIE0); }

	/// Disable overflow interrupt.
	static inline void disable_ovf0() { TIMSK0 &= ~_BV(TOIE0); }

	/// Enable interrupt on compare match for OCR0A.
	/**
	 * ISR(TIMER0_COMPA_vect) must be defined when using this.
	 */
	static inline void enable_compa0() { TIMSK0 |= _BV(OCIE0A); }

	/// Disable interrupt on compare match for OCR0A.
	static inline void disable_compa0() { TIMSK0 &= ~_BV(OCIE0A); }

	/// Enable interrupt on compare match for OCR0B.
	/**
	 * ISR(TIMER0_COMPB_vect) must be defined when using this.
	 */
	static inline void enable_compb0() { TIMSK0 |= _BV(OCIE0B); }

	/// Disable interrupt on compare match for OCR0B.
	static inline void disable_compb0() { TIMSK0 &= ~_BV(OCIE0B); }

	/// Return OCR0A value.
	static inline uint8_t get_ocr0a() { return OCR0A; }

	/// Set OCR0A value.
	static inline void set_ocr0a(uint8_t value) { OCR0A = value; }

	/// Return OCR0B value.
	static inline uint8_t get_ocr0b() { return OCR0B; }

	/// Set OCR0B value.
	static inline void set_ocr0b(uint8_t value) { OCR0B = value; }

	/// Return current counter 0 value.
	static inline uint8_t read0() { return TCNT0; }

	/// Set current counter 0 value.
	static inline void write0(uint8_t d) { TCNT0 = d; }

	/// Return state of overflow interrupt flag.
	static inline bool has_ovf0() { return TIFR0 & _BV(TOV0); }

	/// Return state of oc0a interrupt flag.
	static inline bool has_ocf0a() { return TIFR0 & _BV(OCF0A); }

	/// Return state of oc0b interrupt flag.
	static inline bool has_ocf0b() { return TIFR0 & _BV(OCF0B); }

	/// Clear all counter 0 interrupt flags.
	static inline void clear_ints0() { TIFR0 = _BV(TOV0) | _BV(OCF0A) | _BV(OCF0B); }
#ifdef PRR0

	/// Disable power to counter 0.
	static inline void off0() { PRR0 |= _BV(PRTIM0); }

	/// Enable power to counter 0.
	static inline void on0() { PRR0 &= ~_BV(PRTIM0); }
#endif

	// }}}

	/// @}

// Functions for using counter0 as a system clock. {{{
// Features:
// - busy wait
// - trigger interrupt at absolute or relative time
// - read current time

#ifdef DOXYGEN
/// Activate a system clock using Counter 0.
#define SYSTEM_CLOCK0_ENABLE

/// Enable callback system_clock0_interrupt(), called when Counter::set_interrupt0() or Counter::set_timeout0() is called.
#define CALL_system_clock0_interrupt
#endif

#ifdef SYSTEM_CLOCK0_ENABLE

	/// @name System Clock 0
	/// @{

#if defined(SYSTEM_CLOCK_DIVIDER) && !defined(SYSTEM_CLOCK0_DIVIDER)
#define SYSTEM_CLOCK0_DIVIDER SYSTEM_CLOCK_DIVIDER
#endif

#if defined(SYSTEM_CLOCK_TICKS_PER_UNIT) && !defined(SYSTEM_CLOCK0_TICKS_PER_UNIT)
#define SYSTEM_CLOCK0_TICKS_PER_UNIT SYSTEM_CLOCK_TICKS_PER_UNIT
#endif

#if defined(SYSTEM_CLOCK_TYPE) && !defined(SYSTEM_CLOCK0_TYPE)
#define SYSTEM_CLOCK0_TYPE SYSTEM_CLOCK_TYPE
#endif

#ifndef SYSTEM_CLOCK0_DIVIDER
/// Clock prescaler value when Counter0 is used as a system clock.
/**
 * When SYSTEM_CLOCK0_ENABLE is defined, Counter0 is running as a system clock.
 *
 * The default setup sets the source to Counter::s0_div64, which works well
 * for counting milliseconds on a 16 MHz crystal.
 *
 * Note that the only allowed values are those for which Counter::s0_div* are
 * defined in Counter::Source0.
 *
 * @sa SYSTEM_CLOCK0_TICKS_PER_UNIT
 */
#define SYSTEM_CLOCK0_DIVIDER 64
#endif

#ifndef SYSTEM_CLOCK0_TICKS_PER_UNIT
/// Prescaled clock ticks per time unit when Counter0 is used as a system clock.
/**
 * When SYSTEM_CLOCK0_ENABLE is defined, Counter0 is running as a system clock.
 *
 * This value defines how many prescaled clock ticks make up a time unit.
 *
 * The default value uses milliseconds as the unit. With a 16MHz crystal and
 * the default prescaler of 64, that makes this value 250.
 *
 * @sa SYSTEM_CLOCK0_DIVIDER
 */
#define SYSTEM_CLOCK0_TICKS_PER_UNIT (F_CPU / SYSTEM_CLOCK0_DIVIDER / 1000)
#endif

#ifndef SYSTEM_CLOCK0_TYPE
/// Type of variable for time values.
/**
 * When SYSTEM_CLOCK0_ENABLE is defined, Counter0 is running as a system clock.
 *
 * This macro defines the type that is used for storing the time value and the
 * time interval argument to set_timeout.
 *
 * Larger types cost more memory (both SRAM to store them and flash to do
 * computations on them), but allow for longer timeouts.
 */
#define SYSTEM_CLOCK0_TYPE uint16_t
#endif

/// @cond
	static volatile SYSTEM_CLOCK0_TYPE counter0_time = 0;
#ifdef CALL_system_clock0_interrupt
	static volatile SYSTEM_CLOCK0_TYPE counter0_target;
	static volatile bool counter0_target_active = false;
#endif
#define _AVR_SETUP_COUNTER0 \
	Counter::set_ocr0a(SYSTEM_CLOCK0_TICKS_PER_UNIT); \
	Counter::enable0(COUNTER0_DIV_TO_SOURCE(SYSTEM_CLOCK0_DIVIDER), Counter::m0_ctc); \
	Counter::enable_compa0();

ISR(TIMER0_COMPA_vect) {
	++Counter::counter0_time;
#ifdef CALL_system_clock0_interrupt
	if (Counter::counter0_target_active && (Counter::counter0_target == Counter::counter0_time)) {
		Counter::counter0_target_active = false;
		system_clock0_interrupt();
	}
#endif
}

/// @endcond

	/// Get current time (in units as stored, default is milliseconds) from counter 0.
	static inline SYSTEM_CLOCK0_TYPE get_time0() {
		return counter0_time;
	}

	/// Busy wait for a number of time units (default is milliseconds) using counter 0.
	static inline void busy_wait0(SYSTEM_CLOCK0_TYPE interval) {
		SYSTEM_CLOCK0_TYPE time = get_time0();
		SYSTEM_CLOCK0_TYPE target = time + interval;
		while (time != target) {
			// This may be called with interrupts disabled, so handle overflow events.
			// If interrupts are enabled, this code will never see the flag as set.
			if (!Counter::has_ocf0a()) {
				time = get_time0();
				continue;
			}
			TIFR0 |= _BV(OCF0A);
			++counter0_time;
#ifdef CALL_system_clock0_interrupt
			if (counter0_target_active && (counter0_time == counter0_target)) {
				counter0_target_active = false;
				system_clock0_interrupt();
			}
#endif
			time = get_time0();
		}
	}

#ifdef CALL_system_clock0_interrupt
	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Call system_clock0_interrupt at the specified time. Only one such
	 * interrupt can be scheduled at a time; if this function is called
	 * before the function is called, it will replace the previous setting.
	 *
	 * If check is true, a time in the past results in an immediate call.
	 *
	 * This function is only available if CALL_system_clock0_interrupt is
	 * defined.
	 *
	 * system_clock0_interrupt() must be defined by user code.
	 */
	static inline void set_interrupt0(SYSTEM_CLOCK0_TYPE time) {
		counter0_target = time;
		counter0_target_active = true;
	}

	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Identical to set_interrupt0(), but the argument is a relative time.
	 *
	 * This function is only available if CALL_system_clock0_interrupt is
	 * defined.
	 */
	static inline void set_timeout0(SYSTEM_CLOCK0_TYPE interval) {
		set_interrupt0(get_time0() + interval);
	}

#endif

	/// @}

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
	// Macro is checked again and defined below.
	/// Alias for first available get_time*()
	static inline SYSTEM_CLOCK0_TYPE get_time() { return get_time0(); }
	/// Alias for first available busy_wait*()
	static inline void busy_wait(SYSTEM_CLOCK0_TYPE interval) { return busy_wait0(interval); }

#ifdef CALL_system_clock0_interrupt
	/// Alias for first available set_interrupt*()
	static inline void set_interrupt(SYSTEM_CLOCK0_TYPE time) { return set_interrupt0(time); }
	/// Alias for first available set_timeout*()
	static inline void set_timeout(SYSTEM_CLOCK0_TYPE interval) { return set_timeout0(interval); }
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

#ifdef AVR_TEST_COUNTER0 // {{{

#if AVR_TEST_INDEX == 0
namespace Counter {
	static const uint8_t testcode0 = '0';
	static bool test0(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode0)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode0);
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
		//	a+T	enable oc0a with type T
		//	a-	disable oc0a
		//	a!	enable compa interrupt
		//	a.	disable compa interrupt
		//	a=XX	set OCR0A register
		//	ar	get OCR0A register
		//	b+T	enable oc0b with type T
		//	b-	disable oc0b
		//	b!	enable compb interrupt
		//	b.	disable compb interrupt
		//	b=XX	set OCR0B register
		//	br	get OCR0B register
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
			enable0(static_cast <Source0>(source), static_cast <Mode0>(mode));
			break;
		}
		case 'd':
			if (len != 0)
				return false;
			disable0();
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
					enable_oc0a(type);
				else
					enable_oc0b(type);
				break;
			}
			case '-':
				if (len != 1)
					return false;
				if (cmd == 'a')
					disable_oc0a();
				else
					disable_oc0b();
				break;
			case '!':
				if (len != 1)
					return false;
				if (cmd == 'a')
					enable_compa0();
				else
					enable_compb0();
				break;
			case '.':
				if (len != 1)
					return false;
				if (cmd == 'a')
					disable_compa0();
				else
					disable_compb0();
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
					set_ocr0a(value);
				else
					set_ocr0b(value);
				break;
			}
			case 'r':
			{
				if (len != 1)
					return false;
				uint8_t value = (cmd == 'a' ? get_ocr0a() : get_ocr0b());
				Test::tx(testcode0);
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
			enable_ovf0();
			break;
		case 'O':
			if (len != 0)
				return false;
			disable_ovf0();
			break;
		case 'r':
		{
			if (len != 0)
				return false;
			uint8_t value = read0();
			Test::tx(testcode0);
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
			write0(value);
			break;
		}
		case 'F':
			if (len != 0)
				return false;
			clear_ints0();
			break;
#ifdef PRR0
		case '-':
			if (len != 0)
				return false;
			off0();
			break;
		case '+':
			if (len != 0)
				return false;
			on0();
			break;
#endif
		default:
			return false;
		}
		return true;
	}
}

ISR(TIMER0_COMPA_vect) {
	Test::tx(Counter::testcode0);
	Test::tx('A');
	Test::tx('\n');
}

ISR(TIMER0_COMPB_vect) {
	Test::tx(Counter::testcode0);
	Test::tx('B');
	Test::tx('\n');
}

ISR(TIMER0_OVF_vect) {
	Test::tx(Counter::testcode0);
	Test::tx('O');
	Test::tx('\n');
}

#else

namespace Counter {
	static const uint8_t testcode0 = 0;
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
