#ifndef _AVR_ATMEGA328P
#define _AVR_ATMEGA328P

// Int {{{
#define INT0_PIN GPIO_MAKE_PIN(PD, 0)
#define INT1_PIN GPIO_MAKE_PIN(PD, 1)
#define INT2_PIN GPIO_MAKE_PIN(PD, 2)
#define INT3_PIN GPIO_MAKE_PIN(PD, 3)
#define INT6_PIN GPIO_MAKE_PIN(PE, 6)
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
#define ADC0_PIN GPIO_MAKE_PIN(PF, 0)
#define ADC1_PIN GPIO_MAKE_PIN(PF, 1)
#define ADC4_PIN GPIO_MAKE_PIN(PF, 4)
#define ADC5_PIN GPIO_MAKE_PIN(PF, 5)
#define ADC6_PIN GPIO_MAKE_PIN(PF, 6)
#define ADC7_PIN GPIO_MAKE_PIN(PF, 7)
#define ADC8_PIN GPIO_MAKE_PIN(PD, 4)
#define ADC9_PIN GPIO_MAKE_PIN(PD, 6)
#define ADC10_PIN GPIO_MAKE_PIN(PD, 7)
#define ADC11_PIN GPIO_MAKE_PIN(PB, 4)
#define ADC12_PIN GPIO_MAKE_PIN(PB, 5)
#define ADC13_PIN GPIO_MAKE_PIN(PB, 6)
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
#define AIN0_PIN GPIO_MAKE_PIN(PE, 6)
// }}}

// Timers {{{
#define OC0A_PIN GPIO_MAKE_PIN(PB, 7)
#define OC0B_PIN GPIO_MAKE_PIN(PD, 0)
#define T0_PIN GPIO_MAKE_PIN(PD, 7)

#define OC1A_PIN GPIO_MAKE_PIN(PB, 5)
#define OC1B_PIN GPIO_MAKE_PIN(PB, 6)
#define OC1C_PIN GPIO_MAKE_PIN(PB, 7)
#define T1_PIN GPIO_MAKE_PIN(PD, 6)
#define ICP1_PIN GPIO_MAKE_PIN(PD, 4)

#define OC3A_PIN GPIO_MAKE_PIN(PC, 6)
#define ICP3_PIN GPIO_MAKE_PIN(PC, 7)
// }}}

// High speed counter {{{
#define OC4A_PIN GPIO_MAKE_PIN(PC, 7)
#define NOT_OC4A_PIN GPIO_MAKE_PIN(PC, 6)

#define OC4B_PIN GPIO_MAKE_PIN(PB, 6)
#define NOT_OC4B_PIN GPIO_MAKE_PIN(PB, 5)

#define OC4D_PIN GPIO_MAKE_PIN(PD, 7)
#define NOT_OC4D_PIN GPIO_MAKE_PIN(PD, 6)
// }}}

// Twi {{{
#define SCL_PIN GPIO_MAKE_PIN(PD, 0)
#define SDA_PIN GPIO_MAKE_PIN(PD, 1)
// }}}

// Spi {{{
#define SS_PIN GPIO_MAKE_PIN(PB, 0)
#define MOSI_PIN GPIO_MAKE_PIN(PB, 2)
#define MISO_PIN GPIO_MAKE_PIN(PB, 3)
#define SCK_PIN GPIO_MAKE_PIN(PB, 1)
// }}}

// Usart {{{
#define RXD1_PIN GPIO_MAKE_PIN(PD, 2)
#define TXD1_PIN GPIO_MAKE_PIN(PD, 3)
#define XCK1_PIN GPIO_MAKE_PIN(PD, 5)
// }}}

// Other pins {{{
#define RESET_PIN GPIO_MAKE_PIN(0, 0)
#define XTAL1_PIN GPIO_MAKE_PIN(0, 1)
#define XTAL2_PIN GPIO_MAKE_PIN(0, 2)
#define CLKO_PIN GPIO_MAKE_PIN(PC, 7)
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
