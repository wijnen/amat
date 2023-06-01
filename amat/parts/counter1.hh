// Counter 1

// Options (all also for 3,4,5):
// CALL_system_clock1_interrupt
// SYSTEM_CLOCK1_ENABLE_CAPT
// SYSTEM_CLOCK1_ENABLE_COMPΑ
// SYSTEM_CLOCK1_DIVIDER
// SYSTEM_CLOCK1_TICKS_PER_UNIT
// SYSTEM_CLOCK1_TYPE

#ifndef _AVR_COUNTER1_HH
#define _AVR_COUNTER1_HH

#ifdef AVR_TEST

#if AVR_TEST_INDEX < 2
#define SYSTEM_CLOCK1_ENABLE
#define SYSTEM_CLOCK3_ENABLE
#define SYSTEM_CLOCK4_ENABLE
#define SYSTEM_CLOCK5_ENABLE
#endif

#if AVR_TEST_INDEX == 1

#define SYSTEM_CLOCK1_DIVIDER 64
#define SYSTEM_CLOCK1_TICKS_PER_UNIT 42
#define SYSTEM_CLOCK1_TYPE uint32_t

#define SYSTEM_CLOCK3_DIVIDER 64
#define SYSTEM_CLOCK3_TICKS_PER_UNIT 42
#define SYSTEM_CLOCK3_TYPE uint32_t

#define SYSTEM_CLOCK4_DIVIDER 64
#define SYSTEM_CLOCK4_TICKS_PER_UNIT 42
#define SYSTEM_CLOCK4_TYPE uint32_t

#define SYSTEM_CLOCK5_DIVIDER 64
#define SYSTEM_CLOCK5_TICKS_PER_UNIT 42
#define SYSTEM_CLOCK5_TYPE uint32_t

#endif
#endif

/** @file
Timer/Counter 1, 3, 4 and 5 (but not high speed counter 4)

The documentation only shows the functions for Counter 1. The versions for
Counter 3, 4 and 5 are equivaluent, and only present if available in
hardware.

Example using input capture registers for system clock:
```
#define SYSTEM_CLOCK1_ENABLE_CAPT
#define CALL_system_clock_interrupt
#define DBG_ENABLE

#include <amat.hh>

void setup() {
        dbg("start");
        Counter::set_timeout(1000);
}

void system_clock_interrupt() {
        dbg("interrupt");
        Counter::set_timeout(1000);
}

@author Bas Wijnen <wijnen@debian.org>
```
*/

// Convenience defines. {{{
#if defined(TIM0_OVF_vect) && !defined(TIMER0_OVF_vect)
#define TIMER0_OVF_vect TIM0_OVF_vect
#endif
#if defined(TIM0_COMPA_vect) && !defined(TIMER0_COMPA_vect)
#define TIMER0_COMPA_vect TIM0_COMPA_vect
#endif
#if defined(TIM0_COMPB_vect) && !defined(TIMER0_COMPB_vect)
#define TIMER0_COMPB_vect TIM0_COMPB_vect
#endif
#if defined(TIM1_OVF_vect) && !defined(TIMER1_OVF_vect)
#define TIMER1_OVF_vect TIM1_OVF_vect
#endif
#if defined(TIM1_COMPA_vect) && !defined(TIMER1_COMPA_vect)
#define TIMER1_COMPA_vect TIM1_COMPA_vect
#endif
#if defined(TIM1_COMPB_vect) && !defined(TIMER1_COMPB_vect)
#define TIMER1_COMPB_vect TIM1_COMPB_vect
#endif
#if defined(TIM1_CAPT_vect) && !defined(TIMER1_CAPT_vect)
#define TIMER1_CAPT_vect TIM1_CAPT_vect
#endif
// }}}

// Definition of _AVR_NUM_COUNTER3 {{{
/// @cond
#ifdef TCNT5L
#define _AVR_NUM_COUNTER3 3
#elif defined(TCNT4L) && !defined(TCCR4E)
#define _AVR_NUM_COUNTER3 2
#elif defined(TCNT3L)
#define _AVR_NUM_COUNTER3 1
#else
#define _AVR_NUM_COUNTER3 0
#endif
/// @endcond
// }}}

// System clock defaults for counter 1 {{{
#if defined(SYSTEM_CLOCK1_ENABLE_CAPT) || defined(SYSTEM_CLOCK1_ENABLE_COMPA)

#if defined(CALL_system_clock_interrupt) && !defined(CALL_system_clock1_interrupt)
#define CALL_system_clock1_interrupt
#endif

#ifndef system_clock_interrupt
/// Alias for the first system_clock*_interrupt
#define system_clock_interrupt system_clock1_interrupt
#endif

#ifdef CALL_system_clock1_interrupt
static void system_clock1_interrupt();
#endif

#endif
// }}}

#ifdef TCNT3L
// System clock defaults for counter 3 {{{
#if defined(SYSTEM_CLOCK3_ENABLE_CAPT) || defined(SYSTEM_CLOCK3_ENABLE_COMPA)

#if defined(CALL_system_clock_interrupt) && !defined(CALL_system_clock3_interrupt)
#define CALL_system_clock3_interrupt
#endif

#ifndef system_clock_interrupt
/// Alias for the first system_clock*_interrupt
#define system_clock_interrupt system_clock3_interrupt
#endif

#ifdef CALL_system_clock3_interrupt
static void system_clock3_interrupt();
#endif

#endif
// }}}
#endif

#if (defined(TCNT4L) && !defined(TCCR4E))
// System clock defaults for counter 4 {{{
#if defined(SYSTEM_CLOCK4_ENABLE_CAPT) || defined(SYSTEM_CLOCK4_ENABLE_COMPA)

#if defined(CALL_system_clock_interrupt) && !defined(CALL_system_clock4_interrupt)
#define CALL_system_clock4_interrupt
#endif

#ifndef system_clock_interrupt
/// Alias for the first system_clock*_interrupt
#define system_clock_interrupt system_clock4_interrupt
#endif

#ifdef CALL_system_clock4_interrupt
static void system_clock4_interrupt();
#endif

#endif
// }}}
#endif

#ifdef TCNT5L
// System clock defaults for counter 5 {{{
#if defined(SYSTEM_CLOCK5_ENABLE_CAPT) || defined(SYSTEM_CLOCK5_ENABLE_COMPA)

#if defined(CALL_system_clock_interrupt) && !defined(CALL_system_clock5_interrupt)
#define CALL_system_clock5_interrupt
#endif

#ifndef system_clock_interrupt
/// Alias for the first system_clock*_interrupt
#define system_clock_interrupt system_clock5_interrupt
#endif

#ifdef CALL_system_clock5_interrupt
static void system_clock5_interrupt();
#endif

#endif
// }}}
#endif

namespace Counter {
	/// @name Timer/counter 1
	/// @{

	/// The functions listed for Timer/counter 1 are also available for Timer/counter 3, 4 and 5 (if they are supported in hardware).
	///
	/// The high speed timer/counter 4 that is present in USB capable devices is not controlled through these functions.

	//

	/// Counter source.
	enum Source1 { // {{{
		/// Disable counter
		s1_off = 0,
		/// System clock
		s1_div1 = 1,
		/// System clock / 8
		s1_div8 = 2,
		/// System clock / 64
		s1_div64 = 3,
		/// System clock / 256
		s1_div256 = 4,
		/// System clock / 1024
		s1_div1024 = 5,
		/// External pin T1, falling edge
		s1_T_falling = 6,
		/// External pin T1, rising edge
		s1_T_rising = 7
	}; // }}}
	/// @cond
#define _AVR_COUNTER1_D2S(div, x) div == x ? Counter::s1_div ## x :
#define _AVR_COUNTER1_S2D(src, x) src == Counter::s1_div ## x ? x :
/// @endcond
        /// Convert numerical divider to Source1
#define COUNTER1_DIV_TO_SOURCE(div) ( \
        _AVR_COUNTER1_D2S(div, 1024) \
        _AVR_COUNTER1_D2S(div, 256) \
        _AVR_COUNTER1_D2S(div, 64) \
        _AVR_COUNTER1_D2S(div, 8) \
        _AVR_COUNTER1_D2S(div, 1) \
        Counter::s1_off )
        /// Convert Source1 to numerical divider
#define COUNTER1_SOURCE_TO_DIV(src) ( \
        _AVR_COUNTER1_S2D(src, 1024) \
        _AVR_COUNTER1_S2D(src, 256) \
        _AVR_COUNTER1_S2D(src, 64) \
        _AVR_COUNTER1_S2D(src, 8) \
        _AVR_COUNTER1_S2D(src, 1) \
        0 )
        // }}}

	/// Counter mode.
	enum Mode1 { // {{{
		/// Normal mode
		m1_normal = 0,
		/// Phase correct pwm, 8 bit
		m1_pwm_pc8 = 1,
		/// Phase correct pwm, 9 bit
		m1_pwm_pc9 = 2,
		/// Phase correct pwm, 10 bit
		m1_pwm_pc10 = 3,
		/// Clear timer on compare match, OCR1A is TOP
		m1_ctc_ocra = 4,
		/// Fast pwm, 8 bit
		m1_pwm_fast8 = 5,
		/// Fast pwm, 9 bit
		m1_pwm_fast9 = 6,
		/// Fast pwm, 10 bit
		m1_pwm_fast10 = 7,
		/// Phase and frequency correct pwm, ICR1 as TOP
		m1_pwm_pfc_icr = 8,
		/// Phase and frequency correct pwm, OCR1A as TOP
		m1_pwm_pfc_ocra = 9,
		/// Phase correct pwm, ICR1 as TOP
		m1_pwm_pc_icr = 10,
		/// Phase correct pwm, OCR1A as TOP
		m1_pwm_pc_ocra = 11,
		/// Clear timer on compare match, ICR1 as TOP
		m1_ctc_icr = 12,
		/// Fast pwm, ICR1 as TOP
		m1_pwm_fast_icr = 14,
		/// Fast pwm, OCR1A as TOP
		m1_pwm_fast_ocra = 15
	}; // }}}

	/// @cond
#ifdef TCNT3L
	enum Source3 { // {{{
		s3_off = 0,
		s3_div1 = 1,
		s3_div8 = 2,
		s3_div64 = 3,
		s3_div256 = 4,
		s3_div1024 = 5,
		s3_T_falling = 6,
		s3_T_rising = 7
	}; // }}}
	enum Mode3 { // {{{
		m3_normal = 0,
		m3_pwm_pc8 = 1,
		m3_pwm_pc9 = 2,
		m3_pwm_pc10 = 3,
		m3_ctc_ocra = 4,
		m3_pwm_fast8 = 5,
		m3_pwm_fast9 = 6,
		m3_pwm_fast10 = 7,
		m3_pwm_pfc_icr = 8,
		m3_pwm_pfc_ocra = 9,
		m3_pwm_pc_icr = 10,
		m3_pwm_pc_ocra = 11,
		m3_ctc_icr = 12,
		m3_pwm_fast_icr = 14,
		m3_pwm_fast_ocra = 15
	}; // }}}
#endif
#if (defined(TCNT4L) && !defined(TCCR4E))
	enum Source4 { // {{{
		s4_off = 0,
		s4_div1 = 1,
		s4_div8 = 2,
		s4_div64 = 3,
		s4_div256 = 4,
		s4_div1024 = 5,
		s4_T_falling = 6,
		s4_T_rising = 7
	}; // }}}
	enum Mode4 { // {{{
		m4_normal = 0,
		m4_pwm_pc8 = 1,
		m4_pwm_pc9 = 2,
		m4_pwm_pc10 = 3,
		m4_ctc_ocra = 4,
		m4_pwm_fast8 = 5,
		m4_pwm_fast9 = 6,
		m4_pwm_fast10 = 7,
		m4_pwm_pfc_icr = 8,
		m4_pwm_pfc_ocra = 9,
		m4_pwm_pc_icr = 10,
		m4_pwm_pc_ocra = 11,
		m4_ctc_icr = 12,
		m4_pwm_fast_icr = 14,
		m4_pwm_fast_ocra = 15
	}; // }}}
#endif
#ifdef TCNT5L
	enum Source5 { // {{{
		s5_off = 0,
		s5_div1 = 1,
		s5_div8 = 2,
		s5_div64 = 3,
		s5_div256 = 4,
		s5_div1024 = 5,
		s5_T_falling = 6,
		s5_T_rising = 7
	}; // }}}
	enum Mode5 { // {{{
		m5_normal = 0,
		m5_pwm_pc8 = 1,
		m5_pwm_pc9 = 2,
		m5_pwm_pc10 = 3,
		m5_ctc_ocra = 4,
		m5_pwm_fast8 = 5,
		m5_pwm_fast9 = 6,
		m5_pwm_fast10 = 7,
		m5_pwm_pfc_icr = 8,
		m5_pwm_pfc_ocra = 9,
		m5_pwm_pc_icr = 10,
		m5_pwm_pc_ocra = 11,
		m5_ctc_icr = 12,
		m5_pwm_fast_icr = 14,
		m5_pwm_fast_ocra = 15
	}; // }}}
#endif
	/// @endcond

#ifdef DOXYGEN

	// Documentation of timer1 functions. {{{
	/// Enable timer/counter 1.
	static inline void enable1(Source1 source = s1_div1, Mode1 mode = m1_normal);

	/// Disable timer/counter 1.
	static inline void disable1();

	/// Enable overflow interrupt.
	/**
	 * ISR(TIMER1_OVF_vect) must be defined when using this.
	 */
	static inline void enable_ovf1();

	/// Disable overflow interrupt.
	static inline void disable_ovf1();

	/// Set up input capture.
	static inline void setup_capt1(bool rising = true, bool cancel_noise = false);

	/// Enable input capture interrupt.
	/**
	 * ISR(TIMER1_CAPT_vect) must be defined when using this.
	 */
	static inline void enable_capt1();

	/// Disable input capture interrupt.
	static inline void disable_capt1();

	/// Enable output compare on pin OC1A.
	static inline void enable_oc1a(uint8_t type = 2);

	/// Disable output compare on pin OC1A.
	static inline void disable_oc1a();

	/// Enable interrupt on compare match for OCR1A.
	/**
	 * ISR(TIMER1_COMPA_vect) must be defined when using this.
	 */
	static inline void enable_compa1();

	/// Disable interrupt on compare match for OCR1A.
	static inline void disable_compa1();

	/// Return OCR1A value.
	static inline uint16_t get_ocr1a();

	/// Set OCR1A value.
	static inline void set_ocr1a(uint16_t value);

	/// Enable output compare on pin OC1B.
	static inline void enable_oc1b(uint8_t type = 2);

	/// Disable output compare on pin OC1B.
	static inline void disable_oc1b();

	/// Enable interrupt on compare match for OCR1B.
	/**
	 * ISR(TIMER1_COMPB_vect) must be defined when using this.
	 */
	static inline void enable_compb1();

	/// Disable interrupt on compare match for OCR1B.
	static inline void disable_compb1();

	/// Return OCR1B value.
	static inline uint16_t get_ocr1b();

	/// Set OCR1B value.
	static inline void set_ocr1b(uint16_t value);

	/// Enable output compare on pin OC1C.
	static inline void enable_oc1c(uint8_t type = 2);

	/// Disable output compare on pin OC1C.
	static inline void disable_oc1c();

	/// Enable interrupt on compare match for OCR1C.
	/**
	 * ISR(TIMER1_COMPC_vect) must be defined when using this.
	 */
	static inline void enable_compc1();

	/// Disable output compare on pin OC1C.
	static inline void disable_compc1();

	/// Return OCR1C value.
	static inline uint16_t get_ocr1c();

	/// Set OCR1C value.
	static inline void set_ocr1c(uint16_t value);

	/// Return captured value.
	static inline uint16_t get_icr1();

	/// Set "captured" value (ICR1 registers)
	static inline void set_icr1(uint16_t hl);

	/// Return current counter 1 value.
	static inline uint16_t read1();

	/// Set current counter 1 value.
	static inline void write1(uint16_t hl);

	/// Return state of overflow flag.
	static inline bool has_ovf1();

	/// Return state of overflow flag.
	static inline bool has_ocf1a();

	/// Return state of overflow flag.
	static inline bool has_ocf1b();

	/// Return state of overflow flag.
	static inline bool has_ocf1c();

	/// Clear all counter 1 interrupt flags.
	static inline void clear_ints1();

	// }}}

#else

	// Actual implementation of timerN functions. {{{
#define _AVR_COUNTER(N, OCRB, OCRC) \
	static inline void enable ## N(Source ## N source = s ## N ## _div1, Mode ## N mode = m ## N ## _normal) { \
		TCCR ## N ## A = (mode & 3) << WGM ## N ## 0; \
		TCCR ## N ## B = ((mode & 0xc) << (WGM ## N ## 2 - 2)) | (source << CS ## N ## 0); \
	} \
	static inline void disable ## N() { enable ## N(s ## N ## _off); } \
	static inline void enable_ovf ## N() { TIMSK ## N |= _BV(TOIE ## N); } \
	static inline void disable_ovf ## N() { TIMSK ## N &= ~_BV(TOIE ## N); } \
	static inline void setup_capt ## N(bool rising = true, bool cancel_noise = false) { TCCR ## N ## B = (TCCR ## N ## B & ~(_BV(ICNC ## N) | _BV(ICES ## N))) | (cancel_noise ? _BV(ICNC ## N) : 0) | (rising ? _BV(ICES ## N) : 0); } \
	static inline void enable_capt ## N() { TIMSK ## N |= _BV(ICIE ## N); } \
	static inline void disable_capt ## N() { TIMSK ## N &= ~_BV(ICIE ## N); } \
	_AVR_COUNTER1_OCR(N, a, A) \
	OCRB \
	OCRC \
	static inline uint16_t get_icr ## N() { \
		uint8_t l = ICR ## N ## L; \
		uint8_t h = ICR ## N ## H; \
		return uint16_t(h) << 8 | l;  \
	} \
	static inline void set_icr ## N(uint16_t hl) { \
		ICR ## N ## H = (hl >> 8) & 0xff; \
		ICR ## N ## L = hl & 0xff; \
	} \
	static inline uint16_t read ## N() { \
		uint8_t l = TCNT ## N ## L; \
		uint8_t h = TCNT ## N ## H; \
		return uint16_t(h) << 8 | l;  \
	} \
	static inline void write ## N(uint16_t hl) { \
		TCNT ## N ## H = (hl >> 8) & 0xff; \
		TCNT ## N ## L = hl & 0xff; \
	} \
	static inline bool has_ovf ## N() { return TIFR ## N & _BV(TOV ## N); } \
	static inline bool has_capt ## N() { return TIFR ## N & _BV(ICF ## N); } \
	static inline void clear_ints ## N() { TIFR ## N = _BV(TOV ## N) | _BV(OCF ## N ## A) | _BV(OCF ## N ## B) | _BV(ICF ## N); }

#define _AVR_COUNTER1_OCR(N, p, P) \
	static inline void enable_oc ## N ## p(uint8_t type = 2) { \
		TCCR ## N ## P = (TCCR ## N ## P & ~(_BV(COM ## N ## P ## 1) | _BV(COM ## N ## P ## 0))) | (type << COM ## N ## P ## 0); \
	} \
	static inline void disable_oc ## N ## p() { \
		enable_oc ## N ## p(0); \
	} \
	static inline void enable_comp ## p ## N() { \
		TIMSK ## N |= _BV(OCIE ## N ## P); \
	} \
	static inline void disable_comp ## p ## N() { \
		TIMSK ## N &= ~_BV(OCIE ## N ## P); \
	} \
	static inline uint16_t get_ocr ## N ## p() { \
		uint8_t l = OCR ## N ## P ## L; \
		uint8_t h = OCR ## N ## P ## H; \
		return uint16_t(h) << 8 | l;  \
	} \
	static inline void set_ocr ## N ## p(uint16_t value) { \
		OCR ## N ## P ## H = (value >> 8) & 0xff; \
		OCR ## N ## P ## L = value & 0xff; \
	} \
	static inline bool has_ocf ## N ## p() { return TIFR ## N & _BV(OCF ## N ## P); }

#ifdef OCR1BL
#define _AVR_COUNTER1_B _AVR_COUNTER1_OCR(1, b, B)
#else
#define _AVR_COUNTER1_B
#endif
#ifdef OCR1CL
#define _AVR_COUNTER1_C _AVR_COUNTER1_OCR(1, c, C)
#else
#define _AVR_COUNTER1_C
#endif
	_AVR_COUNTER(1, _AVR_COUNTER1_B, _AVR_COUNTER1_C)

	// }}}

#endif	// !defined(DOXYGEN)

	// Power reduction functions for timer1. {{{
#ifdef PRR0
	/// Disable power to counter 1.
	static inline void off1() { PRR0 |= _BV(PRTIM1); }

	/// Enable power to counter 1.
	static inline void on1() { PRR0 &= ~_BV(PRTIM1); }
#endif
	// }}}

#ifdef TCNT3L // {{{
/// @cond
#ifdef OCR3BL
#define _AVR_COUNTER3_B _AVR_COUNTER1_OCR(3, b, B)
#else
#define _AVR_COUNTER3_B
#endif
#ifdef OCR3CL
#define _AVR_COUNTER3_C _AVR_COUNTER1_OCR(3, c, C)
#else
#define _AVR_COUNTER3_C
#endif
	_AVR_COUNTER(3, _AVR_COUNTER3_B, _AVR_COUNTER3_C)

	/// Disable power to counter 3.
	static inline void off3() { PRR1 |= _BV(PRTIM3); }

	/// Enable power to counter 3.
	static inline void on3() { PRR1 &= ~_BV(PRTIM3); }
/// @endcond
#endif // }}}

#if (defined(TCNT4L) && !defined(TCCR4E)) // {{{
/// @cond
#ifdef OCR4BL
#define _AVR_COUNTER4_B _AVR_COUNTER1_OCR(4, b, B)
#else
#define _AVR_COUNTER4_B
#endif
#ifdef OCR4CL
#define _AVR_COUNTER4_C _AVR_COUNTER1_OCR(4, c, C)
#else
#define _AVR_COUNTER4_C
#endif
	_AVR_COUNTER(4, _AVR_COUNTER4_B, _AVR_COUNTER4_C)

	/// Disable power to counter 4.
	static inline void off4() { PRR1 |= _BV(PRTIM4); }

	/// Enable power to counter 4.
	static inline void on4() { PRR1 &= ~_BV(PRTIM4); }
/// @endcond
#endif // }}}

#ifdef TCNT5L // {{{
/// @cond
#ifdef OCR5BL
#define _AVR_COUNTER5_B _AVR_COUNTER1_OCR(5, b, B)
#else
#define _AVR_COUNTER5_B
#endif
#ifdef OCR5CL
#define _AVR_COUNTER5_C _AVR_COUNTER1_OCR(5, c, C)
#else
#define _AVR_COUNTER5_C
#endif
	_AVR_COUNTER(5, _AVR_COUNTER5_B, _AVR_COUNTER5_C)

	/// Disable power to counter 5.
	static inline void off5() { PRR1 |= _BV(PRTIM5); }

	/// Enable power to counter 5.
	static inline void on5() { PRR1 &= ~_BV(PRTIM5); }
/// @endcond
#endif // }}}

	/// @}

	//

	/// @name System Clock 1
	/// @{

// Functions for using counter1 as a system clock. {{{
// Features:
// - busy wait
// - trigger interrupt at absolute or relative time
// - read current time

#ifdef DOXYGEN
/// Activate a system clock using Counter 1 and the ICR1 registers.
#define SYSTEM_CLOCK1_ENABLE_CAPT
/// Activate a system clock using Counter 1 and the OCR1A registers.
#define SYSTEM_CLOCK1_ENABLE_COMPA

/// Enable callback system_clock1_interrupt(), called when Counter::set_interrupt1() or Counter::set_timeout1() is called.
#define CALL_system_clock1_interrupt
#endif

#if defined(SYSTEM_CLOCK1_ENABLE_CAPT) || defined(SYSTEM_CLOCK1_ENABLE_COMPA)

#if defined(SYSTEM_CLOCK_DIVIDER) && !defined(SYSTEM_CLOCK1_DIVIDER)
#define SYSTEM_CLOCK1_DIVIDER SYSTEM_CLOCK_DIVIDER
#endif

#if defined(SYSTEM_CLOCK_TICKS_PER_UNIT) && !defined(SYSTEM_CLOCK1_TICKS_PER_UNIT)
#define SYSTEM_CLOCK1_TICKS_PER_UNIT SYSTEM_CLOCK_TICKS_PER_UNIT
#endif

#if defined(SYSTEM_CLOCK_TYPE) && !defined(SYSTEM_CLOCK1_TYPE)
#define SYSTEM_CLOCK1_TYPE SYSTEM_CLOCK_TYPE
#endif

#ifndef SYSTEM_CLOCK1_DIVIDER
/// Clock prescaler value when Counter1 is used as a system clock.
/**
 * When SYSTEM_CLOCK1_ENABLE_CAPT or SYSTEM_CLOCK1_ENABLE_COMPA is defined, Counter1 is running as a system clock.
 *
 * The default setup sets the source to Counter::s1_div1, which can count
 * milliseconds on any supported clock speed.
 *
 * Note that the only allowed values are those for which Counter::s1_div* are
 * defined in Counter::Source1.
 *
 * @sa SYSTEM_CLOCK1_TICKS_PER_UNIT
 */
#define SYSTEM_CLOCK1_DIVIDER 1
#endif

#ifndef SYSTEM_CLOCK1_TICKS_PER_UNIT
/// Prescaled clock ticks per time unit when Counter1 is used as a system clock.
/**
 * When SYSTEM_CLOCK1_ENABLE_CAPT or SYSTEM_CLOCK1_ENABLE_COMPA is defined, Counter1 is running as a system clock.
 *
 * This value defines how many prescaled clock ticks make up a time unit.
 *
 * The default value uses milliseconds as the unit. With a 16MHz crystal and
 * the default prescaler of 1, that makes this value 16000.
 *
 * @sa SYSTEM_CLOCK1_DIVIDER
 */
#define SYSTEM_CLOCK1_TICKS_PER_UNIT (F_CPU / SYSTEM_CLOCK1_DIVIDER / 1000)
#endif

#ifndef SYSTEM_CLOCK1_TYPE
/// Type of variable for time values.
/**
 * When SYSTEM_CLOCK1_ENABLE_CAPT or SYSTEM_CLOCK1_ENABLE_COMPA is defined, Counter1 is running as a system clock.
 *
 * This macro defines the type that is used for storing the time value and the
 * time interval argument to set_timeout.
 *
 * Larger types cost more memory (both SRAM to store them and flash to do
 * computations on them), but allow for longer timeouts.
 */
#define SYSTEM_CLOCK1_TYPE uint16_t
#endif

/// @cond
	static volatile SYSTEM_CLOCK1_TYPE counter1_time = 0;
#ifdef CALL_system_clock1_interrupt
	static volatile SYSTEM_CLOCK1_TYPE counter1_target;
	static volatile bool counter1_target_active = false;
#endif
#ifdef SYSTEM_CLOCK1_ENABLE_CAPT
#define _AVR_SETUP_COUNTER1 \
	Counter::set_icr1(SYSTEM_CLOCK1_TICKS_PER_UNIT); \
	Counter::enable1(COUNTER1_DIV_TO_SOURCE(SYSTEM_CLOCK1_DIVIDER), Counter::m1_ctc_icr); \
	Counter::enable_capt1();
#else
#define _AVR_SETUP_COUNTER1 \
	Counter::set_ocr1a(SYSTEM_CLOCK1_TICKS_PER_UNIT); \
	Counter::enable1(COUNTER1_DIV_TO_SOURCE(SYSTEM_CLOCK1_DIVIDER), Counter::m1_ctc_ocra); \
	Counter::enable_comp1();
#endif

#ifdef SYSTEM_CLOCK1_ENABLE_CAPT
#define _AVR_COUNTER_CHECK has_capt1
#define _AVR_COUNTER_CLEAR ICF1
ISR(TIMER1_CAPT_vect) {
#else
#define _AVR_COUNTER_CHECK has_ocf1a
#define _AVR_COUNTER_CLEAR OCF1A
ISR(TIMER1_COMPA_vect) {
#endif
	++Counter::counter1_time;
#ifdef CALL_system_clock1_interrupt
	if (Counter::counter1_target_active && (Counter::counter1_target == Counter::counter1_time)) {
		Counter::counter1_target_active = false;
		system_clock1_interrupt();
	}
#endif
}

/// @endcond

	/// Get current time (in units as stored, default is milliseconds) from counter 1.
	static inline SYSTEM_CLOCK1_TYPE get_time1() {
		return counter1_time;
	}

	/// Busy wait for a number of time units (default is milliseconds) using counter 1.
	static inline void busy_wait1(SYSTEM_CLOCK1_TYPE interval) {
		SYSTEM_CLOCK1_TYPE time = get_time1();
		SYSTEM_CLOCK1_TYPE target = time + interval;
		while (time != target) {
			// This may be called with interrupts disabled, so handle overflow events.
			// If interrupts are enabled, this code will never see the flag as set.
			if (!Counter::_AVR_COUNTER_CHECK()) {
				time = get_time1();
				continue;
			}
			TIFR1 |= _BV(_AVR_COUNTER_CLEAR);
			++counter1_time;
#ifdef CALL_system_clock1_interrupt
			if (counter1_target_active && (counter1_time == counter1_target)) {
				counter1_target_active = false;
				system_clock1_interrupt();
			}
#endif
			time = get_time1();
		}
	}

	/// @cond
#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
	// Macro is checked again and defined below.
	static inline SYSTEM_CLOCK1_TYPE get_time() { return get_time1(); }
	static inline void busy_wait(SYSTEM_CLOCK1_TYPE interval) { return busy_wait1(interval); }
#endif
	/// @endcond

#ifdef CALL_system_clock1_interrupt
	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Call system_clock1_interrupt at the specified time. Only one such
	 * interrupt can be scheduled at a time; if this function is called
	 * before the function is called, it will replace the previous setting.
	 *
	 * If check is true, a time in the past results in an immediate call.
	 *
	 * This function is only available if CALL_system_clock1_interrupt is
	 * defined.
	 *
	 * system_clock1_interrupt() must be defined by user code.
	 */
	static inline void set_interrupt1(SYSTEM_CLOCK1_TYPE time) {
		counter1_target = time;
		counter1_target_active = true;
	}

	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Identical to set_interrupt1(), but the argument is a relative time.
	 *
	 * This function is only available if CALL_system_clock1_interrupt is
	 * defined.
	 */
	static inline void set_timeout1(SYSTEM_CLOCK1_TYPE interval) {
		set_interrupt1(get_time1() + interval);
	}

/// @cond
#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
	static inline void set_interrupt(SYSTEM_CLOCK1_TYPE time) { return set_interrupt1(time); }
	static inline void set_timeout(SYSTEM_CLOCK1_TYPE interval) { return set_timeout1(interval); }
#endif
/// @endcond

#endif

/// @cond
#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
#define _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
#endif
/// @endcond

#endif
	// }}}

	/// @}

	/// @cond
#ifdef TCNT3L
// Functions for using counter3 as a system clock. {{{
// Features:
// - busy wait
// - trigger interrupt at absolute or relative time
// - read current time

#ifdef DOXYGEN
/// Activate a system clock using Counter 3 and the ICR3 registers.
#define SYSTEM_CLOCK3_ENABLE_CAPT
/// Activate a system clock using Counter 3 and the OCR3A registers.
#define SYSTEM_CLOCK3_ENABLE_COMPA

/// Enable callback system_clock3_interrupt(), called when Counter::set_interrupt3() or Counter::set_timeout3() is called.
#define CALL_system_clock3_interrupt
#endif

#if defined(SYSTEM_CLOCK3_ENABLE_CAPT) || defined(SYSTEM_CLOCK3_ENABLE_COMPA)

#if defined(SYSTEM_CLOCK_DIVIDER) && !defined(SYSTEM_CLOCK3_DIVIDER)
#define SYSTEM_CLOCK3_DIVIDER SYSTEM_CLOCK_DIVIDER
#endif

#if defined(SYSTEM_CLOCK_TICKS_PER_UNIT) && !defined(SYSTEM_CLOCK3_TICKS_PER_UNIT)
#define SYSTEM_CLOCK3_TICKS_PER_UNIT SYSTEM_CLOCK_TICKS_PER_UNIT
#endif

#if defined(SYSTEM_CLOCK_TYPE) && !defined(SYSTEM_CLOCK3_TYPE)
#define SYSTEM_CLOCK3_TYPE SYSTEM_CLOCK_TYPE
#endif

#ifndef SYSTEM_CLOCK3_DIVIDER
/// Clock prescaler value when Counter3 is used as a system clock.
/**
 * When SYSTEM_CLOCK3_ENABLE_CAPT or SYSTEM_CLOCK3_ENABLE_COMPA is defined, Counter3 is running as a system clock.
 *
 * The default setup sets the source to Counter::s3_div1, which can count
 * milliseconds on any supported clock speed.
 *
 * Note that the only allowed values are those for which Counter::s3_div* are
 * defined in Counter::Source3.
 *
 * @sa SYSTEM_CLOCK3_TICKS_PER_UNIT
 */
#define SYSTEM_CLOCK3_DIVIDER 1
#endif

#ifndef SYSTEM_CLOCK3_TICKS_PER_UNIT
/// Prescaled clock ticks per time unit when Counter3 is used as a system clock.
/**
 * When SYSTEM_CLOCK3_ENABLE_CAPT or SYSTEM_CLOCK3_ENABLE_COMPA is defined, Counter3 is running as a system clock.
 *
 * This value defines how many prescaled clock ticks make up a time unit.
 *
 * The default value uses milliseconds as the unit. With a 16MHz crystal and
 * the default prescaler of 1, that makes this value 16000.
 *
 * @sa SYSTEM_CLOCK3_DIVIDER
 */
#define SYSTEM_CLOCK3_TICKS_PER_UNIT (F_CPU / SYSTEM_CLOCK3_DIVIDER / 1000)
#endif

#ifndef SYSTEM_CLOCK3_TYPE
/// Type of variable for time values.
/**
 * When SYSTEM_CLOCK3_ENABLE_CAPT or SYSTEM_CLOCK3_ENABLE_COMPA is defined, Counter3 is running as a system clock.
 *
 * This macro defines the type that is used for storing the time value and the
 * time interval argument to set_timeout.
 *
 * Larger types cost more memory (both SRAM to store them and flash to do
 * computations on them), but allow for longer timeouts.
 */
#define SYSTEM_CLOCK3_TYPE uint16_t
#endif

	static volatile SYSTEM_CLOCK3_TYPE counter3_time = 0;
#ifdef CALL_system_clock3_interrupt
	static volatile SYSTEM_CLOCK3_TYPE counter3_target;
	static volatile bool counter3_target_active = false;
#endif
#ifdef SYSTEM_CLOCK3_ENABLE_CAPT
#define _AVR_SETUP_COUNTER3 \
	Counter::set_icr3(SYSTEM_CLOCK3_TICKS_PER_UNIT); \
	Counter::enable3(COUNTER1_DIV_TO_SOURCE(SYSTEM_CLOCK3_DIVIDER), Counter::m3_ctc_icr); \
	Counter::enable_capt3();
#else
#define _AVR_SETUP_COUNTER3 \
	Counter::set_ocr3a(SYSTEM_CLOCK3_TICKS_PER_UNIT); \
	Counter::enable3(COUNTER1_DIV_TO_SOURCE(SYSTEM_CLOCK3_DIVIDER), Counter::m3_ctc_ocra); \
	Counter::enable_comp3();
#endif

#ifdef SYSTEM_CLOCK3_ENABLE_CAPT
#define _AVR_COUNTER_CHECK has_capt3
#define _AVR_COUNTER_CLEAR ICF1
ISR(TIMER3_CAPT_vect) {
#else
#define _AVR_COUNTER_CHECK has_ocf3a
#define _AVR_COUNTER_CLEAR OCF3A
ISR(TIMER3_COMPA_vect) {
#endif
	++Counter::counter3_time;
#ifdef CALL_system_clock3_interrupt
	if (Counter::counter3_target_active && (Counter::counter3_target == Counter::counter3_time)) {
		Counter::counter3_target_active = false;
		system_clock3_interrupt();
	}
#endif
}

	/// Get current time (in units as stored, default is milliseconds) from counter 3.
	static inline SYSTEM_CLOCK3_TYPE get_time3() {
		return counter3_time;
	}

	/// Busy wait for a number of time units (default is milliseconds) using counter 3.
	static inline void busy_wait3(SYSTEM_CLOCK3_TYPE interval) {
		SYSTEM_CLOCK3_TYPE time = get_time3();
		SYSTEM_CLOCK3_TYPE target = time + interval;
		while (time != target) {
			// This may be called with interrupts disabled, so handle overflow events.
			// If interrupts are enabled, this code will never see the flag as set.
			if (!Counter::_AVR_COUNTER_CHECK()) {
				time = get_time3();
				continue;
			}
			TIFR3 |= _BV(_AVR_COUNTER_CLEAR);
			++counter3_time;
#ifdef CALL_system_clock3_interrupt
			if (counter3_target_active && (counter3_time == counter3_target)) {
				counter3_target_active = false;
				system_clock3_interrupt();
			}
#endif
			time = get_time3();
		}
	}

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
	// Macro is checked again and defined below.
	static inline SYSTEM_CLOCK3_TYPE get_time() { return get_time3(); }
	static inline void busy_wait(SYSTEM_CLOCK3_TYPE interval) { return busy_wait3(interval); }
#endif

#ifdef CALL_system_clock3_interrupt
	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Call system_clock3_interrupt at the specified time. Only one such
	 * interrupt can be scheduled at a time; if this function is called
	 * before the function is called, it will replace the previous setting.
	 *
	 * If check is true, a time in the past results in an immediate call.
	 *
	 * This function is only available if CALL_system_clock3_interrupt is
	 * defined.
	 *
	 * system_clock3_interrupt() must be defined by user code.
	 */
	static inline void set_interrupt3(SYSTEM_CLOCK3_TYPE time) {
		counter3_target = time;
		counter3_target_active = true;
	}

	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Identical to set_interrupt3(), but the argument is a relative time.
	 *
	 * This function is only available if CALL_system_clock3_interrupt is
	 * defined.
	 */
	static inline void set_timeout3(SYSTEM_CLOCK3_TYPE interval) {
		set_interrupt3(get_time3() + interval);
	}

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
	static inline void set_interrupt(SYSTEM_CLOCK3_TYPE time) { return set_interrupt3(time); }
	static inline void set_timeout(SYSTEM_CLOCK3_TYPE interval) { return set_timeout3(interval); }
#endif

#endif

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
#define _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
#endif

#endif
	// }}}
#endif

#if (defined(TCNT4L) && !defined(TCCR4E))
// Functions for using counter4 as a system clock. {{{
// Features:
// - busy wait
// - trigger interrupt at absolute or relative time
// - read current time

#ifdef DOXYGEN
/// Activate a system clock using Counter 4 and the ICR4 registers.
#define SYSTEM_CLOCK4_ENABLE_CAPT
/// Activate a system clock using Counter 4 and the OCR4A registers.
#define SYSTEM_CLOCK4_ENABLE_COMPA

/// Enable callback system_clock4_interrupt(), called when Counter::set_interrupt4() or Counter::set_timeout4() is called.
#define CALL_system_clock4_interrupt
#endif

#if defined(SYSTEM_CLOCK4_ENABLE_CAPT) || defined(SYSTEM_CLOCK4_ENABLE_COMPA)

#if defined(SYSTEM_CLOCK_DIVIDER) && !defined(SYSTEM_CLOCK4_DIVIDER)
#define SYSTEM_CLOCK4_DIVIDER SYSTEM_CLOCK_DIVIDER
#endif

#if defined(SYSTEM_CLOCK_TICKS_PER_UNIT) && !defined(SYSTEM_CLOCK4_TICKS_PER_UNIT)
#define SYSTEM_CLOCK4_TICKS_PER_UNIT SYSTEM_CLOCK_TICKS_PER_UNIT
#endif

#if defined(SYSTEM_CLOCK_TYPE) && !defined(SYSTEM_CLOCK4_TYPE)
#define SYSTEM_CLOCK4_TYPE SYSTEM_CLOCK_TYPE
#endif

#ifndef SYSTEM_CLOCK4_DIVIDER
/// Clock prescaler value when Counter4 is used as a system clock.
/**
 * When SYSTEM_CLOCK4_ENABLE_CAPT or SYSTEM_CLOCK4_ENABLE_COMPA is defined, Counter4 is running as a system clock.
 *
 * The default setup sets the source to Counter::s4_div1, which can count
 * milliseconds on any supported clock speed.
 *
 * Note that the only allowed values are those for which Counter::s4_div* are
 * defined in Counter::Source4.
 *
 * @sa SYSTEM_CLOCK4_TICKS_PER_UNIT
 */
#define SYSTEM_CLOCK4_DIVIDER 1
#endif

#ifndef SYSTEM_CLOCK4_TICKS_PER_UNIT
/// Prescaled clock ticks per time unit when Counter4 is used as a system clock.
/**
 * When SYSTEM_CLOCK4_ENABLE_CAPT or SYSTEM_CLOCK4_ENABLE_COMPA is defined, Counter4 is running as a system clock.
 *
 * This value defines how many prescaled clock ticks make up a time unit.
 *
 * The default value uses milliseconds as the unit. With a 16MHz crystal and
 * the default prescaler of 1, that makes this value 16000.
 *
 * @sa SYSTEM_CLOCK4_DIVIDER
 */
#define SYSTEM_CLOCK4_TICKS_PER_UNIT (F_CPU / SYSTEM_CLOCK4_DIVIDER / 1000)
#endif

#ifndef SYSTEM_CLOCK4_TYPE
/// Type of variable for time values.
/**
 * When SYSTEM_CLOCK4_ENABLE_CAPT or SYSTEM_CLOCK4_ENABLE_COMPA is defined, Counter4 is running as a system clock.
 *
 * This macro defines the type that is used for storing the time value and the
 * time interval argument to set_timeout.
 *
 * Larger types cost more memory (both SRAM to store them and flash to do
 * computations on them), but allow for longer timeouts.
 */
#define SYSTEM_CLOCK4_TYPE uint16_t
#endif

	static volatile SYSTEM_CLOCK4_TYPE counter4_time = 0;
#ifdef CALL_system_clock4_interrupt
	static volatile SYSTEM_CLOCK4_TYPE counter4_target;
	static volatile bool counter4_target_active = false;
#endif
#ifdef SYSTEM_CLOCK4_ENABLE_CAPT
#define _AVR_SETUP_COUNTER4 \
	Counter::set_icr4(SYSTEM_CLOCK4_TICKS_PER_UNIT); \
	Counter::enable4(COUNTER1_DIV_TO_SOURCE(SYSTEM_CLOCK4_DIVIDER), Counter::m4_ctc_icr); \
	Counter::enable_capt4();
#else
#define _AVR_SETUP_COUNTER4 \
	Counter::set_ocr4a(SYSTEM_CLOCK4_TICKS_PER_UNIT); \
	Counter::enable4(COUNTER1_DIV_TO_SOURCE(SYSTEM_CLOCK4_DIVIDER), Counter::m4_ctc_ocra); \
	Counter::enable_comp4();
#endif

#ifdef SYSTEM_CLOCK4_ENABLE_CAPT
#define _AVR_COUNTER_CHECK has_capt4
#define _AVR_COUNTER_CLEAR ICF4
ISR(TIMER4_CAPT_vect) {
#else
#define _AVR_COUNTER_CHECK has_ocf4a
#define _AVR_COUNTER_CLEAR OCF4A
ISR(TIMER4_COMPA_vect) {
#endif
	++Counter::counter4_time;
#ifdef CALL_system_clock4_interrupt
	if (Counter::counter4_target_active && (Counter::counter4_target == Counter::counter4_time)) {
		Counter::counter4_target_active = false;
		system_clock4_interrupt();
	}
#endif
}

	/// Get current time (in units as stored, default is milliseconds) from counter 4.
	static inline SYSTEM_CLOCK4_TYPE get_time4() {
		return counter4_time;
	}

	/// Busy wait for a number of time units (default is milliseconds) using counter 4.
	static inline void busy_wait4(SYSTEM_CLOCK4_TYPE interval) {
		SYSTEM_CLOCK4_TYPE time = get_time4();
		SYSTEM_CLOCK4_TYPE target = time + interval;
		while (time != target) {
			// This may be called with interrupts disabled, so handle overflow events.
			// If interrupts are enabled, this code will never see the flag as set.
			if (!Counter::_AVR_COUNTER_CHECK()) {
				time = get_time4();
				continue;
			}
			TIFR4 |= _BV(_AVR_COUNTER_CLEAR);
			++counter4_time;
#ifdef CALL_system_clock4_interrupt
			if (counter4_target_active && (counter4_time == counter4_target)) {
				counter4_target_active = false;
				system_clock4_interrupt();
			}
#endif
			time = get_time4();
		}
	}

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
	// Macro is checked again and defined below.
	static inline SYSTEM_CLOCK4_TYPE get_time() { return get_time4(); }
	static inline void busy_wait(SYSTEM_CLOCK4_TYPE interval) { return busy_wait4(interval); }
#endif

#ifdef CALL_system_clock4_interrupt
	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Call system_clock4_interrupt at the specified time. Only one such
	 * interrupt can be scheduled at a time; if this function is called
	 * before the function is called, it will replace the previous setting.
	 *
	 * If check is true, a time in the past results in an immediate call.
	 *
	 * This function is only available if CALL_system_clock4_interrupt is
	 * defined.
	 *
	 * system_clock4_interrupt() must be defined by user code.
	 */
	static inline void set_interrupt4(SYSTEM_CLOCK4_TYPE time) {
		counter4_target = time;
		counter4_target_active = true;
	}

	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Identical to set_interrupt4(), but the argument is a relative time.
	 *
	 * This function is only available if CALL_system_clock4_interrupt is
	 * defined.
	 */
	static inline void set_timeout4(SYSTEM_CLOCK4_TYPE interval) {
		set_interrupt4(get_time4() + interval);
	}

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
	static inline void set_interrupt(SYSTEM_CLOCK4_TYPE time) { return set_interrupt4(time); }
	static inline void set_timeout(SYSTEM_CLOCK4_TYPE interval) { return set_timeout4(interval); }
#endif

#endif

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
#define _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
#endif

#endif
	// }}}
#endif

#ifdef TCNT5L
// Functions for using counter5 as a system clock. {{{
// Features:
// - busy wait
// - trigger interrupt at absolute or relative time
// - read current time

#ifdef DOXYGEN
/// Activate a system clock using Counter 5 and the ICR5 registers.
#define SYSTEM_CLOCK5_ENABLE_CAPT
/// Activate a system clock using Counter 5 and the OCR5A registers.
#define SYSTEM_CLOCK5_ENABLE_COMPA

/// Enable callback system_clock5_interrupt(), called when Counter::set_interrupt5() or Counter::set_timeout5() is called.
#define CALL_system_clock5_interrupt
#endif

#if defined(SYSTEM_CLOCK5_ENABLE_CAPT) || defined(SYSTEM_CLOCK5_ENABLE_COMPA)

#if defined(SYSTEM_CLOCK_DIVIDER) && !defined(SYSTEM_CLOCK5_DIVIDER)
#define SYSTEM_CLOCK5_DIVIDER SYSTEM_CLOCK_DIVIDER
#endif

#if defined(SYSTEM_CLOCK_TICKS_PER_UNIT) && !defined(SYSTEM_CLOCK5_TICKS_PER_UNIT)
#define SYSTEM_CLOCK5_TICKS_PER_UNIT SYSTEM_CLOCK_TICKS_PER_UNIT
#endif

#if defined(SYSTEM_CLOCK_TYPE) && !defined(SYSTEM_CLOCK5_TYPE)
#define SYSTEM_CLOCK5_TYPE SYSTEM_CLOCK_TYPE
#endif

#ifndef SYSTEM_CLOCK5_DIVIDER
/// Clock prescaler value when Counter5 is used as a system clock.
/**
 * When SYSTEM_CLOCK5_ENABLE_CAPT or SYSTEM_CLOCK5_ENABLE_COMPA is defined, Counter5 is running as a system clock.
 *
 * The default setup sets the source to Counter::s5_div1, which can count
 * milliseconds on any supported clock speed.
 *
 * Note that the only allowed values are those for which Counter::s5_div* are
 * defined in Counter::Source5.
 *
 * @sa SYSTEM_CLOCK5_TICKS_PER_UNIT
 */
#define SYSTEM_CLOCK5_DIVIDER 1
#endif

#ifndef SYSTEM_CLOCK5_TICKS_PER_UNIT
/// Prescaled clock ticks per time unit when Counter5 is used as a system clock.
/**
 * When SYSTEM_CLOCK5_ENABLE_CAPT or SYSTEM_CLOCK5_ENABLE_COMPA is defined, Counter5 is running as a system clock.
 *
 * This value defines how many prescaled clock ticks make up a time unit.
 *
 * The default value uses milliseconds as the unit. With a 16MHz crystal and
 * the default prescaler of 1, that makes this value 16000.
 *
 * @sa SYSTEM_CLOCK5_DIVIDER
 */
#define SYSTEM_CLOCK5_TICKS_PER_UNIT (F_CPU / SYSTEM_CLOCK5_DIVIDER / 1000)
#endif

#ifndef SYSTEM_CLOCK5_TYPE
/// Type of variable for time values.
/**
 * When SYSTEM_CLOCK5_ENABLE_CAPT or SYSTEM_CLOCK5_ENABLE_COMPA is defined, Counter5 is running as a system clock.
 *
 * This macro defines the type that is used for storing the time value and the
 * time interval argument to set_timeout.
 *
 * Larger types cost more memory (both SRAM to store them and flash to do
 * computations on them), but allow for longer timeouts.
 */
#define SYSTEM_CLOCK5_TYPE uint16_t
#endif

	static volatile SYSTEM_CLOCK5_TYPE counter5_time = 0;
#ifdef CALL_system_clock5_interrupt
	static volatile SYSTEM_CLOCK5_TYPE counter5_target;
	static volatile bool counter5_target_active = false;
#endif
#ifdef SYSTEM_CLOCK5_ENABLE_CAPT
#define _AVR_SETUP_COUNTER5 \
	Counter::set_icr5(SYSTEM_CLOCK5_TICKS_PER_UNIT); \
	Counter::enable5(COUNTER1_DIV_TO_SOURCE(SYSTEM_CLOCK5_DIVIDER), Counter::m5_ctc_icr); \
	Counter::enable_capt5();
#else
#define _AVR_SETUP_COUNTER5 \
	Counter::set_ocr5a(SYSTEM_CLOCK5_TICKS_PER_UNIT); \
	Counter::enable5(COUNTER1_DIV_TO_SOURCE(SYSTEM_CLOCK5_DIVIDER), Counter::m5_ctc_ocra); \
	Counter::enable_comp5();
#endif

#ifdef SYSTEM_CLOCK5_ENABLE_CAPT
#define _AVR_COUNTER_CHECK has_capt5
#define _AVR_COUNTER_CLEAR ICF5
ISR(TIMER5_CAPT_vect) {
#else
#define _AVR_COUNTER_CHECK has_ocf5a
#define _AVR_COUNTER_CLEAR OCF5A
ISR(TIMER5_COMPA_vect) {
#endif
	++Counter::counter5_time;
#ifdef CALL_system_clock5_interrupt
	if (Counter::counter5_target_active && (Counter::counter5_target == Counter::counter5_time)) {
		Counter::counter5_target_active = false;
		system_clock5_interrupt();
	}
#endif
}

	/// Get current time (in units as stored, default is milliseconds) from counter 5.
	static inline SYSTEM_CLOCK5_TYPE get_time5() {
		return counter5_time;
	}

	/// Busy wait for a number of time units (default is milliseconds) using counter 5.
	static inline void busy_wait5(SYSTEM_CLOCK5_TYPE interval) {
		SYSTEM_CLOCK5_TYPE time = get_time5();
		SYSTEM_CLOCK5_TYPE target = time + interval;
		while (time != target) {
			// This may be called with interrupts disabled, so handle overflow events.
			// If interrupts are enabled, this code will never see the flag as set.
			if (!Counter::_AVR_COUNTER_CHECK()) {
				time = get_time5();
				continue;
			}
			TIFR5 |= _BV(_AVR_COUNTER_CLEAR);
			++counter5_time;
#ifdef CALL_system_clock5_interrupt
			if (counter5_target_active && (counter5_time == counter5_target)) {
				counter5_target_active = false;
				system_clock5_interrupt();
			}
#endif
			time = get_time5();
		}
	}

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
	// Macro is checked again and defined below.
	static inline SYSTEM_CLOCK5_TYPE get_time() { return get_time5(); }
	static inline void busy_wait(SYSTEM_CLOCK5_TYPE interval) { return busy_wait5(interval); }
#endif

#ifdef CALL_system_clock5_interrupt
	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Call system_clock5_interrupt at the specified time. Only one such
	 * interrupt can be scheduled at a time; if this function is called
	 * before the function is called, it will replace the previous setting.
	 *
	 * If check is true, a time in the past results in an immediate call.
	 *
	 * This function is only available if CALL_system_clock5_interrupt is
	 * defined.
	 *
	 * system_clock5_interrupt() must be defined by user code.
	 */
	static inline void set_interrupt5(SYSTEM_CLOCK5_TYPE time) {
		counter5_target = time;
		counter5_target_active = true;
	}

	/// Generate "interrupt" when counter reaches a certain value.
	/**
	 * Identical to set_interrupt5(), but the argument is a relative time.
	 *
	 * This function is only available if CALL_system_clock5_interrupt is
	 * defined.
	 */
	static inline void set_timeout5(SYSTEM_CLOCK5_TYPE interval) {
		set_interrupt5(get_time5() + interval);
	}

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
	static inline void set_interrupt(SYSTEM_CLOCK5_TYPE time) { return set_interrupt5(time); }
	static inline void set_timeout(SYSTEM_CLOCK5_TYPE interval) { return set_timeout5(interval); }
#endif

#endif

#ifndef _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
#define _AVR_SYSTEM_CLOCK_HAVE_DEFAULT
#endif

#endif
	// }}}
#endif
	/// @endcond

}

#ifdef AVR_TEST_COUNTER1 // {{{

#if AVR_TEST_INDEX == 0

#ifdef TCNT3L
#define _AVR_COUNTER1_3(pre, post) \
	if (part == '3') { \
		pre ## 3 ## post \
	} else
#define _AVR_COUNTER1_3f(pre, post) \
	if (part == '3') { \
		pre ## 3 post \
	} else
#else
#define _AVR_COUNTER1_3(pre, post)
#define _AVR_COUNTER1_3f(pre, post)
#endif

#if defined(TCNT4L) && !defined(TCCR4E)
#define _AVR_COUNTER1_4(pre, post) \
	if (part == '4') { \
		pre ## 4 ## post \
	} else
#define _AVR_COUNTER1_4f(pre, post) \
	if (part == '4') { \
		pre ## 4 post \
	} else
#else
#define _AVR_COUNTER1_4(pre, post)
#define _AVR_COUNTER1_4f(pre, post)
#endif

#ifdef TCNT5L
#define _AVR_COUNTER1_5(pre, post) \
	if (part == '5') { \
		pre ## 5 ## post \
	} else
#define _AVR_COUNTER1_5f(pre, post) \
	if (part == '5') { \
		pre ## 5 post \
	} else
#else
#define _AVR_COUNTER1_5(pre, post)
#define _AVR_COUNTER1_5f(pre, post)
#endif

#define _AVR_COUNTER1_SPLIT(pre, post) \
	do { \
	_AVR_COUNTER1_5(pre, post) \
	_AVR_COUNTER1_4(pre, post) \
	_AVR_COUNTER1_3(pre, post) \
	{ pre ## 1 ## post } \
	} while (false)

#define _AVR_COUNTER1_SPLIT_F(pre, post) \
	do { \
	_AVR_COUNTER1_5f(pre, post) \
	_AVR_COUNTER1_4f(pre, post) \
	_AVR_COUNTER1_3f(pre, post) \
	{ pre ## 1 post } \
	} while (false)

namespace Counter {
	static const uint8_t testcode1 = '1';
	static const uint8_t testcode3 = '3';
	static const uint8_t testcode4 = '4';
	static const uint8_t testcode5 = '5';
	static bool test1(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode1 && part != testcode3 && part != testcode4 && part != testcode5)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(part);
			Test::tx('e');
			Test::tx('d');
			Test::tx('a');
			Test::tx('b');
			//Test::tx('c');
			Test::tx('o');
			Test::tx('O');
			Test::tx('i');
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
		//	a+T	enable oc*a with type T
		//	a-	disable oc*a
		//	a!	enable compa interrupt
		//	a.	disable compa interrupt
		//	a=XXXX	set OCR*A register
		//	ar	get OCR*A register
		//	b+T	enable oc*b with type T
		//	b-	disable oc*b
		//	b!	enable compb interrupt
		//	b.	disable compb interrupt
		//	b=XXXX	set OCR*B register
		//	br	get OCR*B register
		//	c+T	enable oc*c with type T
		//	c-	disable oc*c
		//	c!	enable compc interrupt
		//	c.	disable compc interrupt
		//	c=XXXX	set OCR*C register
		//	c?	get OCR*C register
		//	o	enable ovf
		//	O	disable ovf
		//	ir	get icr
		//	i=XXXX	set icr
		//	i!	enable icp interrupt
		//	i.	disable icp interrupt
		//	r	read value
		//	wXXXX	write value
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
#define _AVR_COUNTER1_TMP(N) \
	if (part == '0' + N) \
		enable ## N(static_cast <Source ## N>(source), static_cast <Mode ## N>(mode)); \
	else
#ifdef TCNT5L
			_AVR_COUNTER1_TMP(5)
#endif
#if (defined(TCNT4L) && !defined(TCCR4E))
			_AVR_COUNTER1_TMP(4)
#endif
#ifdef TCNT3L
			_AVR_COUNTER1_TMP(3)
#endif
			_AVR_COUNTER1_TMP(1)
				return false;
			break;
#undef _AVR_COUNTER1_TMP
		}
		case 'd':
			if (len != 0)
				return false;
			_AVR_COUNTER1_SPLIT_F(disable, (););
			break;
		case 'a':
		case 'b':
		//case 'c': // TODO: support OC*C
			if (len < 1)
				return false;
			switch (Test::rx_read(0)) {
			case '+':
			{
				if (len != 2)
					return false;
				uint8_t type = Test::read_digit(1);
				if (cmd == 'a')
					_AVR_COUNTER1_SPLIT(enable_oc, a(type););
				else if (cmd == 'b')
					_AVR_COUNTER1_SPLIT(enable_oc, b(type););
				//else
				//	_AVR_COUNTER1_SPLIT(enable_oc, c(type););
				break;
			}
			case '-':
				if (len != 1)
					return false;
				if (cmd == 'a')
					_AVR_COUNTER1_SPLIT(disable_oc, a(););
				else if (cmd == 'b')
					_AVR_COUNTER1_SPLIT(disable_oc, b(););
				//else
				//	_AVR_COUNTER1_SPLIT(disable_oc, c(););
				break;
			case '!':
				if (len != 1)
					return false;
				if (cmd == 'a')
					_AVR_COUNTER1_SPLIT_F(enable_compa, (););
				else if (cmd == 'b')
					_AVR_COUNTER1_SPLIT_F(enable_compb, (););
				//else
				//	_AVR_COUNTER1_SPLIT_F(enable_compc, (););
				break;
			case '.':
				if (len != 1)
					return false;
				if (cmd == 'a')
					_AVR_COUNTER1_SPLIT_F(disable_compa, (););
				else if (cmd == 'b')
					_AVR_COUNTER1_SPLIT_F(disable_compb, (););
				//else
				//	_AVR_COUNTER1_SPLIT_F(disable_compc, (););
				break;
			case '=':
			{
				if (len != 5)
					return false;
				bool ok = true;
				uint8_t h = Test::read_byte(1, ok);
				uint8_t l = Test::read_byte(3, ok);
				if (!ok)
					return false;
				uint16_t hl = (h << 8) | l;
				if (cmd == 'a')
					_AVR_COUNTER1_SPLIT(set_ocr, a(hl););
				else if (cmd == 'b')
					_AVR_COUNTER1_SPLIT(set_ocr, b(hl););
				//else
				//	_AVR_COUNTER1_SPLIT(set_ocr, c(hl););
				break;
			}
			case 'r':
			{
				if (len != 1)
					return false;
				uint16_t value;
				if (cmd == 'a')
					_AVR_COUNTER1_SPLIT(value = get_ocr, a(););
				if (cmd == 'b')
					_AVR_COUNTER1_SPLIT(value = get_ocr, b(););
				//if (cmd == 'c')
				//	_AVR_COUNTER1_SPLIT(value = get_ocr, c(););
				Test::tx(part);
				Test::send_byte(value >> 8);
				Test::send_byte(value & 0xff);
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
			_AVR_COUNTER1_SPLIT_F(enable_ovf, (););
			break;
		case 'O':
			if (len != 0)
				return false;
			_AVR_COUNTER1_SPLIT_F(disable_ovf, (););
			break;
		case 'i':
			if (len < 1)
				return false;
			switch (Test::rx_read(0)) {
			case '!':
				if (len != 1)
					return false;
				_AVR_COUNTER1_SPLIT_F(enable_capt, (););
				break;
			case '.':
				if (len != 1)
					return false;
				_AVR_COUNTER1_SPLIT_F(disable_capt, (););
				break;
			case '=':
			{
				if (len != 5)
					return false;
				bool ok = true;
				uint8_t h = Test::read_byte(1, ok);
				uint8_t l = Test::read_byte(3, ok);
				if (!ok)
					return false;
				uint16_t hl = (h << 8) | l;
				_AVR_COUNTER1_SPLIT_F(set_icr, (hl););
				break;
			}
			case 'r':
			{
				if (len != 1)
					return false;
				uint16_t value;
				_AVR_COUNTER1_SPLIT_F(value = get_icr, (););
				Test::tx(part);
				Test::send_byte(value >> 8);
				Test::send_byte(value & 0xff);
				Test::tx('\n');
				break;
			}
			default:
				return false;
			}
			break;
		case 'r':
		{
			if (len != 0)
				return false;
			uint16_t value;
			_AVR_COUNTER1_SPLIT_F(value = read, (););
			Test::tx(part);
			Test::send_byte(value >> 8);
			Test::send_byte(value & 0xff);
			Test::tx('\n');
			break;
		}
		case 'w':
		{
			if (len != 5)
				return false;
			bool ok = true;
			uint8_t h = Test::read_byte(1, ok);
			uint8_t l = Test::read_byte(3, ok);
			if (!ok)
				return false;
			uint16_t hl = (h << 8) | l;
			_AVR_COUNTER1_SPLIT_F(write, (hl););
			break;
		}
		case 'F':
			if (len != 0)
				return false;
			_AVR_COUNTER1_SPLIT_F(clear_ints, (););
			break;
#ifdef PRR0
		case '-':
			if (len != 0)
				return false;
			_AVR_COUNTER1_SPLIT_F(off, (););
			break;
		case '+':
			if (len != 0)
				return false;
			_AVR_COUNTER1_SPLIT_F(on, (););
			break;
#endif
		default:
			return false;
		}
		return true;
	}
}

#define _AVR_COUNTER1_ISRS(N) \
ISR(TIMER ## N ## _COMPA_vect) { \
	Test::tx('0' + N); \
	Test::tx('A'); \
	Test::tx('\n'); \
} \
ISR(TIMER ## N ## _COMPB_vect) { \
	Test::tx('0' + N); \
	Test::tx('B'); \
	Test::tx('\n'); \
} \
ISR(TIMER ## N ## _CAPT_vect) { \
	Test::tx('0' + N); \
	Test::tx('I'); \
	Test::tx('\n'); \
} \
ISR(TIMER ## N ## _OVF_vect) { \
	Test::tx('0' + N); \
	Test::tx('O'); \
	Test::tx('\n'); \
}
// TODO: support OC*C.

_AVR_COUNTER1_ISRS(1)
#ifdef TCNT3L
_AVR_COUNTER1_ISRS(3)
#endif
#if defined(TCNT4L) && !defined(TCCR4E)
_AVR_COUNTER1_ISRS(4)
#endif
#ifdef TCNT5L
_AVR_COUNTER1_ISRS(5)
#endif

#else

namespace Counter {
	static const uint8_t testcode1 = 0;
	static const uint8_t testcode3 = 0;
	static const uint8_t testcode4 = 0;
	static const uint8_t testcode5 = 0;
	static bool test1(uint8_t part, uint8_t cmd, uint8_t len) {
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
