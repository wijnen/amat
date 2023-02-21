#ifndef _AVR_ATTINY2313
#define _AVR_ATTINY2313

// Timers {{{
#define PIN_OC0A GPIO_MAKE_PIN(PB, 2)
#define PIN_OC0B GPIO_MAKE_PIN(PD, 5)
#define PIN_T0 GPIO_MAKE_PIN(PD, 4)
#define TIMSK0 TIMSK
#define TIFR0 TIFR

#define TIMSK1 TIMSK
#define TIFR1 TIFR
#define PIN_OC1A GPIO_MAKE_PIN(PB, 3)
#define PIN_OC1B GPIO_MAKE_PIN(PB, 4)
#define PIN_T1 GPIO_MAKE_PIN(PD, 5)
#define PIN_ICP1 GPIO_MAKE_PIN(PD, 6)
// }}}

// Comparator {{{
#define PIN_AIN0 GPIO_MAKE_PIN(PB, 0)
#define PIN_AIN1 GPIO_MAKE_PIN(PB, 1)
// }}}

// Usart {{{
#define PIN_RXD0 GPIO_MAKE_PIN(PD, 0)
#define PIN_TXD0 GPIO_MAKE_PIN(PD, 1)
#define PIN_XCK0 GPIO_MAKE_PIN(PD, 2)
// }}}

// Usi {{{
#define PIN_DI GPIO_MAKE_PIN(PB, 5)
#define PIN_DO GPIO_MAKE_PIN(PB, 6)
#define PIN_USCK GPIO_MAKE_PIN(PB, 7)
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

// Int {{{
#define PIN_INT0 GPIO_MAKE_PIN(PD, 2)
#define PIN_INT1 GPIO_MAKE_PIN(PD, 3)
// }}}

// Other pins {{{
#define PIN_RESET GPIO_MAKE_PIN(PA, 2)
#define PIN_DW GPIO_MAKE_PIN(PA, 2)
#define PIN_XTAL1 GPIO_MAKE_PIN(PA, 0)
#define PIN_XTAL2 GPIO_MAKE_PIN(PA, 1)
#define PIN_CLKO GPIO_MAKE_PIN(PD, 2)
// }}}

#include "../parts/common.hh"
#include "../parts/comparator.hh"
#include "../parts/counter1.hh"
#include "../parts/usart.hh"
#include "../parts/usi.hh"

#endif

// vim: set foldmethod=marker :
