#ifndef _AVR_ATTINY84A
#define _AVR_ATTINY84A

// Timers {{{
#define OC0A_PIN GPIO_MAKE_PIN(PB, 2)
#define OC0B_PIN GPIO_MAKE_PIN(PA, 7)
#define T0_PIN GPIO_MAKE_PIN(PA, 3)

#define OC1A_PIN GPIO_MAKE_PIN(PA, 6)
#define OC1B_PIN GPIO_MAKE_PIN(PA, 5)
#define T1_PIN GPIO_MAKE_PIN(PA, 4)
#define ICP1_PIN GPIO_MAKE_PIN(PA, 7)
// }}}

// Comparator {{{
#define AIN0_PIN GPIO_MAKE_PIN(PA, 1)
#define AIN1_PIN GPIO_MAKE_PIN(PA, 2)
// }}}

// Adc {{{
#define _AVR_NUM_ADC_PINS 8
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
#define DI_PIN GPIO_MAKE_PIN(PA, 6)
#define DO_PIN GPIO_MAKE_PIN(PA, 5)
#define USCK_PIN GPIO_MAKE_PIN(PA, 4)
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
	GPIO_MAKE_PIN(PA, 7) \
	GPIO_MAKE_PIN(PB, 0), \
	GPIO_MAKE_PIN(PB, 1), \
	GPIO_MAKE_PIN(PB, 2), \
	GPIO_MAKE_PIN(PB, 3), \
}
// }}}

// Int {{{
#define INT0_PIN GPIO_MAKE_PIN(PB, 2)
// }}}

// Other pins {{{
#define RESET_PIN GPIO_MAKE_PIN(PB, 3)
#define DW_PIN GPIO_MAKE_PIN(PB, 3)
#define XTAL1_PIN GPIO_MAKE_PIN(PB, 0)
#define XTAL2_PIN GPIO_MAKE_PIN(PB, 1)
#define CLKO_PIN GPIO_MAKE_PIN(PB, 2)
// }}}

#include "../parts/common.hh"
#include "../parts/comparator.hh"
#include "../parts/counter1.hh"
#include "../parts/usi.hh"

#endif

// vim: set foldmethod=marker :
