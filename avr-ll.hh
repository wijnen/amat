#ifndef AVR_H
#define AVR_H

// Define some constants that the mcu files need.
#define ADC_SRC_1V1 0xfc
#define ADC_SRC_GND 0xfd
#define ADC_SRC_TEMP 0xfe

// Almost everything is defined in the mcu header.
#include AVR_MCU_HEADER

// Every mcu has info support. It must be included last (but before the second test.hh), so do it here.
#include "parts/info.hh"
// At the end, include test.hh a second time; it defines some variables then.
#include "parts/test.hh"

// Finally, define the main function if it is not disabled.
#include "parts/main.hh"

#endif
