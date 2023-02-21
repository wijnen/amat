#ifndef _AVR_ATTINY84A
#define _AVR_ATTINY84A

// Timers {{{
#define PIN_OC0A GPIO_MAKE_PIN(PB, 2)
#define PIN_OC0B GPIO_MAKE_PIN(PA, 7)
#define PIN_T0 GPIO_MAKE_PIN(PA, 3)

#define PIN_OC1A GPIO_MAKE_PIN(PA, 6)
#define PIN_OC1B GPIO_MAKE_PIN(PA, 5)
#define PIN_T1 GPIO_MAKE_PIN(PA, 4)
#define PIN_ICP1 GPIO_MAKE_PIN(PA, 7)
// }}}

// Comparator {{{
#define PIN_AIN0 GPIO_MAKE_PIN(PA, 1)
#define PIN_AIN1 GPIO_MAKE_PIN(PA, 2)
// }}}

// Adc {{{
#define _AVR_NUM_ADC_PINS 8
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

		SRC_A0_A1,
		SRC_A0_A1_20x,
		SRC_A0_A3,
		SRC_A0_A3_20x,
		SRC_A1_A2,
		SRC_A1_A2_20x,
		SRC_A1_A3,
		SRC_A1_A3_20x,

		SRC_A2_A3,
		SRC_A2_A3_20x,
		SRC_A3_A4,
		SRC_A3_A4_20x,
		SRC_A3_A5,
		SRC_A3_A5_20x,
		SRC_A3_A6,
		SRC_A3_A6_20x,

		SRC_A3_A7,
		SRC_A3_A7_20x,
		SRC_A4_A5,
		SRC_A4_A5_20x,
		SRC_A5_A6,
		SRC_A5_A6_20x,
		SRC_A6_A7,
		SRC_A6_A7_20x,

		SRC_GND,
		SRC_1V1,
		SRC_TEMP,
		SRC_A0_A0_20x,
		SRC_A3_A3,
		SRC_A3_A3_20x,
		SRC_A7_A7,
		SRC_A7_A7_20x,

		SRC_A1_A0,
		SRC_A1_A0_20x,
		SRC_A3_A0,
		SRC_A3_A0_20x,
		SRC_A2_A1,
		SRC_A2_A1_20x,
		SRC_A3_A1,
		SRC_A3_A1_20x,

		SRC_A3_A2,
		SRC_A3_A2_20x,
		SRC_A4_A3,
		SRC_A4_A3_20x,
		SRC_A5_A3,
		SRC_A5_A3_20x,
		SRC_A6_A3,
		SRC_A6_A3_20x,

		SRC_A7_A3,
		SRC_A7_A3_20x,
		SRC_A5_A4,
		SRC_A5_A4_20x,
		SRC_A6_A5,
		SRC_A6_A5_20x,
		SRC_A7_A6,
		SRC_A7_A6_20x
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
\
	{0, 1, 1}, \
	{0, 1, 20}, \
	{0, 3, 1}, \
	{0, 3, 20}, \
	{1, 2, 1}, \
	{1, 2, 20}, \
	{1, 3, 1}, \
	{1, 3, 20}, \
\
	{2, 3, 1}, \
	{2, 3, 20}, \
	{3, 4, 1}, \
	{3, 4, 20}, \
	{3, 5, 1}, \
	{3, 5, 20}, \
	{3, 6, 1}, \
	{3, 6, 20}, \
\
	{3, 7, 1}, \
	{3, 7, 20}, \
	{4, 5, 1}, \
	{4, 5, 20}, \
	{5, 6, 1}, \
	{5, 6, 20}, \
	{6, 7, 1}, \
	{6, 7, 20}, \
\
	{ADC_SRC_GND, 0xff, 1} \
	{ADC_SRC_1V1, 0xff, 1}, \
	{ADC_SRC_TEMP, 0xff, 1}, \
	{0, 0, 20}, \
	{3, 3, 1}, \
	{3, 3, 20}, \
	{7, 7, 1}, \
	{7, 7, 20}, \
\
	{1, 0, 1}, \
	{1, 0, 20}, \
	{3, 0, 1}, \
	{3, 0, 20}, \
	{2, 1, 1}, \
	{2, 1, 20}, \
	{3, 1, 1}, \
	{3, 1, 20}, \
\
	{3, 2, 1}, \
	{3, 2, 20}, \
	{4, 3, 1}, \
	{4, 3, 20}, \
	{5, 3, 1}, \
	{5, 3, 20}, \
	{6, 3, 1}, \
	{6, 3, 20}, \
\
	{7, 3, 1}, \
	{7, 3, 20}, \
	{5, 4, 1}, \
	{5, 4, 20}, \
	{6, 5, 1}, \
	{6, 5, 20}, \
	{7, 6, 1}, \
	{7, 6, 20}, \
}
#define _AVR_ADC_MAP_SIZE 0x40
// }}}


// Usi {{{
#define PIN_DI GPIO_MAKE_PIN(PA, 6)
#define PIN_DO GPIO_MAKE_PIN(PA, 5)
#define PIN_USCK GPIO_MAKE_PIN(PA, 4)
// }}}

// Pcint {{{
#define _AVR_NUM_PCINT_PINS 8
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
	GPIO_MAKE_PIN(PB, 3) \
}
// }}}

// Int {{{
#define PIN_INT0 GPIO_MAKE_PIN(PB, 2)
// }}}

// Other pins {{{
#define PIN_RESET GPIO_MAKE_PIN(PB, 3)
#define PIN_DW GPIO_MAKE_PIN(PB, 3)
#define PIN_XTAL1 GPIO_MAKE_PIN(PB, 0)
#define PIN_XTAL2 GPIO_MAKE_PIN(PB, 1)
#define PIN_CLKO GPIO_MAKE_PIN(PB, 2)
// }}}

#include "../parts/common.hh"
#include "../parts/comparator.hh"
#include "../parts/counter1.hh"
#include "../parts/usi.hh"

#endif

// vim: set foldmethod=marker :
