#ifndef _AVR_ATMEGA644
#define _AVR_ATMEGA644

// Timers {{{
#define OC0A_PIN GPIO_MAKE_PIN(PB, 3)
#define OC0B_PIN GPIO_MAKE_PIN(PB, 4)
#define T0_PIN GPIO_MAKE_PIN(PB, 0)

#define OC1A_PIN GPIO_MAKE_PIN(PD, 5)
#define OC1B_PIN GPIO_MAKE_PIN(PD, 4)
#define T1_PIN GPIO_MAKE_PIN(PB, 1)
#define ICP1_PIN GPIO_MAKE_PIN(PD, 6)

#define OC2A_PIN GPIO_MAKE_PIN(PD, 7)
#define OC2B_PIN GPIO_MAKE_PIN(PD, 6)
// }}}

// Adc {{{
#define _AVR_NUM_ADC_PINS 8
#define _AVR_HAVE_REF_2_56V
#define ADC0_PIN GPIO_MAKE_PIN(PA, 0)
#define ADC1_PIN GPIO_MAKE_PIN(PA, 1)
#define ADC2_PIN GPIO_MAKE_PIN(PA, 2)
#define ADC3_PIN GPIO_MAKE_PIN(PA, 3)
#define ADC4_PIN GPIO_MAKE_PIN(PA, 4)
#define ADC5_PIN GPIO_MAKE_PIN(PA, 5)
#define ADC6_PIN GPIO_MAKE_PIN(PA, 6)
#define ADC7_PIN GPIO_MAKE_PIN(PA, 7)
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
		SRC_A3_A2_10x,
		SRC_A2_A2_10x,
		SRC_A3_A2_200x,
		SRC_A2_A2_200x,
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
		SRC_GND
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
}
#define _AVR_ADC_MAP_SIZE 0x20
// }}}

// Comparator {{{
#define AIN0_PIN GPIO_MAKE_PIN(PB, 2)
#define AIN1_PIN GPIO_MAKE_PIN(PB, 3)
// }}}

// Twi {{{
#define SCL_PIN GPIO_MAKE_PIN(PC, 0)
#define SDA_PIN GPIO_MAKE_PIN(PC, 1)
// }}}

// Usart {{{
#define RXD0_PIN GPIO_MAKE_PIN(PD, 0)
#define TXD0_PIN GPIO_MAKE_PIN(PD, 1)
#define XCK0_PIN GPIO_MAKE_PIN(PB, 0)
#define RXD1_PIN GPIO_MAKE_PIN(PD, 2)
#define TXD1_PIN GPIO_MAKE_PIN(PD, 3)
#define XCK1_PIN GPIO_MAKE_PIN(PD, 4)
// }}}

// Spi {{{
#define SS_PIN GPIO_MAKE_PIN(PB, 4)
#define MOSI_PIN GPIO_MAKE_PIN(PB, 5)
#define MISO_PIN GPIO_MAKE_PIN(PB, 6)
#define SCK_PIN GPIO_MAKE_PIN(PB, 7)
// }}}

// Pcint {{{
#define _AVR_NUM_PCINT_PINS 32
#define _AVR_PCINT_IDS { \
	GPIO_MAKE_PIN(PA, 0), \
	GPIO_MAKE_PIN(PA, 1), \
	GPIO_MAKE_PIN(PA, 2), \
	GPIO_MAKE_PIN(PA, 3), \
	GPIO_MAKE_PIN(PA, 4), \
	GPIO_MAKE_PIN(PA, 5), \
	GPIO_MAKE_PIN(PA, 6), \
	GPIO_MAKE_PIN(PA, 7), \
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
	GPIO_MAKE_PIN(PC, 7), \
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

// Int {{{
#define INT0_PIN GPIO_MAKE_PIN(PD, 2)
#define INT1_PIN GPIO_MAKE_PIN(PD, 3)
#define INT2_PIN GPIO_MAKE_PIN(PB, 2)
// }}}

// Jtag {{{
#define TCK_PIN GPIO_MAKE_PIN(PC, 2)
#define TMS_PIN GPIO_MAKE_PIN(PC, 3)
#define TDO_PIN GPIO_MAKE_PIN(PC, 4)
#define TDI_PIN GPIO_MAKE_PIN(PC, 5)
// }}}

// Other pins {{{
#define RESET_PIN GPIO_MAKE_PIN(4, 0)
#define XTAL1_PIN GPIO_MAKE_PIN(4, 1)
#define XTAL2_PIN GPIO_MAKE_PIN(4, 2)
#define CLKO_PIN GPIO_MAKE_PIN(PB, 1)
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
