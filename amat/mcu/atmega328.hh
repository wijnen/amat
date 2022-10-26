#ifndef _AVR_ATMEGA328
#define _AVR_ATMEGA328

// Int {{{
#define INT0_PIN GPIO_MAKE_PIN(PD, 2)
#define INT1_PIN GPIO_MAKE_PIN(PD, 3)
// }}}

// Pcint {{{
#define _AVR_NUM_PCINT_PINS 24
#define _AVR_PCINT_IDS { \
	GPIO_MAKE_PIN(PB, 0), \
	GPIO_MAKE_PIN(PB, 1), \
	GPIO_MAKE_PIN(PB, 2), \
	GPIO_MAKE_PIN(PB, 3), \
	GPIO_MAKE_PIN(PB, 4), \
	GPIO_MAKE_PIN(PB, 5), \
	GPIO_MAKE_PIN(PB, 6), \
	GPIO_MAKE_PIN(PB, 7), \
	GPIO_MAKE_PIN(PC, 0), \
	GPIO_MAKE_PIN(PC, 1), \
	GPIO_MAKE_PIN(PC, 2), \
	GPIO_MAKE_PIN(PC, 3), \
	GPIO_MAKE_PIN(PC, 4), \
	GPIO_MAKE_PIN(PC, 5), \
	GPIO_MAKE_PIN(PC, 6), \
	0xff, \
	GPIO_MAKE_PIN(PD, 0), \
	GPIO_MAKE_PIN(PD, 1), \
	GPIO_MAKE_PIN(PD, 2), \
	GPIO_MAKE_PIN(PD, 3), \
	GPIO_MAKE_PIN(PD, 4), \
	GPIO_MAKE_PIN(PD, 5), \
	GPIO_MAKE_PIN(PD, 6), \
	GPIO_MAKE_PIN(PD, 7) \
}
// }}}

// Adc {{{
#define _AVR_NUM_ADC_PINS 8
// Use ids that are not digital pins for adc6 and adc7, because they are not shared with digital pins.
#define ADC0_PIN GPIO_MAKE_PIN(PC, 0)
#define ADC1_PIN GPIO_MAKE_PIN(PC, 1)
#define ADC2_PIN GPIO_MAKE_PIN(PC, 2)
#define ADC3_PIN GPIO_MAKE_PIN(PC, 3)
#define ADC4_PIN GPIO_MAKE_PIN(PC, 4)
#define ADC5_PIN GPIO_MAKE_PIN(PC, 5)
#define ADC6_PIN GPIO_MAKE_PIN(0, 6)
#define ADC7_PIN GPIO_MAKE_PIN(0, 7)
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
		SRC_TEMP,
		SRC_1V1 = 0xe,
		SRC_GND = 0xf
	};
}
#define _AVR_ADC_MAP { \
	{0, 0xff, 1}, \
	{1, 0xff, 1}, \
	{2, 0xff, 1}, \
	{3, 0xff, 1}, \
	{4, 0xff, 1}, \
	{5, 0xff, 1}, \
	{6, 0xff, 1}, \
	{7, 0xff, 1}, \
	{ADC_SRC_TEMP, 0xff, 1}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{0xff, 0xff, 0}, \
	{ADC_SRC_1V1, 0xff, 1}, \
	{ADC_SRC_GND, 0xff, 1} \
}
#define _AVR_ADC_MAP_SIZE 0x10
// }}}

// Comparator {{{
#define AIN0_PIN GPIO_MAKE_PIN(PD, 6)
#define AIN1_PIN GPIO_MAKE_PIN(PD, 7)
// }}}

// Timers {{{
#define OC0A_PIN GPIO_MAKE_PIN(PD, 6)
#define OC0B_PIN GPIO_MAKE_PIN(PD, 5)
#define T0_PIN GPIO_MAKE_PIN(PD, 4)

#define OC1A_PIN GPIO_MAKE_PIN(PB, 1)
#define OC1B_PIN GPIO_MAKE_PIN(PB, 2)
#define T1_PIN GPIO_MAKE_PIN(PD, 5)
#define ICP1_PIN GPIO_MAKE_PIN(PB, 0)

#define OC2A_PIN GPIO_MAKE_PIN(PB, 3)
#define OC2B_PIN GPIO_MAKE_PIN(PD, 3)
// }}}

// Twi {{{
#define SCL_PIN GPIO_MAKE_PIN(PC, 5)
#define SDA_PIN GPIO_MAKE_PIN(PC, 4)
// }}}

// Spi {{{
#define SS_PIN GPIO_MAKE_PIN(PB, 2)
#define MOSI_PIN GPIO_MAKE_PIN(PB, 3)
#define MISO_PIN GPIO_MAKE_PIN(PB, 4)
#define SCK_PIN GPIO_MAKE_PIN(PB, 5)
// }}}

// Usart {{{
#define RXD0_PIN GPIO_MAKE_PIN(PD, 0)
#define TXD0_PIN GPIO_MAKE_PIN(PD, 1)
#define XCK0_PIN GPIO_MAKE_PIN(PD, 4)
// }}}

// Other pins {{{
#define RESET_PIN GPIO_MAKE_PIN(PC, 6)
#define XTAL1_PIN GPIO_MAKE_PIN(PB, 6)
#define XTAL2_PIN GPIO_MAKE_PIN(PB, 7)
#define CLKO_PIN GPIO_MAKE_PIN(PB, 0)
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
