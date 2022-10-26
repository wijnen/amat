#ifndef _AVR_ATTINY2313
#define _AVR_ATTINY2313

// Timers {{{
#define OC0A_PIN GPIO_MAKE_PIN(PB, 2)
#define OC0B_PIN GPIO_MAKE_PIN(PD, 5)
#define T0_PIN GPIO_MAKE_PIN(PD, 4)
#define TIMSK0 TIMSK
#define TIFR0 TIFR

#define TIMSK1 TIMSK
#define TIFR1 TIFR
#define OC1A_PIN GPIO_MAKE_PIN(PB, 3)
#define OC1B_PIN GPIO_MAKE_PIN(PB, 4)
#define T1_PIN GPIO_MAKE_PIN(PD, 5)
#define ICP1_PIN GPIO_MAKE_PIN(PD, 6)
// }}}

// Comparator {{{
#define AIN0_PIN GPIO_MAKE_PIN(PB, 0)
#define AIN1_PIN GPIO_MAKE_PIN(PB, 1)
// }}}

// Usart {{{
#define RXD0_PIN GPIO_MAKE_PIN(PD, 0)
#define TXD0_PIN GPIO_MAKE_PIN(PD, 1)
#define XCK0_PIN GPIO_MAKE_PIN(PD, 2)
// }}}

// Usi {{{
#define DI_PIN GPIO_MAKE_PIN(PB, 5)
#define DO_PIN GPIO_MAKE_PIN(PB, 6)
#define USCK_PIN GPIO_MAKE_PIN(PB, 7)
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
#define INT0_PIN GPIO_MAKE_PIN(PD, 2)
#define INT1_PIN GPIO_MAKE_PIN(PD, 3)
// }}}

// Other pins {{{
#define RESET_PIN GPIO_MAKE_PIN(PA, 2)
#define DW_PIN GPIO_MAKE_PIN(PA, 2)
#define XTAL1_PIN GPIO_MAKE_PIN(PA, 0)
#define XTAL2_PIN GPIO_MAKE_PIN(PA, 1)
#define CLKO_PIN GPIO_MAKE_PIN(PD, 2)
// }}}

#include "../parts/common.hh"
#include "../parts/comparator.hh"
#include "../parts/counter1.hh"
#include "../parts/usart.hh"
#include "../parts/usi.hh"

#endif

// vim: set foldmethod=marker :
