// Define pin names.

// Options: TODO: handle test option settings.
// INFO_ENABLE
// INFO_ENABLE_NAMES
// INFO_ENABLE_GPIO
// INFO_ENABLE_INT
// INFO_ENABLE_PCINT
// INFO_ENABLE_COUNTER_OC
// INFO_ENABLE_COUNTER_T
// INFO_ENABLE_COUNTER_ICP
// INFO_ENABLE_COMPARATOR
// INFO_ENABLE_ADC
// INFO_ENABLE_USI
// INFO_ENABLE_TWI
// INFO_ENABLE_CLKO
// INFO_ENABLE_DW
// INFO_ENABLE_JTAG
// INFO_ENABLE_RESET

/** @file
# Pin information
This file contains the pin information system.
It needs macros to be defined to do anything.

The goal of this file is to allow code to be written for any device, which can
then query the hardware it is running on. This can be useful when the user
should be able to configure which pins are to be used for certain features.

Example: send all PWM pin names to host.
```
#define DBG_ENABLE
#define INFO_ENABLE
#define INFO_ENABLE_NAMES
#define INFO_ENABLE_COUNTER_OC

#include <amat.hh>

void setup() {
	for (uint8_t pin = Info::first_pin(); pin != 0xff; pin = Info::next_pin(pin))
		dbg(Info::get_name(pin));
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

#ifndef _AVR_INFO_HH
#define _AVR_INFO_HH

#ifdef DOXYGEN
/// @defgroup usemacros Macros to enable features
/**
 * Features that need variables in memory (either SRAM or flash) are disabled
 * by default. They can be enabled using these macros.
 *
 * If a program uses multiple source files, every macro must not be defined in
 * more than one.
 */

// Separator to avoid using the above description for the following define.

/// All the Info functions are disabled unless this macro is defined. @ingroup usemacros
#define INFO_ENABLE

/// All the pin name functions are disabled unless this macro is defined. @ingroup usemacros
#define INFO_ENABLE_NAMES

/// Convenience macro that can be defined to enable all supported information.
/**
 * This includes the effects of INFO_ENABLE and INFO_ENABLE_NAMES
 */
#define INFO_ENABLE_ALL

/// @defgroup infomacros Macros to control which information should be enabled
/// @{

/// Define this macro to include information about Gpio pins.
#define INFO_ENABLE_GPIO

/// Define this macro to include information about external interrupt (Int) pins.
#define INFO_ENABLE_INT

/// Define this macro to include information about pin change interrupt (Pcint) pins.
#define INFO_ENABLE_PCINT

/// Define this macro to include information about Usart pins.
#define INFO_ENABLE_USART

/// Define this macro to include information about Spi pins.
#define INFO_ENABLE_SPI

/// Define this macro to include information about Counter pins.
/**
 * This is syntactic sugar for defining all of INFO_ENABLE_COUNTER_OC,
 * INFO_ENABLE_COUNTER_T and INFO_ENABLE_COUNTER_ICP.
 */
#define INFO_ENABLE_COUNTER

/// Define this macro to include information about Counter output compare pins.
/**
 * This macro must not be defined if INFO_ENABLE_COUNTER is defined.
 */
#define INFO_ENABLE_COUNTER_OC

/// Define this macro to include information about Counter timer input pins.
/**
 * This macro must not be defined if INFO_ENABLE_COUNTER is defined.
 */
#define INFO_ENABLE_COUNTER_T

/// Define this macro to include information about Counter input capture pins.
/**
 * This macro must not be defined if INFO_ENABLE_COUNTER is defined.
 */
#define INFO_ENABLE_COUNTER_ICP

/// Define this macro to include information about Comparator pins.
#define INFO_ENABLE_COMPARATOR

/// Define this macro to include information about Adc pins.
#define INFO_ENABLE_ADC

/// Define this macro to include information about Usi pins.
#define INFO_ENABLE_USI

/// Define this macro to include information about Twi pins.
#define INFO_ENABLE_TWI

/// Define this macro to include information about the Clock Out pin.
#define INFO_ENABLE_CLKO

/// Define this macro to include information about the dW pin.
#define INFO_ENABLE_DW

/// Define this macro to include information about JTAG pins.
#define INFO_ENABLE_JTAG

/// Define this macro to include information about Crystal pins.
#define INFO_ENABLE_XTAL

/// Define this macro to include information about the RESET pin.
#define INFO_ENABLE_RESET

/// @}

#endif

#ifdef AVR_TEST_INFO
#define INFO_ENABLE_ALL
#endif

// Functions: {{{
// Digital pins			N max 96		0ppppbbb	// *011xxxxx unused
// External interrupt		N max 8			01111xxx	// *0110xxxx unused, *01110xxx unused
// Pin change interrupt		N max 32		100xxxxx

// USART RX			N max 4			1010xx00
// USART TX			N max 4			1010xx01
// USART XCK			N max 4			1010xx10
									// 1010xx11 unused
// SPI SS			1			10110000
// SPI MOSI			1			10110001
// SPI MISO			1			10110010
// SPI SCK			1			10110011

// Counter0 output compare	2			1011010x
// Counter0 input		1			10110110
									// 10110111 unused
// Counter1 output compare	N max 3			101110xx	// *10111011 unused
// Counter1 input		1			10111100
// Counter1 capture		1			10111101

// Counter2 output compare	2			1011111x

// Counter* output compare	N max 3*3 = 12		110cc0xx	// *110110xx unused
									// *110xx011 unused
// Counter* input		N max 3			110cc100	// *11011100 unused
// Counter* capture		N max 3			110cc101	// *11011101 unused
									// *110xx101 unused
									// 110xx11x unused
// Comparator AIN0		1			11011110
// Comparator AIN1		1			11011111

// ADC input			N max 16		1110xxxx

// USI				3			111100xx	// *11110011 unused

// TWI SCL			1			11110100
// TWI SDA			1			11110101

// CLKO				1			11110110

// dW				1			11110111

// JTAG				4			111110xx

// XTAL				2			1111110x

// RESET			1			11111110
									// 11111111 used as invalid flag
// }}}

#ifdef INFO_ENABLE_ALL
#define INFO_ENABLE
#define INFO_ENABLE_NAMES
#define INFO_ENABLE_GPIO
#define INFO_ENABLE_INT
#define INFO_ENABLE_PCINT
#define INFO_ENABLE_USART
#define INFO_ENABLE_SPI
#define INFO_ENABLE_COUNTER
#define INFO_ENABLE_COMPARATOR
#define INFO_ENABLE_ADC
#define INFO_ENABLE_USI
#define INFO_ENABLE_TWI
#define INFO_ENABLE_CLKO
#define INFO_ENABLE_DW
#define INFO_ENABLE_JTAG
#define INFO_ENABLE_XTAL
#define INFO_ENABLE_RESET
#endif

#ifdef INFO_ENABLE_COUNTER
#define INFO_ENABLE_COUNTER_OC
#define INFO_ENABLE_COUNTER_T
#define INFO_ENABLE_COUNTER_ICP
#endif

#ifdef INFO_ENABLE
/// Information about available pins
/**
 * These functions provide a method for querying the available pins on the device that the program is compiled for.
 * This uses a significant amount of flash memory, so they are disabled by default.
 *
 * To enable them, define the macro INFO_ENABLE.
 * 
 * Pin names take much more space than the rest of the system, so they must be separately enabled.
 *
 * To enable pin name information, define the macro INFO_ENABLE_NAMES (in addition to INFO_ENABLE).
 *
 * The full list of macros that can be defined is in the file documentation.
 */
namespace Info {

	/// Pin types returned by get_type().
	enum Type { // {{{
		/// Digital pin (Gpio)
		PT_GPIO		= 0x00,
		/// Pin change interrupt (Pcint)
		PT_PCINT	= 0x01,
		/// External interrupt (Int)
		PT_INT		= 0x02,
		/// Serial port receive (Usart)
		PT_RX		= 0x03,
		/// Serial port transmit (Usart)
		PT_TX		= 0x04,
		/// Serial port clock (Usart)
		PT_XCK		= 0x05,
		/// Slave select (Spi)
		PT_SS		= 0x06,
		/// Master out slave in (Spi)
		PT_MOSI		= 0x07,
		/// Master in slave out (Spi)
		PT_MISO		= 0x08,
		/// Clock (Spi)
		PT_SCK		= 0x09,
		/// Output compare 8 bit (Counter)
		PT_OC8		= 0x0a,
		/// Output compare 16 bit (Counter)
		PT_OC16		= 0x0b,
		/// Count input (Counter)
		PT_T		= 0x0c,
		/// Input capture trigger (Counter)
		PT_ICP		= 0x0d,
		/// Analog input (ADC)
		PT_ADC		= 0x0e,
		/// Analog comparator reference input (Comparator)
		PT_ACREF	= 0x0f,
		/// Analog comparator measurement input (Comparator)
		PT_ACIN		= 0x10,
		/// Data (Twi)
		PT_SDA		= 0x11,
		/// Clock (Twi)
		PT_SCL		= 0x12,
		/// Clock (JTAG)
		PT_TCK		= 0x13,
		/// Mode select (JTAG)
		PT_TMS		= 0x14,
		/// Data out (JTAG)
		PT_TDO		= 0x15,
		/// Data in (JTAG)
		PT_TDI		= 0x16,
		/// Debug wire (dW)
		PT_DW		= 0x17,
		/// Reset pin
		PT_RESET	= 0x18,
		/// Crystal pin or clock input
		PT_XTAL1	= 0x19,
		/// Crystal pin
		PT_XTAL2	= 0x1a,
		/// Data in (Usi)
		PT_DI		= 0x1b,
		/// Data out (Usi)
		PT_DO		= 0x1c,
		/// Clock (Usi)
		PT_USCK		= 0x1d,
		/// Clock out
		PT_CLKO		= 0x1e,
	}; // }}}

#ifdef DOXYGEN
	/// Return the first pin for which documentation is enabled.
	/**
	 * If no pins are enabled, return 0xff.
	 */
	static inline uint8_t first_pin();

	/// Return the first pin after pin for which documentation is enabled.
	/**
	 * If there are no more pins enabled, return 0xff.
	 */
	static inline uint8_t next_pin(uint8_t pin);

	/// Return the name of the pin.
	/**
	 * This returns a pointer to a static buffer.
	 *
	 * The value is overwritten when get_name is called again.
	 *
	 * If a pin has multiple functions, other functions are not included in
	 * the name. To get combined pin names, the caller must use get_id()
	 * and combine pins with equal IDs.
	 */
	static inline char *get_name(uint8_t pin);

	/// Return the type of a pin.
	static inline Type get_type(uint8_t pin);

	/// Return the ID of a pin.
	/**
	 * Pin IDs are opaque; they must only be used to compare for equality.
	 *
	 * Pins which have equal IDs are on the same hardware pin. The caller
	 * can use this information to create a list of pin functions.
	 */
	static inline uint8_t get_id(uint8_t pin);
#else
#ifdef INFO_ENABLE_NAMES
	static char buffer[8];
#endif

	// Digital pins. {{{
	static uint8_t const last_digital_pin = 0x5f;
#ifdef INFO_ENABLE_GPIO
	static inline uint8_t next_digital_port(uint8_t port) {  // {{{
		// Internal function: find first port that is not smaller than port.
		// Return port if found, 0xff if not.
		while (true) {
			if (port > GPIO_LAST_PORT)
				return 0xff;
			uint8_t mask = Gpio::MASK(port);
			if (mask != 0)
				return port;
			port += 1;
		}
	} // }}}

	static inline uint8_t first_digital_pin() { // {{{
		// Return the first existing pin number.
		uint8_t port = next_digital_port(0);
		if (port == 0xff)
			return 0xff;
		return Gpio::make_pin(port, 0);
	} // }}}

	static inline uint8_t get_digital_pin_id(uint8_t pin) { return pin; }

	static inline uint8_t next_digital_pin(uint8_t pin) { // {{{
		// Return the first pin number after pin. The order is alphabetical for ports, lowest bit to highest bit.
		// If pin is the last pin, return 0xff.
		uint8_t port = (pin >> 3) & 0xf;
		pin &= 0x7;
		pin += 1;
		while (true) {
			for (uint8_t p = pin; p < 8; ++p) {
				uint8_t mask = Gpio::MASK(port);
				if (mask & (1 << p))
					return Gpio::make_pin(port, p);
			}
			port = next_digital_port(port + 1);
			if (port == 0xff)
				return 0xff;
			pin = 0;
		}
	} // }}}

#ifdef INFO_ENABLE_NAMES
	static inline char *get_digital_pin_name(uint8_t pin) { // {{{
		buffer[0] = 'P';
		buffer[1] = 'A' + ((pin >> 3) & 0xf);
		buffer[2] = '0' + (pin & 0x7);
		buffer[3] = '\0';
		return buffer;
	} // }}}
#endif
	static inline Type get_digital_pin_type(uint8_t pin) { (void)&pin; return PT_GPIO; }
#else
	static inline uint8_t first_digital_pin() { return 0xff; }
	static inline uint8_t next_digital_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_digital_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_digital_pin_type(uint8_t pin) { (void)&pin; return PT_GPIO; }
	static inline uint8_t get_digital_pin_id(uint8_t pin) { return pin; }
#endif
	// }}}

	// External interrupts. {{{
	static uint8_t const last_int_pin = 0x7f;
#ifdef INFO_ENABLE_INT

	// Build pin struct.
	static inline uint8_t get_int_pin_id(uint8_t pin) {
		switch (pin & 7) {
#ifdef INT0_PIN
		case 0:
			return INT0_PIN;
#endif
#ifdef INT1_PIN
		case 1:
			return INT1_PIN;
#endif
#ifdef INT2_PIN
		case 2:
			return INT2_PIN;
#endif
#ifdef INT3_PIN
		case 3:
			return INT3_PIN;
#endif
#ifdef INT4_PIN
		case 4:
			return INT4_PIN;
#endif
#ifdef INT5_PIN
		case 5:
			return INT5_PIN;
#endif
#ifdef INT6_PIN
		case 6:
			return INT6_PIN;
#endif
#ifdef INT7_PIN
		case 7:
			return INT7_PIN;
#endif
		default:
			return 0xff;
		}
	}
	static inline uint8_t next_int_pin(uint8_t pin) {
		if (pin < 0x78 - 1)
			pin = 0x78 - 1;
		if ((pin & 0x7) + 1 > INT_LAST_PIN)
			return 0xff;
		while (true) {
			pin += 1;
			if (!(INT_MASK & (1 << (pin & 7))))
				continue;
			return pin;
		}
	}
	static inline uint8_t first_int_pin() {
		return next_int_pin(0);
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_int_pin_name(uint8_t pin) {
		buffer[0] = 'I';
		buffer[1] = 'N';
		buffer[2] = 'T';
		buffer[3] = '0' + (pin & 7);
		buffer[4] = '\0';
		return buffer;
	}
#endif
	static inline Type get_int_pin_type(uint8_t pin) { (void)&pin; return PT_INT; }
#else
	static inline uint8_t first_int_pin() { return 0xff; }
	static inline uint8_t get_int_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_int_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_int_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_int_pin_type(uint8_t pin) { (void)&pin; return PT_INT; }
#endif
	// }}}

	// Pin change interrupts. {{{
	static uint8_t const last_pcint_pin = 0x9f;
#ifdef INFO_ENABLE_PCINT
	static const uint8_t _AVR_PCINT_ID[] PROGMEM = _AVR_PCINT_IDS;
	static inline uint8_t first_pcint_pin() {
		return 0x80;
	}
	static inline uint8_t get_pcint_pin_id(uint8_t pin) {
		return pgm_read_byte(&_AVR_PCINT_ID[pin - first_pcint_pin()]);
	}
	static inline uint8_t next_pcint_pin(uint8_t pin) {
		uint8_t first = first_pcint_pin();
		if (pin < first)
			return first;
		if ((pin & 0x1f) + 1 < _AVR_NUM_PCINT_PINS) {
			pin += 1;
			while (get_pcint_pin_id(pin) == 0xff)
				pin += 1;
			return pin;
		}
		return 0xff;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_pcint_pin_name(uint8_t pin) {
		buffer[0] = 'P';
		buffer[1] = 'C';
		buffer[2] = 'I';
		buffer[3] = 'N';
		buffer[4] = 'T';
		pin &= 0x1f;
		if (pin < 10) {
			buffer[5] = '0' + pin;
			buffer[6] = '\0';
		}
		else {
			buffer[5] = '0' + (pin / 10);
			buffer[6] = '0' + (pin % 10);
			buffer[7] = '\0';
		}
		return buffer;
	}
#endif
	static inline Type get_pcint_pin_type(uint8_t pin) { (void)&pin; return PT_PCINT; }
#else
	static inline uint8_t first_pcint_pin() { return 0xff; }
	static inline uint8_t get_pcint_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_pcint_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_pcint_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_pcint_pin_type(uint8_t pin) { (void)&pin; return PT_PCINT; }
#endif
	// }}}

	// USART. {{{
	static uint8_t const last_usart_pin = 0xaf;
#if defined(_AVR_USART_HH) && defined(INFO_ENABLE_USART)
	static inline uint8_t first_usart_pin() {
#ifdef UDR0
		return 0xa0;
#elif defined(UDR1)
		return 0xa4;
#elif defined(UDR2)
		return 0xa8;
#else
		return 0xac;
#endif
	}
	static inline uint8_t get_usart_pin_id(uint8_t pin) {
		uint8_t usart = (pin >> 2) & 0x3;
		uint8_t usart_pins[] = _AVR_USART_IDS;
		return usart_pins[usart * 3 + (pin & 0x3)];
	}
	static inline uint8_t next_usart_pin(uint8_t pin) {
		uint8_t first = first_usart_pin();
		if (pin < first)
			return first;
		while (true) {
			if ((pin & 3) == 2) {
				uint8_t usart = (pin >> 2) & 0x3;
				if (usart + 1 >= _AVR_NUM_USART)
					return 0xff;
				pin += 2;
			}
			else
			pin += 1;
			if (get_usart_pin_id(pin) != 0xff)
				return pin;
		}
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_usart_pin_name(uint8_t pin) {
		if ((pin & 2) == 0) {
			buffer[0] = pin & 1 ? 'T' : 'R';
			buffer[1] = 'X';
			buffer[2] = 'D';
		}
		else {
			buffer[0] = 'X';
			buffer[1] = 'C';
			buffer[2] = 'K';
		}
		if (_AVR_NUM_USART == 1)
			buffer[3] = '\0';
		else {
			buffer[3] = '0' + ((pin >> 2) & 0x3);
			buffer[4] = '\0';
		}
		return buffer;
	}
#endif
	static inline Type get_usart_pin_type(uint8_t pin) { return pin & 2 ? PT_XCK : pin & 1 ? PT_TX : PT_RX; }
#else
	static inline uint8_t first_usart_pin() { return 0xff; }
	static inline uint8_t get_usart_pin_id(uint8_t pin) {
		uint8_t const ids[] = _AVR_USART_IDS;
		return ids[(pin - first_usart_pin()) >> 1] + (pin & 1);
	}
	static inline uint8_t next_usart_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_usart_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_usart_pin_type(uint8_t pin) { (void)&pin; return PT_RX; }
#endif
	// }}}

	// SPI. {{{
	static uint8_t const last_spi_pin = 0xb3;
#if defined(_AVR_SPI_HH) && defined(INFO_ENABLE_SPI)
	static inline uint8_t first_spi_pin() {
		return 0xb0;
	}
	static inline uint8_t get_spi_pin_id(uint8_t pin) {
		uint8_t const ids[] = { SS_PIN, MOSI_PIN, MISO_PIN, SCK_PIN };
		return ids[pin - first_spi_pin()];
	}
	static inline uint8_t next_spi_pin(uint8_t pin) {
		uint8_t first = first_spi_pin();
		if (pin < first)
			return first;
		if ((pin & 3) < 3)
			return pin + 1;
		return 0xff;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_spi_pin_name(uint8_t pin) {
		switch (pin & 3) {
		case 0:
			buffer[0] = 'S';
			buffer[1] = 'S';
			buffer[2] = '\0';
			break;
		case 1:
			buffer[0] = 'M';
			buffer[1] = 'O';
			buffer[2] = 'S';
			buffer[3] = 'I';
			buffer[4] = '\0';
			break;
		case 2:
			buffer[0] = 'M';
			buffer[1] = 'I';
			buffer[2] = 'S';
			buffer[3] = 'O';
			buffer[4] = '\0';
			break;
		case 3:
			buffer[0] = 'S';
			buffer[1] = 'C';
			buffer[2] = 'K';
			buffer[3] = '\0';
			break;
		}
		return buffer;
	}
#endif
	static inline Type get_spi_pin_type(uint8_t pin) {
		switch (pin & 3) {
		case 0:
			return PT_SS;
		case 1:
			return PT_MOSI;
		case 2:
			return PT_MISO;
		default:
			return PT_SCK;
		}
	}
#else
	static inline uint8_t first_spi_pin() { return 0xff; }
	static inline uint8_t get_spi_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_spi_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_spi_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_spi_pin_type(uint8_t pin) { (void)&pin; return PT_SS; }
#endif
	// }}}

	// Counter0. {{{
	static uint8_t const last_counter0_pin = 0xb7;
#if defined(_AVR_COUNTER0_HH) && (defined(INFO_ENABLE_COUNTER_OC) || defined(INFO_ENABLE_COUNTER_T))
	static inline uint8_t first_counter0_pin() {
#ifdef INFO_ENABLE_COUNTER_OC
		return 0xb4;
#else
		return 0xb6;
#endif
	}
	static inline uint8_t get_counter0_pin_id(uint8_t pin) {
		uint8_t const ids[] = { OC0A_PIN, OC0B_PIN, T0_PIN };
		return ids[pin - 0xb4];
	}
	static inline uint8_t next_counter0_pin(uint8_t pin) {
		uint8_t first = first_counter0_pin();
		if (pin < first)
			return first;
#ifdef INFO_ENABLE_COUNTER_OC
#ifdef INFO_ENABLE_COUNTER_T
		if (pin < 0xb6)
#else
		if (pin < 0xb5)
#endif
			return pin + 1;
#endif
		return 0xff;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_counter0_pin_name(uint8_t pin) {
		switch (pin & 0x3) {
#ifdef INFO_ENABLE_COUNTER_OC
		case 0:
		case 1:
			buffer[0] = 'O';
			buffer[1] = 'C';
			buffer[2] = '0';
			buffer[3] = 'A' + (pin & 1);
			buffer[4] = '\0';
			break;
#endif
#ifdef INFO_ENABLE_COUNTER_T
		case 2:
			buffer[0] = 'T';
			buffer[1] = '0';
			buffer[2] = '\0';
			break;
#endif
		default:
			return nullptr;
		}
		return buffer;
	}
#endif
	static inline Type get_counter0_pin_type(uint8_t pin) {
		(void)&pin;
#ifndef INFO_ENABLE_COUNTER_T
		return PT_OC8;
#elif defined(INFO_ENABLE_COUNTER_OC)
		if ((pin & 2) == 0)
			return PT_OC8;
#endif
		return PT_T;
	}
#else
	static inline uint8_t first_counter0_pin() { return 0xff; }
	static inline uint8_t get_counter0_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_counter0_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_counter0_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_counter0_pin_type(uint8_t pin) { (void)&pin; return PT_OC8; }
#endif
	// }}}

	// Counter1. {{{
	static uint8_t const last_counter1_pin = 0xbd;
#if defined(_AVR_COUNTER1_HH) && (defined(INFO_ENABLE_COUNTER_OC) || defined(INFO_ENABLE_COUNTER_T) || defined(INFO_ENABLE_COUNTER_ICP))
	static const uint8_t _AVR_COUNTER1_ID[] PROGMEM = {
		OC1A_PIN,
		OC1B_PIN,
#ifdef OC1C_PIN
		OC1C_PIN,
#else
		0xff,
#endif
		0xff,
#ifdef T1_PIN
		T1_PIN,
#else
		0xff,
#endif
		ICP1_PIN
	};
	static inline uint8_t first_counter1_pin() {
#ifdef INFO_ENABLE_COUNTER_OC
		return 0xb8;
#elif defined(INFO_ENABLE_COUNTER_T)
		return 0xbc;
#else
		return 0xbd;
#endif
	}
	static inline uint8_t get_counter1_pin_id(uint8_t pin) {
		return pgm_read_byte(&_AVR_COUNTER1_ID[pin - 0xb8]);
	}
	static inline uint8_t next_counter1_pin(uint8_t pin) {
		uint8_t first = first_counter1_pin();
		if (pin < first)
			return first;
#ifdef INFO_ENABLE_COUNTER_OC
		if ((pin & 0x4) == 0) {
			switch (pin & 0x3) {
			case 0:
				return pin + 1;
			case 1:
#ifdef OCR1CL
				return pin + 1;
#else
#ifdef INFO_ENABLE_COUNTER_T
				return pin + 3;
#elif defined(INFO_ENABLE_COUNTER_ICP)
				return pin + 4;
#else
				return 0xff;
#endif
#endif
			case 2:
#ifdef INFO_ENABLE_COUNTER_T
				return pin + 2;
#elif defined(INFO_ENABLE_COUNTER_ICP)
				return pin + 3;
#else
				return 0xff;
#endif
			}
		}
#endif
		if (pin & 0x1)
			return 0xff;
#ifdef INFO_ENABLE_COUNTER_ICP
		return pin + 1;
#else
		return 0xff;
#endif
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_counter1_pin_name(uint8_t pin) {
#ifdef INFO_ENABLE_COUNTER_OC
		if ((pin & 0x4) == 0) {
			buffer[0] = 'O';
			buffer[1] = 'C';
			buffer[2] = '1';
			buffer[3] = 'A' + (pin & 0x3);
			buffer[4] = '\0';
			return buffer;
		}
#endif
#ifdef INFO_ENABLE_COUNTER_T
		if ((pin & 0x1) == 0) {
			buffer[0] = 'T';
			buffer[1] = '1';
			buffer[2] = '\0';
			return buffer;
		}
#endif
#ifdef INFO_ENABLE_COUNTER_ICP
		buffer[0] = 'I';
		buffer[1] = 'C';
		buffer[2] = 'P';
		buffer[3] = '1';
		buffer[4] = '\0';
		return buffer;
#endif
	}
#endif
	static inline Type get_counter1_pin_type(uint8_t pin) {
		(void)&pin;
#ifdef INFO_ENABLE_COUNTER_OC
#if !defined(INFO_ENABLE_COUNTER_T) && !defined(INFO_ENABLE_COUNTER_ICP)
		return PT_OC16;
#else
		if ((pin & 0x4) == 0)
			return PT_OC16;
#endif
#endif
#ifdef INFO_ENABLE_COUNTER_T
#ifndef INFO_ENABLE_COUNTER_ICP
		return PT_T;
#else
		if ((pin & 0x1) == 0)
			return PT_T;
#endif
#endif
#ifdef INFO_ENABLE_COUNTER_ICP
		return PT_ICP;
#endif
	}
#else
	static inline uint8_t first_counter1_pin() { return 0xff; }
	static inline uint8_t get_counter1_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_counter1_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_counter1_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_counter1_pin_type(uint8_t pin) { (void)&pin; return PT_OC16; }
#endif
	// }}}

	// Counter2. {{{
	static uint8_t const last_counter2_pin = 0xbf;
#if defined(_AVR_COUNTER2_HH) && defined(INFO_ENABLE_COUNTER_OC)
	static inline uint8_t first_counter2_pin() {
		return 0xbe;
	}
	static inline uint8_t get_counter2_pin_id(uint8_t pin) {
		uint8_t const ids[] = { OC2A_PIN, OC2B_PIN };
		return ids[pin & 1];
	}
	static inline uint8_t next_counter2_pin(uint8_t pin) {
		uint8_t first = first_counter2_pin();
		if (pin < first)
			return first;
		if (pin < 0xbf)
			return pin + 1;
		return 0xff;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_counter2_pin_name(uint8_t pin) {
		buffer[0] = 'O';
		buffer[1] = 'C';
		buffer[2] = '2';
		buffer[3] = 'A' + (pin & 1);
		buffer[4] = '\0';
		return buffer;
	}
#endif
	static inline Type get_counter2_pin_type(uint8_t pin) { (void)&pin; return PT_OC8; }
#else
	static inline uint8_t first_counter2_pin() { return 0xff; }
	static inline uint8_t get_counter2_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_counter2_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_counter2_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_counter2_pin_type(uint8_t pin) { (void)&pin; return PT_OC8; }
#endif
	// }}}

	// Counter 3, 4, 5. {{{
	static uint8_t const last_counter3_pin = 0xdf;
#if defined(_AVR_COUNTER1_HH) && defined(TCNT3L) && (defined(INFO_ENABLE_COUNTER_OC) || defined(INFO_ENABLE_COUNTER_T) || defined(INFO_ENABLE_COUNTER_ICP))
#ifdef OC3B_PIN
#define AVR_OC3B_PIN OC3B_PIN
#else
#define AVR_OC3B_PIN 0xff
#endif
#ifdef OC3C_PIN
#define AVR_OC3C_PIN OC3C_PIN
#else
#define AVR_OC3C_PIN 0xff
#endif
#ifdef T3_PIN
#define AVR_T3_PIN T3_PIN
#else
#define AVR_T3_PIN 0xff
#endif
#define _AVR_COUNTER3_ID3 \
	OC3A_PIN, \
	AVR_OC3B_PIN, \
	AVR_OC3C_PIN, \
	0xff, \
	AVR_T3_PIN, \
	ICP3_PIN
#if defined(TCNT4L) && !defined(TCCR4E)
#ifdef OC4C_PIN
#define AVR_OC4C_PIN OC4C_PIN
#else
#define AVR_OC4C_PIN 0xff
#endif
#define _AVR_COUNTER3_ID4 \
	, 0xff, 0xff, \
	OC4A_PIN, \
	OC4B_PIN, \
	AVR_OC4C_PIN, \
	0xff, \
	T4_PIN, \
	ICP4_PIN
#else
#define _AVR_COUNTER3_ID4
#endif
#ifdef TCNT5L
#ifdef OC5C_PIN
#define AVR_OC5C_PIN OC5C_PIN
#else
#define AVR_OC5C_PIN 0xff
#endif
#define _AVR_COUNTER3_ID5 \
	, 0xff, 0xff, \
	OC5A_PIN, \
	OC5B_PIN, \
	AVR_OC5C_PIN, \
	0xff, \
	T5_PIN, \
	ICP5_PIN
#else
#define _AVR_COUNTER3_ID5
#endif
	static const uint8_t _AVR_COUNTER3_ID[] PROGMEM = { _AVR_COUNTER3_ID3 _AVR_COUNTER3_ID4 _AVR_COUNTER3_ID5 };
	static inline uint8_t get_counter3_pin_id(uint8_t pin);
	static inline uint8_t next_counter3_pin(uint8_t pin);
	static inline uint8_t first_counter3_pin() {
		uint8_t ret;
#ifdef INFO_ENABLE_COUNTER_OC
		ret = 0xc0;
#elif defined(INFO_ENABLE_COUNTER_T)
		ret = 0xc4;
#else
		ret = 0xc5;
#endif
		if (get_counter3_pin_id(ret) == 0xff)
			return next_counter3_pin(ret);
		return ret;
	}
	static inline uint8_t get_counter3_pin_id(uint8_t pin) {
		return pgm_read_byte(&_AVR_COUNTER3_ID[pin - 0xc0]);
	}
	static inline uint8_t next_counter3_pin(uint8_t pin) {
		if (pin < 0xc0)
			pin = 0xbf;
		while (pin < 0xc0 + 8 * _AVR_NUM_COUNTER3 - 2) {
			pin += 1;
			if (get_counter3_pin_id(pin) == 0xff)
				continue;
#ifndef INFO_ENABLE_COUNTER_OC
			if ((pin & 0x7) < 4)
				continue;
#endif
#ifndef INFO_ENABLE_COUNTER_T
			if ((pin & 7) == 4)
				continue;
#endif
#ifndef INFO_ENABLE_COUNTER_ICP
			if ((pin & 7) == 5)
				continue;
#endif
			return pin;
		}
		return 0xff;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_counter3_pin_name(uint8_t pin) {
		uint8_t counter = (pin >> 3) & 0x3;
#ifdef INFO_ENABLE_COUNTER_OC
		if ((pin & 0x4) == 0) {
			buffer[0] = 'O';
			buffer[1] = 'C';
			buffer[2] = '3' + counter;
			buffer[3] = 'A' + (pin & 0x3);
			buffer[4] = '\0';
			return buffer;
		}
#endif
#ifdef INFO_ENABLE_COUNTER_T
		if ((pin & 0x1) == 0) {
			buffer[0] = 'T';
			buffer[1] = '3' + counter;
			buffer[2] = '\0';
			return buffer;
		}
#endif
#ifdef INFO_ENABLE_COUNTER_ICP
		buffer[0] = 'I';
		buffer[1] = 'C';
		buffer[2] = 'P';
		buffer[3] = '3' + counter;
		buffer[4] = '\0';
		return buffer;
#endif
	}
#endif
	static inline Type get_counter3_pin_type(uint8_t pin) {
		(void)&pin;
#ifdef INFO_ENABLE_COUNTER_OC
#if !defined(INFO_ENABLE_COUNTER_T) && !defined(INFO_ENABLE_COUNTER_ICP)
		return PT_OC16;
#else
		if ((pin & 0x4) == 0)
			return PT_OC16;
#endif
#endif
#ifdef INFO_ENABLE_COUNTER_T
#ifndef INFO_ENABLE_COUNTER_ICP
		return PT_T;
#else
		if ((pin & 0x1) == 0)
			return PT_T;
#endif
#endif
#ifdef INFO_ENABLE_COUNTER_ICP
		return PT_ICP;
#endif
	}
#else
	static inline uint8_t first_counter3_pin() { return 0xff; }
	static inline uint8_t get_counter3_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_counter3_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_counter3_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_counter3_pin_type(uint8_t pin) { (void)&pin; return PT_OC16; }
#endif
	// }}}

	// Comparator. {{{
	static uint8_t const last_comparator_pin = 0xbf;
#if defined(_AVR_COMPARATOR_HH) && defined(INFO_ENABLE_COMPARATOR)
	static inline uint8_t first_comparator_pin() {
		return 0xbe;
	}
	static inline uint8_t get_comparator_pin_id(uint8_t pin) {
#ifdef AIN1_PIN
		uint8_t const ids[] = { AIN0_PIN, AIN1_PIN };
		return ids[pin & 1];
#else
		(void)&pin;
		return AIN0_PIN;
#endif
	}
	static inline uint8_t next_comparator_pin(uint8_t pin) {
		uint8_t first = first_comparator_pin();
		if (pin < first)
			return first;
#ifdef AIN1_PIN
		if ((pin & 1) == 0)
			return pin + 1;
#endif
		return 0xff;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_comparator_pin_name(uint8_t pin) {
		buffer[0] = 'A';
		buffer[1] = 'I';
		buffer[2] = 'N';
		buffer[3] = '0' + (pin & 1);
		buffer[4] = '\0';
		return buffer;
	}
#endif
	static inline Type get_comparator_pin_type(uint8_t pin) { return pin & 1 ? PT_ACIN : PT_ACREF; }
#else
	static inline uint8_t first_comparator_pin() { return 0xff; }
	static inline uint8_t get_comparator_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_comparator_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_comparator_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_comparator_pin_type(uint8_t pin) { (void)&pin; return PT_ACREF; }
#endif
	// }}}

	// ADC input. {{{
	static uint8_t const last_adc_pin = 0xef;
#if defined(_AVR_ADC_HH) && defined(INFO_ENABLE_ADC)
	static const uint8_t _AVR_ADC_ID[] PROGMEM = _AVR_ADC_IDS;
	static inline uint8_t first_adc_pin() {
		return 0xe0;
	}
	static inline uint8_t get_adc_pin_id(uint8_t pin) {
		return pgm_read_byte(&_AVR_ADC_ID[pin - first_adc_pin()]);
	}
	static inline uint8_t next_adc_pin(uint8_t pin) {
		uint8_t first = first_adc_pin();
		if (pin < first)
			return first;
		if ((pin & 0xf) + 1 < _AVR_NUM_ADC_PINS) {
			while (true) {
				pin += 1;
				if (get_adc_pin_id(pin) == 0xff)
					continue;
				return pin;
			}
		}
		return 0xff;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_adc_pin_name(uint8_t pin) {
		buffer[0] = 'A';
		buffer[1] = 'D';
		buffer[2] = 'C';
		pin &= 0xf;
		if (pin < 10) {
			buffer[3] = '0' + pin;
			buffer[4] = '\0';
		}
		else {
			buffer[3] = '1';
			buffer[4] = '0' + (pin - 10);
			buffer[5] = '\0';
		}
		return buffer;
	}
#endif
	static inline Type get_adc_pin_type(uint8_t pin) { (void)&pin; return PT_ADC; }
#else
	static inline uint8_t first_adc_pin() { return 0xff; }
	static inline uint8_t get_adc_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_adc_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_adc_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_adc_pin_type(uint8_t pin) { (void)&pin; return PT_ADC; }
#endif
	// }}}

	// USI. {{{
	static uint8_t const last_usi_pin = 0xf2;
#if defined(_AVR_USI_HH) && defined(INFO_ENABLE_USI)
	static inline uint8_t first_usi_pin() {
		return 0xf0;
	}
	static inline uint8_t get_usi_pin_id(uint8_t pin) {
		uint8_t const ids[] = { DI_PIN, DO_PIN, USCK_PIN };
		return ids[pin & 3];
	}
	static inline uint8_t next_usi_pin(uint8_t pin) {
		uint8_t first = first_usi_pin();
		if (pin < first)
			return first;
		if (pin == last_usi_pin)
			return 0xff;
		return pin + 1;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_usi_pin_name(uint8_t pin) {
		switch(pin & 3) {
		case 0:
			buffer[0] = 'D';
			buffer[1] = 'I';
			buffer[2] = '\0';
			return buffer;
		case 1:
			buffer[0] = 'D';
			buffer[1] = 'O';
			buffer[2] = '\0';
			return buffer;
		default:
			buffer[0] = 'U';
			buffer[1] = 'S';
			buffer[2] = 'C';
			buffer[3] = 'K';
			buffer[4] = '\0';
			return buffer;
		}
	}
#endif
	static inline Type get_usi_pin_type(uint8_t pin) {
		switch (pin & 0x3) {
		case 0:
			return PT_DI;
		case 1:
			return PT_DO;
		default:
			return PT_USCK;
		}
	}
#else
	static inline uint8_t first_usi_pin() { return 0xff; }
	static inline uint8_t get_usi_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_usi_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_usi_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_usi_pin_type(uint8_t pin) { (void)&pin; return PT_DI; }
#endif
	// }}}

	// TWI. {{{
	static uint8_t const last_twi_pin = 0xf5;
#if defined(_AVR_TWI_HH) && defined(INFO_ENABLE_TWI)
	static inline uint8_t first_twi_pin() {
		return 0xf4;
	}
	static inline uint8_t get_twi_pin_id(uint8_t pin) {
		uint8_t const ids[] = { SCL_PIN, SDA_PIN };
		return ids[pin & 1];
	}
	static inline uint8_t next_twi_pin(uint8_t pin) {
		uint8_t first = first_twi_pin();
		if (pin < first)
			return first;
		if (pin & 1)
			return 0xff;
		return pin + 1;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_twi_pin_name(uint8_t pin) {
		buffer[0] = 'S';
		if (pin & 1) {
			buffer[1] = 'D';
			buffer[2] = 'A';
		}
		else {
			buffer[1] = 'C';
			buffer[2] = 'L';
		}
		buffer[3] = '\0';
		return buffer;
	}
#endif
	static inline Type get_twi_pin_type(uint8_t pin) { return pin & 1 ? PT_SDA : PT_SCL; }
#else
	static inline uint8_t first_twi_pin() { return 0xff; }
	static inline uint8_t get_twi_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_twi_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_twi_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_twi_pin_type(uint8_t pin) { (void)&pin; return PT_SCL; }
#endif
	// }}}

	// CLKO. {{{
	static uint8_t const last_clko_pin = 0xf6;
#if defined(_AVR_CLKO_PIN) && defined(INFO_ENABLE_CLKO)
	static inline uint8_t first_clko_pin() {
		uint8_t first = first_clko_pin();
		if (pin < first)
			return first;
		return 0xf6;
	}
	static inline uint8_t get_clko_pin_id(uint8_t pin) {
		(void)&pin;
		return CLKO_PIN;
	}
	static inline uint8_t next_clko_pin(uint8_t pin) {
		uint8_t first = first_clko_pin();
		if (pin < first)
			return first;
		return 0xff;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_clko_pin_name(uint8_t pin) {
		(void)&pin;
		buffer[0] = 'C';
		buffer[1] = 'L';
		buffer[2] = 'K';
		buffer[3] = 'O';
		buffer[4] = '\0';
		return buffer;
	}
#endif
	static inline Type get_clko_pin_type(uint8_t pin) { (void)&pin; return PT_CLKO; }
#else
	static inline uint8_t first_clko_pin() { return 0xff; }
	static inline uint8_t get_clko_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_clko_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_clko_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_clko_pin_type(uint8_t pin) { (void)&pin; return PT_CLKO; }
#endif
	// }}}

	// dW. {{{
	static uint8_t const last_dw_pin = 0xf7;
#if defined(DW_PIN) && defined(INFO_ENABLE_DW)
	static inline uint8_t first_dw_pin() {
		return 0xf7;
	}
	static inline uint8_t get_dw_pin_id(uint8_t pin) {
		(void)&pin;
		return DW_PIN;
	}
	static inline uint8_t next_dw_pin(uint8_t pin) {
		uint8_t first = first_dw_pin();
		if (pin < first)
			return first;
		return 0xff;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_dw_pin_name(uint8_t pin) {
		(void)&pin;
		buffer[0] = 'd';
		buffer[1] = 'W';
		buffer[2] = '\0';
		return buffer;
	}
#endif
	static inline Type get_dw_pin_type(uint8_t pin) { (void)&pin; return PT_DW; }
#else
	static inline uint8_t first_dw_pin() { return 0xff; }
	static inline uint8_t get_dw_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_dw_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_dw_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_dw_pin_type(uint8_t pin) { (void)&pin; return PT_DW; }
#endif
	// }}}

	// JTAG. {{{
	// TCK, TMS, TDO, TDI.
	static uint8_t const last_jtag_pin = 0xfb;
#if defined(TCK_PIN) && defined(INFO_ENABLE_JTAG)
	static inline uint8_t first_jtag_pin() {
		return 0xf8;
	}
	static inline uint8_t get_jtag_pin_id(uint8_t pin) {
		uint8_t const ids[] = { TCK_PIN, TMS_PIN, TDO_PIN, TDI_PIN };
		return ids[pin & 3];
	}
	static inline uint8_t next_jtag_pin(uint8_t pin) {
		uint8_t first = first_jtag_pin();
		if (pin < first)
			return first;
		if (pin == last_jtag_pin)
			return 0xff;
		return pin + 1;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_jtag_pin_name(uint8_t pin) {
		buffer[0] = 'T';
		switch(pin & 3) {
		case 0:
			buffer[1] = 'C';
			buffer[2] = 'K';
			break;
		case 1:
			buffer[1] = 'M';
			buffer[2] = 'S';
			break;
		case 2:
			buffer[1] = 'D';
			buffer[2] = 'O';
			break;
		default:
			buffer[1] = 'D';
			buffer[2] = 'I';
			break;
		}
		buffer[3] = '\0';
		return buffer;
	}
#endif
	static inline Type get_jtag_pin_type(uint8_t pin) {
		switch (pin & 0x3) {
		case 0:
			return PT_TCK;
		case 1:
			return PT_TMS;
		case 2:
			return PT_TDO;
		default:
			return PT_TDI;
		}
	}
#else
	static inline uint8_t first_jtag_pin() { return 0xff; }
	static inline uint8_t get_jtag_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_jtag_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_jtag_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_jtag_pin_type(uint8_t pin) { (void)&pin; return PT_TCK; }
#endif
	// }}}

	// XTAL. {{{
	static uint8_t const last_xtal_pin = 0xfd;
#if defined(XTAL1_PIN) && defined(INFO_ENABLE_XTAL)
	static inline uint8_t first_xtal_pin() {
		return 0xfc;
	}
	static inline uint8_t get_xtal_pin_id(uint8_t pin) {
		uint8_t const ids[] = { XTAL1_PIN, XTAL2_PIN };
		return ids[pin & 1];
	}
	static inline uint8_t next_xtal_pin(uint8_t pin) {
		uint8_t first = first_xtal_pin();
		if (pin < first)
			return first;
		if (pin & 1)
			return 0xff;
		return pin + 1;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_xtal_pin_name(uint8_t pin) {
		buffer[0] = 'X';
		buffer[1] = 'T';
		buffer[2] = 'A';
		buffer[3] = 'L';
		buffer[4] = '1' + (pin & 1);
		buffer[5] = '\0';
		return buffer;
	}
#endif
	static inline Type get_xtal_pin_type(uint8_t pin) { return pin & 1 ? PT_XTAL2 : PT_XTAL1; }
#else
	static inline uint8_t first_xtal_pin() { return 0xff; }
	static inline uint8_t get_xtal_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_xtal_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_xtal_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_xtal_pin_type(uint8_t pin) { (void)&pin; return PT_XTAL1; }
#endif
	// }}}

	// RESET. {{{
	static uint8_t const last_reset_pin = 0xfe;
#if defined(RESET_PIN) && defined(INFO_ENABLE_RESET)
	static inline uint8_t first_reset_pin() {
		return 0xfe;
	}
	static inline uint8_t get_reset_pin_id(uint8_t pin) {
		(void)&pin;
		return RESET_PIN;
	}
	static inline uint8_t next_reset_pin(uint8_t pin) {
		uint8_t first = first_reset_pin();
		if (pin < first)
			return first;
		return 0xff;
	}
#ifdef INFO_ENABLE_NAMES
	static inline char *get_reset_pin_name(uint8_t pin) {
		(void)&pin;
		buffer[0] = 'R';
		buffer[1] = 'E';
		buffer[2] = 'S';
		buffer[3] = 'E';
		buffer[4] = 'T';
		buffer[5] = '\0';
		return buffer;
	}
#endif
	static inline Type get_reset_pin_type(uint8_t pin) { (void)&pin; return PT_RESET; }
#else
	static inline uint8_t first_reset_pin() { return 0xff; }
	static inline uint8_t get_reset_pin_id(uint8_t pin) { return pin; }
	static inline uint8_t next_reset_pin(uint8_t pin) { (void)&pin; return 0xff; }
#ifdef INFO_ENABLE_NAMES
	static inline char *get_reset_pin_name(uint8_t pin) { (void)&pin; return nullptr; }
#endif
	static inline Type get_reset_pin_type(uint8_t pin) { (void)&pin; return PT_RESET; }
#endif
	// }}}

	// Interface for all pins. {{{
#define _AVR_INFO_FIRST(op) \
	ret = first_ ## op ## _pin(); \
	if (ret != 0xff) \
		return ret;

	static inline uint8_t first_pin() {
		uint8_t
		_AVR_INFO_FIRST(digital)
		_AVR_INFO_FIRST(int)
		_AVR_INFO_FIRST(pcint)
		_AVR_INFO_FIRST(usart)
		_AVR_INFO_FIRST(spi)
		_AVR_INFO_FIRST(counter0)
		_AVR_INFO_FIRST(counter1)
		_AVR_INFO_FIRST(counter2)
		_AVR_INFO_FIRST(counter3)
		_AVR_INFO_FIRST(comparator)
		_AVR_INFO_FIRST(adc)
		_AVR_INFO_FIRST(usi)
		_AVR_INFO_FIRST(twi)
		_AVR_INFO_FIRST(clko)
		_AVR_INFO_FIRST(dw)
		_AVR_INFO_FIRST(jtag)
		_AVR_INFO_FIRST(xtal)
		return first_reset_pin();
	}

#define _AVR_INFO_NEXT(op) \
	if (pin <= last_ ## op ## _pin) { \
		uint8_t ret = next_ ## op ## _pin(pin); \
		if (ret != 0xff) \
			return ret; \
	}

	static inline uint8_t next_pin(uint8_t pin) {
		_AVR_INFO_NEXT(digital)
		_AVR_INFO_NEXT(int)
		_AVR_INFO_NEXT(pcint)
		_AVR_INFO_NEXT(usart)
		_AVR_INFO_NEXT(spi)
		_AVR_INFO_NEXT(counter0)
		_AVR_INFO_NEXT(counter1)
		_AVR_INFO_NEXT(counter2)
		_AVR_INFO_NEXT(counter3)
		_AVR_INFO_NEXT(comparator)
		_AVR_INFO_NEXT(adc)
		_AVR_INFO_NEXT(usi)
		_AVR_INFO_NEXT(twi)
		_AVR_INFO_NEXT(clko)
		_AVR_INFO_NEXT(dw)
		_AVR_INFO_NEXT(jtag)
		_AVR_INFO_NEXT(xtal)
		return next_reset_pin(pin);
	}

#define _AVR_INFO_ONE_CHECK(op, current) \
	first = first_ ## current ## _pin(); \
	if (first != 0xff && pin <= last_ ## current ## _pin) \
		return get_ ## current ## _pin_ ## op (pin);

#define _AVR_INFO_FULL_CHECK(op, rettype) \
	static inline rettype get_ ## op(uint8_t pin) { \
		uint8_t \
		_AVR_INFO_ONE_CHECK(op, digital) \
		_AVR_INFO_ONE_CHECK(op, int) \
		_AVR_INFO_ONE_CHECK(op, pcint) \
		_AVR_INFO_ONE_CHECK(op, usart) \
		_AVR_INFO_ONE_CHECK(op, spi) \
		_AVR_INFO_ONE_CHECK(op, counter0) \
		_AVR_INFO_ONE_CHECK(op, counter1) \
		_AVR_INFO_ONE_CHECK(op, counter2) \
		_AVR_INFO_ONE_CHECK(op, counter3) \
		_AVR_INFO_ONE_CHECK(op, comparator) \
		_AVR_INFO_ONE_CHECK(op, adc) \
		_AVR_INFO_ONE_CHECK(op, usi) \
		_AVR_INFO_ONE_CHECK(op, twi) \
		_AVR_INFO_ONE_CHECK(op, clko) \
		_AVR_INFO_ONE_CHECK(op, dw) \
		_AVR_INFO_ONE_CHECK(op, jtag) \
		_AVR_INFO_ONE_CHECK(op, xtal) \
		return get_reset_pin_ ## op(pin); \
	}

#ifdef INFO_ENABLE_NAMES
	_AVR_INFO_FULL_CHECK(name, char *);
#endif
	_AVR_INFO_FULL_CHECK(type, Type);
	_AVR_INFO_FULL_CHECK(id, uint8_t);
	// }}}
#endif
}
#endif

#ifdef AVR_TEST_INFO // {{{

#define INFO_PROTOCOL_VERSION 0

#if AVR_TEST_INDEX == 0

namespace Info {
	static const uint8_t testcode = '#';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('V');
			Test::tx('0');
			Test::tx('+');
			Test::tx('n');
			Test::tx('t');
			Test::tx('i');
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	V	return protocol version.
		//	0	first pin.
		//	+XX	first pin after pin with id XX.
		//	nXX	name of pin XX.
		//	tXX	type of pin XX.
		//	iXX	id of pin XX.
		if (cmd == 'V') {
			Test::tx(testcode);
			Test::send_byte(INFO_PROTOCOL_VERSION);
			Test::tx('\n');
			return true;
		}
		else if (cmd == '0') {
			if (len != 0)
				return false;
			uint8_t p = first_pin();
			Test::tx(testcode);
			Test::send_byte(p);
			Test::tx('\n');
			return true;
		}
		else if (len != 2)
			return false;
		bool ok = true;
		uint8_t p = Test::read_byte(0, ok);
		if (!ok)
			return false;
		switch (cmd) {
		case '+':
			p = next_pin(p);
			break;
		case 'n':
		{
			char *name = get_name(p);
			Test::tx(testcode);
			while (*name)
				Test::tx(*name++);
			Test::tx('\n');
			return true;
		}
		case 't':
			p = get_type(p);
			break;
		case 'i':
			p = get_id(p);
			break;
		default:
			return false;
		}
		Test::tx(testcode);
		Test::send_byte(p);
		Test::tx('\n');
		return true;
	}
}

#else 

namespace Info {
	static const uint8_t testcode = 0;
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		(void)&part;
		(void)&cmd;
		(void)&len;
		return false;
	}
}
#endif
#endif // }}}

#endif

// vim: set foldmethod=marker :
