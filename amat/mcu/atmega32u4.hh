#ifndef _AVR_ATMEGA328P
#define _AVR_ATMEGA328P

// Int {{{
#define PIN_INT0 GPIO_MAKE_PIN(PD, 0)
#define PIN_INT1 GPIO_MAKE_PIN(PD, 1)
#define PIN_INT2 GPIO_MAKE_PIN(PD, 2)
#define PIN_INT3 GPIO_MAKE_PIN(PD, 3)
#define PIN_INT6 GPIO_MAKE_PIN(PE, 6)
// }}}

// Pcint {{{
#define _AVR_NUM_PCINT_PINS 8
#define _AVR_PCINT_IDS { \
	GPIO_MAKE_PIN(PB, 0), \
	GPIO_MAKE_PIN(PB, 1), \
	GPIO_MAKE_PIN(PB, 2), \
	GPIO_MAKE_PIN(PB, 3), \
	GPIO_MAKE_PIN(PB, 4), \
	GPIO_MAKE_PIN(PB, 5), \
	GPIO_MAKE_PIN(PB, 6), \
	GPIO_MAKE_PIN(PB, 7) \
}
// }}}

// Adc {{{
#define _AVR_NUM_ADC_PINS 14
// Use ids that are not digital pins for adc6 and adc7, because they are not shared with digital pins.
#define PIN_ADC0 GPIO_MAKE_PIN(PF, 0)
#define PIN_ADC1 GPIO_MAKE_PIN(PF, 1)
#define PIN_ADC4 GPIO_MAKE_PIN(PF, 4)
#define PIN_ADC5 GPIO_MAKE_PIN(PF, 5)
#define PIN_ADC6 GPIO_MAKE_PIN(PF, 6)
#define PIN_ADC7 GPIO_MAKE_PIN(PF, 7)
#define PIN_ADC8 GPIO_MAKE_PIN(PD, 4)
#define PIN_ADC9 GPIO_MAKE_PIN(PD, 6)
#define PIN_ADC10 GPIO_MAKE_PIN(PD, 7)
#define PIN_ADC11 GPIO_MAKE_PIN(PB, 4)
#define PIN_ADC12 GPIO_MAKE_PIN(PB, 5)
#define PIN_ADC13 GPIO_MAKE_PIN(PB, 6)
namespace Adc {
	enum Source {
		SRC_A0,
		SRC_A1,
		SRC_A4 = 0x04,
		SRC_A5,
		SRC_A6,
		SRC_A7,
		//
		SRC_A1_A0_10x = 0x09,
		SRC_A1_A0_200x = 0x0b,
		//
		SRC_A0_A1 = 0x10,
		SRC_A4_A1 = 0x14,
		SRC_A5_A1,
		SRC_A6_A1,
		SRC_A7_A1,
		//
		SRC_1V1 = 0x1e,
		SRC_GND = 0x1f,
		//
		SRC_A8,
		SRC_A9,
		SRC_A10,
		SRC_A11,
		SRC_A12,
		SRC_A13,
		SRC_A0_A1_40x,
		SRC_TEMP,
		//
		SRC_A4_A0_10x,
		SRC_A5_A0_10x,
		SRC_A6_A0_10x,
		SRC_A7_A0_10x,
		SRC_A4_A1_10x,
		SRC_A5_A1_10x,
		SRC_A6_A1_10x,
		SRC_A7_A1_10x,
		//
		SRC_A4_A0_40x,
		SRC_A5_A0_40x,
		SRC_A6_A0_40x,
		SRC_A7_A0_40x,
		SRC_A4_A1_40x,
		SRC_A5_A1_40x,
		SRC_A6_A1_40x,
		SRC_A7_A1_40x,
		//
		SRC_A4_A0_200x,
		SRC_A5_A0_200x,
		SRC_A6_A0_200x,
		SRC_A7_A0_200x,
		SRC_A4_A1_200x,
		SRC_A5_A1_200x,
		SRC_A6_A1_200x,
		SRC_A7_A1_200x
	};
}
#define _AVR_ADC_MAP { \
	{0, 0xff, 1}, \
	{1, 0xff, 1}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{4, 0xff, 1}, \
	{5, 0xff, 1}, \
	{6, 0xff, 1}, \
	{7, 0xff, 1}, \
	\
	{0xff, 0xff, 0}, \
	{1, 0, 10}, \
	{0xff, 0xff, 0}, \
	{1, 0, 200}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	\
	{0, 1, 1}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{4, 1, 1}, \
	{5, 1, 1}, \
	{6, 1, 1}, \
	{7, 1, 1}, \
	\
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{ADC_SRC_1V1, 0xff, 1}, \
	{ADC_SRC_GND, 0xff, 1}, \
	\
	{8, 0xff, 1}, \
	{9, 0xff, 1}, \
	{10, 0xff, 1}, \
	{11, 0xff, 1}, \
	{12, 0xff, 1}, \
	{13, 0xff, 1}, \
	{1, 0, 40}, \
	{ADC_SRC_TEMP, 0xff, 1}, \
	\
	{4, 0, 10}, \
	{5, 0, 10}, \
	{6, 0, 10}, \
	{7, 0, 10}, \
	{4, 1, 10}, \
	{5, 1, 10}, \
	{6, 1, 10}, \
	{7, 1, 10}, \
	\
	{4, 0, 40}, \
	{5, 0, 40}, \
	{6, 0, 40}, \
	{7, 0, 40}, \
	{4, 1, 40}, \
	{5, 1, 40}, \
	{6, 1, 40}, \
	{7, 1, 40}, \
	\
	{4, 0, 200}, \
	{5, 0, 200}, \
	{6, 0, 200}, \
	{7, 0, 200}, \
	{4, 1, 200}, \
	{5, 1, 200}, \
	{6, 1, 200}, \
	{7, 1, 200}, \
}
#define _AVR_ADC_MAP_SIZE 0x40
// }}}

// Comparator {{{
#define PIN_AIN0 GPIO_MAKE_PIN(PE, 6)
// }}}

// Timers {{{
#define PIN_OC0A GPIO_MAKE_PIN(PB, 7)
#define PIN_OC0B GPIO_MAKE_PIN(PD, 0)
#define PIN_T0 GPIO_MAKE_PIN(PD, 7)

#define PIN_OC1A GPIO_MAKE_PIN(PB, 5)
#define PIN_OC1B GPIO_MAKE_PIN(PB, 6)
#define PIN_OC1C GPIO_MAKE_PIN(PB, 7)
#define PIN_T1 GPIO_MAKE_PIN(PD, 6)
#define PIN_ICP1 GPIO_MAKE_PIN(PD, 4)

#define PIN_OC3A GPIO_MAKE_PIN(PC, 6)
#define PIN_ICP3 GPIO_MAKE_PIN(PC, 7)
// }}}

// High speed counter {{{
#define PIN_OC4A GPIO_MAKE_PIN(PC, 7)
#define NOT_PIN_OC4A GPIO_MAKE_PIN(PC, 6)

#define PIN_OC4B GPIO_MAKE_PIN(PB, 6)
#define NOT_PIN_OC4B GPIO_MAKE_PIN(PB, 5)

#define PIN_OC4D GPIO_MAKE_PIN(PD, 7)
#define NOT_PIN_OC4D GPIO_MAKE_PIN(PD, 6)
// }}}

// Twi {{{
#define PIN_SCL GPIO_MAKE_PIN(PD, 0)
#define PIN_SDA GPIO_MAKE_PIN(PD, 1)
// }}}

// Spi {{{
#define PIN_SS GPIO_MAKE_PIN(PB, 0)
#define PIN_MOSI GPIO_MAKE_PIN(PB, 2)
#define PIN_MISO GPIO_MAKE_PIN(PB, 3)
#define PIN_SCK GPIO_MAKE_PIN(PB, 1)
// }}}

// Usart {{{
#define PIN_RXD1 GPIO_MAKE_PIN(PD, 2)
#define PIN_TXD1 GPIO_MAKE_PIN(PD, 3)
#define PIN_XCK1 GPIO_MAKE_PIN(PD, 5)
// }}}

// Other pins {{{
#define PIN_RESET GPIO_MAKE_PIN(0, 0)
#define PIN_XTAL1 GPIO_MAKE_PIN(0, 1)
#define PIN_XTAL2 GPIO_MAKE_PIN(0, 2)
#define PIN_CLKO GPIO_MAKE_PIN(PC, 7)
// }}}

#include "../parts/common.hh"
#include "../parts/adc.hh"
#include "../parts/comparator.hh"
#include "../parts/counter1.hh"
#include "../parts/twi.hh"
#include "../parts/usart.hh"
#include "../parts/spi.hh"
#include "../parts/usb.hh"
#include "../parts/hscounter.hh"

#endif

// vim: set foldmethod=marker :
