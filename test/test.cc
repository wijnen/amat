// Allow testing one one (or a few) components by passing -DAVR_TEST_SINGLE -DAVR_TEST_<part>
#ifndef AVR_TEST_SINGLE
#define AVR_TEST_ALL
#endif

//#define TX1_DEBUG	// Used on when mcu supports usb.
//#define SERIAL_ECHO
#include <amat.hh>

// Nothing else needed; main function is defined in parts/test.hh.
