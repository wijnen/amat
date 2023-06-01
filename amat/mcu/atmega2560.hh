#ifndef _AVR_ATMEGA2560
#define _AVR_ATMEGA2560

// Timers {{{
#define PIN_OC0A GPIO_MAKE_PIN(PB, 7)
#define PIN_OC0B GPIO_MAKE_PIN(PG, 5)
#define PIN_T0 GPIO_MAKE_PIN(PD, 7)

#define PIN_OC1A GPIO_MAKE_PIN(PB, 5)
#define PIN_OC1B GPIO_MAKE_PIN(PB, 6)
#define PIN_OC1C GPIO_MAKE_PIN(PB, 7)
#define PIN_T1 GPIO_MAKE_PIN(PD, 6)
#define PIN_ICP1 GPIO_MAKE_PIN(PD, 4)

#define PIN_OC2A GPIO_MAKE_PIN(PB, 4)
#ifdef PORTH
#define PIN_OC2B GPIO_MAKE_PIN(PH, 6)
#endif

#define PIN_OC3A GPIO_MAKE_PIN(PE, 3)
#define PIN_OC3B GPIO_MAKE_PIN(PE, 4)
#define PIN_OC3C GPIO_MAKE_PIN(PE, 5)
#define PIN_T3 GPIO_MAKE_PIN(PE, 6)
#define PIN_ICP3 GPIO_MAKE_PIN(PE, 7)

#ifdef PORTH
#define PIN_OC4A GPIO_MAKE_PIN(PH, 3)
#define PIN_OC4B GPIO_MAKE_PIN(PH, 4)
#define PIN_OC4C GPIO_MAKE_PIN(PH, 5)
#define PIN_T4 GPIO_MAKE_PIN(PH, 7)
#define PIN_ICP4 GPIO_MAKE_PIN(PL, 0)

#define PIN_OC5A GPIO_MAKE_PIN(PL, 3)
#define PIN_OC5B GPIO_MAKE_PIN(PL, 4)
#define PIN_OC5C GPIO_MAKE_PIN(PL, 5)
#define PIN_T5 GPIO_MAKE_PIN(PL, 2)
#define PIN_ICP5 GPIO_MAKE_PIN(PL, 1)
#endif
// }}}

// Adc {{{
#define _AVR_NUM_ADC_PINS 8
#define _AVR_HAVE_REF_2_56V
#define PIN_ADC0 GPIO_MAKE_PIN(PF, 0)
#define PIN_ADC1 GPIO_MAKE_PIN(PF, 1)
#define PIN_ADC2 GPIO_MAKE_PIN(PF, 2)
#define PIN_ADC3 GPIO_MAKE_PIN(PF, 3)
#define PIN_ADC4 GPIO_MAKE_PIN(PF, 4)
#define PIN_ADC5 GPIO_MAKE_PIN(PF, 5)
#define PIN_ADC6 GPIO_MAKE_PIN(PF, 6)
#define PIN_ADC7 GPIO_MAKE_PIN(PF, 7)
#ifdef PORTK
#define PIN_ADC8 GPIO_MAKE_PIN(PK, 0)
#define PIN_ADC9 GPIO_MAKE_PIN(PK, 1)
#define PIN_ADC10 GPIO_MAKE_PIN(PK, 2)
#define PIN_ADC11 GPIO_MAKE_PIN(PK, 3)
#define PIN_ADC12 GPIO_MAKE_PIN(PK, 4)
#define PIN_ADC13 GPIO_MAKE_PIN(PK, 5)
#define PIN_ADC14 GPIO_MAKE_PIN(PK, 6)
#define PIN_ADC15 GPIO_MAKE_PIN(PK, 7)
#endif
namespace Adc {
	enum Source {
		SRC_A0,
		SRC_A1,
		SRC_A2,
		SRC_A3,
		SRC_A4,
		SRC_A5,
		SRC_A6,
		SRC_A7,
		//
		SRC_A0_A0_10x,
		SRC_A1_A0_10x,
		SRC_A0_A0_200x,
		SRC_A1_A0_200x,
		SRC_A2_A2_10x,
		SRC_A3_A2_10x,
		SRC_A2_A2_200x,
		SRC_A3_A2_200x,
		//
		SRC_A0_A1,
		SRC_A1_A1,
		SRC_A2_A1,
		SRC_A3_A1,
		SRC_A4_A1,
		SRC_A5_A1,
		SRC_A6_A1,
		SRC_A7_A1,
		//
		SRC_A0_A2,
		SRC_A1_A2,
		SRC_A2_A2,
		SRC_A3_A2,
		SRC_A4_A2,
		SRC_A5_A2,
		SRC_1V1,
		SRC_GND,
		//
#ifdef PORTK
		SRC_A8,
		SRC_A9,
		SRC_A10,
		SRC_A11,
		SRC_A12,
		SRC_A13,
		SRC_A14,
		SRC_A15,
		//
		SRC_A8_A8_10x,
		SRC_A9_A8_10x,
		SRC_A8_A8_200x,
		SRC_A9_A8_200x,
		SRC_A10_A10_10x,
		SRC_A11_A10_10x,
		SRC_A10_A10_200x,
		SRC_A11_A10_200x,
		//
		SRC_A8_A9,
		SRC_A9_A9,
		SRC_A10_A9,
		SRC_A11_A9,
		SRC_A12_A9,
		SRC_A13_A9,
		SRC_A14_A9,
		SRC_A15_A9,
		//
		SRC_A8_A10,
		SRC_A9_A10,
		SRC_A10_A10,
		SRC_A11_A10,
		SRC_A12_A10,
		SRC_A13_A10
#endif
	};
}
#ifdef PORTK
#define _AVR_ADC_EXTRA_MAP \
	, {8, 0xff, 1}, \
	{9, 0xff, 1}, \
	{10, 0xff, 1}, \
	{11, 0xff, 1}, \
	{12, 0xff, 1}, \
	{13, 0xff, 1}, \
	{14, 0xff, 1}, \
	{15, 0xff, 1}, \
	{8, 8, 10}, \
	{9, 8, 10}, \
	{8, 8, 200}, \
	{9, 8, 200}, \
	{10, 10, 10}, \
	{11, 10, 10}, \
	{10, 10, 200}, \
	{11, 10, 200}, \
	{8, 9, 1}, \
	{9, 9, 1}, \
	{10, 9, 1}, \
	{11, 9, 1}, \
	{12, 9, 1}, \
	{13, 9, 1}, \
	{14, 9, 1}, \
	{15, 9, 1}, \
	{8, 10, 1}, \
	{9, 10, 1}, \
	{10, 10, 1}, \
	{11, 10, 1}, \
	{12, 10, 1}, \
	{13, 10, 1}
#else
#define _AVR_ADC_EXTRA_MAP
#endif
#define _AVR_ADC_MAP { \
	{0, 0xff, 1}, \
	{1, 0xff, 1}, \
	{2, 0xff, 1}, \
	{3, 0xff, 1}, \
	{4, 0xff, 1}, \
	{5, 0xff, 1}, \
	{6, 0xff, 1}, \
	{7, 0xff, 1}, \
	{0, 0, 10}, \
	{1, 0, 10}, \
	{0, 0, 200}, \
	{1, 0, 200}, \
	{2, 2, 10}, \
	{3, 2, 10}, \
	{2, 2, 200}, \
	{3, 2, 200}, \
	{0, 1, 1}, \
	{1, 1, 1}, \
	{2, 1, 1}, \
	{3, 1, 1}, \
	{4, 1, 1}, \
	{5, 1, 1}, \
	{6, 1, 1}, \
	{7, 1, 1}, \
	{0, 2, 1}, \
	{1, 2, 1}, \
	{2, 2, 1}, \
	{3, 2, 1}, \
	{4, 2, 1}, \
	{5, 2, 1}, \
	{ADC_SRC_1V1, 0xff, 1}, \
	{ADC_SRC_GND, 0xff, 1} \
	_AVR_ADC_EXTRA_MAP \
}
#define _AVR_ADC_MAP_SIZE 0x3e
// }}}

// Comparator {{{
#define PIN_AIN0 GPIO_MAKE_PIN(PE, 2)
#define PIN_AIN1 GPIO_MAKE_PIN(PE, 3)
// }}}

// Twi {{{
#define PIN_SCL GPIO_MAKE_PIN(PD, 0)
#define PIN_SDA GPIO_MAKE_PIN(PD, 1)
// }}}

// Usart {{{
#define PIN_RXD0 GPIO_MAKE_PIN(PE, 0)
#define PIN_TXD0 GPIO_MAKE_PIN(PE, 1)
#define PIN_XCK0 GPIO_MAKE_PIN(PE, 2)

#define PIN_RXD1 GPIO_MAKE_PIN(PD, 2)
#define PIN_TXD1 GPIO_MAKE_PIN(PD, 3)
#define PIN_XCK1 GPIO_MAKE_PIN(PD, 5)

#ifdef PORTH
#define PIN_RXD2 GPIO_MAKE_PIN(PH, 0)
#define PIN_TXD2 GPIO_MAKE_PIN(PH, 1)
#define PIN_XCK2 GPIO_MAKE_PIN(PH, 2)

#define PIN_RXD3 GPIO_MAKE_PIN(PJ, 0)
#define PIN_TXD3 GPIO_MAKE_PIN(PJ, 1)
#define PIN_XCK3 GPIO_MAKE_PIN(PJ, 2)
#endif
// }}}

// Spi {{{
#define PIN_SS GPIO_MAKE_PIN(PB, 0)
#define PIN_SCK GPIO_MAKE_PIN(PB, 1)
#define PIN_MOSI GPIO_MAKE_PIN(PB, 2)
#define PIN_MISO GPIO_MAKE_PIN(PB, 3)
// }}}

// Pcint {{{
#ifdef PORTJ
#define _AVR_NUM_PCINT_PINS 24
#define _AVR_PCINT_EXTRA_PINS \
	, GPIO_MAKE_PIN(PJ, 0), \
	GPIO_MAKE_PIN(PJ, 1), \
	GPIO_MAKE_PIN(PJ, 2), \
	GPIO_MAKE_PIN(PJ, 3), \
	GPIO_MAKE_PIN(PJ, 4), \
	GPIO_MAKE_PIN(PJ, 5), \
	GPIO_MAKE_PIN(PJ, 6), \
	GPIO_MAKE_PIN(PK, 0), \
	GPIO_MAKE_PIN(PK, 1), \
	GPIO_MAKE_PIN(PK, 2), \
	GPIO_MAKE_PIN(PK, 3), \
	GPIO_MAKE_PIN(PK, 4), \
	GPIO_MAKE_PIN(PK, 5), \
	GPIO_MAKE_PIN(PK, 6), \
	GPIO_MAKE_PIN(PK, 7)
#else
#define _AVR_NUM_PCINT_PINS 9
#define _AVR_PCINT_EXTRA_PINS
#endif
#define _AVR_PCINT_IDS { \
	GPIO_MAKE_PIN(PB, 0), \
	GPIO_MAKE_PIN(PB, 1), \
	GPIO_MAKE_PIN(PB, 2), \
	GPIO_MAKE_PIN(PB, 3), \
	GPIO_MAKE_PIN(PB, 4), \
	GPIO_MAKE_PIN(PB, 5), \
	GPIO_MAKE_PIN(PB, 6), \
	GPIO_MAKE_PIN(PB, 7), \
	GPIO_MAKE_PIN(PE, 0) \
	_AVR_PCINT_EXTRA_PINS \
}
// }}}

// Int {{{
#define PIN_INT0 GPIO_MAKE_PIN(PD, 0)
#define PIN_INT1 GPIO_MAKE_PIN(PD, 1)
#define PIN_INT2 GPIO_MAKE_PIN(PD, 2)
#define PIN_INT3 GPIO_MAKE_PIN(PD, 3)
#define PIN_INT4 GPIO_MAKE_PIN(PE, 4)
#define PIN_INT5 GPIO_MAKE_PIN(PE, 5)
#define PIN_INT6 GPIO_MAKE_PIN(PE, 6)
#define PIN_INT7 GPIO_MAKE_PIN(PE, 7)
// }}}

// Jtag {{{
#define PIN_TCK GPIO_MAKE_PIN(PF, 4)
#define PIN_TMS GPIO_MAKE_PIN(PF, 5)
#define PIN_TDO GPIO_MAKE_PIN(PF, 6)
#define PIN_TDI GPIO_MAKE_PIN(PF, 7)
// }}}

// Other pins {{{
#define PIN_RESET GPIO_MAKE_PIN(12, 0)
#define PIN_XTAL1 GPIO_MAKE_PIN(12, 1)
#define PIN_XTAL2 GPIO_MAKE_PIN(12, 2)
#define PIN_CLKO GPIO_MAKE_PIN(PE, 7)
// }}}

#include "../parts/common.hh"
#include "../parts/adc.hh"
#include "../parts/comparator.hh"
#include "../parts/counter1.hh"
#include "../parts/counter2.hh"
#include "../parts/twi.hh"
#include "../parts/usart.hh"
#include "../parts/spi.hh"

#endif

// vim: set foldmethod=marker :
