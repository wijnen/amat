#ifndef _AVR_ATMEGA644
#define _AVR_ATMEGA644

// Timers {{{
#define PIN_OC0A GPIO_MAKE_PIN(PB, 3)
#define PIN_OC0B GPIO_MAKE_PIN(PB, 4)
#define PIN_T0 GPIO_MAKE_PIN(PB, 0)

#define PIN_OC1A GPIO_MAKE_PIN(PD, 5)
#define PIN_OC1B GPIO_MAKE_PIN(PD, 4)
#define PIN_T1 GPIO_MAKE_PIN(PB, 1)
#define PIN_ICP1 GPIO_MAKE_PIN(PD, 6)

#define PIN_OC2A GPIO_MAKE_PIN(PD, 7)
#define PIN_OC2B GPIO_MAKE_PIN(PD, 6)
// }}}

// Adc {{{
#define _AVR_NUM_ADC_PINS 8
#define _AVR_HAVE_REF_2_56V
#define PIN_ADC0 GPIO_MAKE_PIN(PA, 0)
#define PIN_ADC1 GPIO_MAKE_PIN(PA, 1)
#define PIN_ADC2 GPIO_MAKE_PIN(PA, 2)
#define PIN_ADC3 GPIO_MAKE_PIN(PA, 3)
#define PIN_ADC4 GPIO_MAKE_PIN(PA, 4)
#define PIN_ADC5 GPIO_MAKE_PIN(PA, 5)
#define PIN_ADC6 GPIO_MAKE_PIN(PA, 6)
#define PIN_ADC7 GPIO_MAKE_PIN(PA, 7)
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
#define PIN_AIN0 GPIO_MAKE_PIN(PB, 2)
#define PIN_AIN1 GPIO_MAKE_PIN(PB, 3)
// }}}

// Twi {{{
#define PIN_SCL GPIO_MAKE_PIN(PC, 0)
#define PIN_SDA GPIO_MAKE_PIN(PC, 1)
// }}}

// Usart {{{
#define PIN_RXD0 GPIO_MAKE_PIN(PD, 0)
#define PIN_TXD0 GPIO_MAKE_PIN(PD, 1)
#define PIN_XCK0 GPIO_MAKE_PIN(PB, 0)
#define PIN_RXD1 GPIO_MAKE_PIN(PD, 2)
#define PIN_TXD1 GPIO_MAKE_PIN(PD, 3)
#define PIN_XCK1 GPIO_MAKE_PIN(PD, 4)
// }}}

// Spi {{{
#define PIN_SS GPIO_MAKE_PIN(PB, 4)
#define PIN_MOSI GPIO_MAKE_PIN(PB, 5)
#define PIN_MISO GPIO_MAKE_PIN(PB, 6)
#define PIN_SCK GPIO_MAKE_PIN(PB, 7)
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
#define PIN_INT0 GPIO_MAKE_PIN(PD, 2)
#define PIN_INT1 GPIO_MAKE_PIN(PD, 3)
#define PIN_INT2 GPIO_MAKE_PIN(PB, 2)
// }}}

// Jtag {{{
#define PIN_TCK GPIO_MAKE_PIN(PC, 2)
#define PIN_TMS GPIO_MAKE_PIN(PC, 3)
#define PIN_TDO GPIO_MAKE_PIN(PC, 4)
#define PIN_TDI GPIO_MAKE_PIN(PC, 5)
// }}}

// Other pins {{{
#define PIN_RESET GPIO_MAKE_PIN(4, 0)
#define PIN_XTAL1 GPIO_MAKE_PIN(4, 1)
#define PIN_XTAL2 GPIO_MAKE_PIN(4, 2)
#define PIN_CLKO GPIO_MAKE_PIN(PB, 1)
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
