#ifndef AMAT_HH
#define AMAT_HH

// Set up test machinery before including anything. {{{
#ifdef AVR_TEST_ALL
#define AVR_TEST_ADC
#define AVR_TEST_CLOCK
#define AVR_TEST_COMPARATOR
#define AVR_TEST_COUNTER0
#define AVR_TEST_COUNTER1
#define AVR_TEST_COUNTER2
#define AVR_TEST_EEPROM
#define AVR_TEST_GPIO
#define AVR_TEST_HSCOUNTER
#define AVR_TEST_INFO
#define AVR_TEST_INT
#define AVR_TEST_PCINT
#define AVR_TEST_SLEEP
#define AVR_TEST_SPI
#define AVR_TEST_TWI
#define AVR_TEST_USART
#define AVR_TEST_USB
#define AVR_TEST_USI
#define AVR_TEST_WDT
#endif

#if defined(AVR_TEST_ADC) \
	|| defined(AVR_TEST_CLOCK) \
	|| defined(AVR_TEST_COMPARATOR) \
	|| defined(AVR_TEST_COUNTER0) \
	|| defined(AVR_TEST_COUNTER1) \
	|| defined(AVR_TEST_COUNTER2) \
	|| defined(AVR_TEST_EEPROM) \
	|| defined(AVR_TEST_GPIO) \
	|| defined(AVR_TEST_HSCOUNTER) \
	|| defined(AVR_TEST_INFO) \
	|| defined(AVR_TEST_INT) \
	|| defined(AVR_TEST_PCINT) \
	|| defined(AVR_TEST_SLEEP) \
	|| defined(AVR_TEST_SPI) \
	|| defined(AVR_TEST_TWI) \
	|| defined(AVR_TEST_USART) \
	|| defined(AVR_TEST_USB) \
	|| defined(AVR_TEST_USI) \
	|| defined(AVR_TEST_WDT)
#define AVR_TEST
#endif
// }}}

// Define some constants that the mcu files need.
#define ADC_SRC_1V1 0xfc
#define ADC_SRC_GND 0xfd
#define ADC_SRC_TEMP 0xfe

// Almost everything is defined in the mcu header.
#include AVR_MCU_HEADER

// The clock must be defined after timer1.
#include "parts/clock.hh"
// Every mcu has info support. It must be included last (but before the second test.hh), so do it here.
#include "parts/info.hh"
// At the end, include test.hh a second time; it defines some variables then.
#include "parts/test.hh"

// Finally, define the main function if it is not disabled.
#include "parts/main.hh"

#endif

// vim: set foldmethod=marker :
