// Code for producing testing firmware.

// Options:
// AVR_TEST_* for testing individual parts.

#ifndef _AVR_TEST_HH
#define _AVR_TEST_HH
// This is the first time this file is included.

/**
 * @file
 * Code for internal testing of avr-ll
 *
 * @author Bas Wijnen <wijnen@debian.org>
 */

/// @cond

#ifdef AVR_TEST_ALL
#define AVR_TEST_ADC
#define AVR_TEST_CLOCK
#define AVR_TEST_COMPARATOR
#define AVR_TEST_COUNTER0
#define AVR_TEST_COUNTER1
#define AVR_TEST_COUNTER2
#define AVR_TEST_EEPROM
#define AVR_TEST_GPIO
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

#ifdef AVR_TEST

// Testing system: usart0 must be available for this to work.
// All components are enabled.
// usart0 is enabled as uart and receives commands from host at 115200n1.
// Commands consist of a single byte to specify a target part, plus arguments.
// Commands end with '\n'.
// Part codes are:
// test (information about the test system): '?'
// info (information about the pins): '#'
// adc: 'A'
// clock: 'O' (oscillator)
// comparator: 'C'
// counter0: '0'
// counter1: '1'
// counter2: '2'
// counter3: '3' (handled by counter1 code)
// counter4: '4' (handled by counter1 code)
// counter5: '5' (handled by counter1 code)
// eeprom: 'E'
// gpio: 'P' (port/pin)
// hscounter: 'H'
// interrupt: 'I'
// pin change interrupt: 'V' (up, down, up) (handled by interrupt code)
// sleep: 'Z' (zzz)
// spi: 'S'
// twi: 'T'
// usart: 'X' (rX/tX)
// usb: 'U'
// usi: 'Y' (like X, but different)
// wdt: 'W'

// Parts define their own test function.
// When a complete packet is received (at '\n'), all test functions are called until one returns true.
// If none of them returns true, an error is returned.
// Returned packets start with the part code and end with '\n'.
// The error part code is '!'.

// Declare some things that the communication needs. // {{{{
namespace Test {
	static void tx(uint8_t c);
	static uint8_t parse_digit(uint8_t c);
	static uint8_t parse_byte(uint8_t h, uint8_t l, bool &ok);
	static uint8_t read_digit(uint8_t pos);
	static uint8_t read_byte(uint8_t pos, bool &ok);
	static void send_digit(uint8_t c);
	static void send_byte(uint8_t b);

// These are only used if SPI or TWI, or at least 1 extra usart, is present.
#if (defined(SPDR) && defined(AVR_TEST_SPI)) || (defined(TWDR) && defined(AVR_TEST_TWI)) || (defined(UDR1) && defined(AVR_TEST_USART))
#define _AVR_TEST_NUM_STRINGS 3
	static char const *strings[_AVR_TEST_NUM_STRINGS] = {"Hello", "World", "Stage"};
	static uint8_t stringlengths[_AVR_TEST_NUM_STRINGS] = {5, 5, 5};
#endif
} // }}}

#ifdef USBCON
namespace Usart {
	// Usb testing code needs this to be declared.
	static bool tx1_write(uint8_t c);
	static inline void tx1_block_done();
}
#include "usb.hh"
#else
#define USART_TX_SIZE 30
#define USART_RX_SIZE 10
// Include serial port code, as this is used by other files.
#include "usart.hh"
#endif

// Convenience functions for tests.
namespace Test {
	static void tx(uint8_t c) {
#ifndef USBCON
		Usart::tx(c);
#else
		Usb::test_tx(c);
#endif
	}
	static uint8_t rx_read(uint8_t pos) {
#ifndef USBCON
		return Usart::rx_read(pos);
#else
		return usb_read(pos);
#endif
	}
	static void rx_pop(uint8_t num = 1) {
#ifndef USBCON
		Usart::rx_pop(num);
#else
		usb_rx_pop(num);
#endif
	}
	static uint8_t parse_digit(uint8_t c) {
		if (c >= '0' && c <= '9')
			return c - '0';
		if (c >= 'a' && c <= 'f')
			return c - 'a' + 10;
		else
			return 0xff;
	}
	static uint8_t parse_byte(uint8_t h, uint8_t l, bool &ok) {
		h = parse_digit(h);
		if (h == 0xff) {
			ok = false;
			return 0xff;
		}
		l = parse_digit(l);
		if (l == 0xff) {
			ok = false;
			return 0xff;
		}
		return (h << 4) | l;
	}
	static uint8_t read_digit(uint8_t pos) {
		return parse_digit(rx_read(pos));
	}
	static uint8_t read_byte(uint8_t pos, bool &ok) {
		return parse_byte(rx_read(pos), rx_read(pos + 1), ok);
	}
	static void send_digit(uint8_t c) {
		if (c < 10)
			Test::tx('0' + c);
		else if (c < 0x10)
			Test::tx('a' + c - 10);
		else
			Test::tx('!');
	}
	static void send_byte(uint8_t b) {
		send_digit(b >> 4);
		send_digit(b & 0xf);
	}
}

#endif

/// @endcond

#else
// This is the second time this file is included.

/// @cond

#ifdef AVR_TEST

// If the mcu has USB, use that for communication; otherwise use Usart.
#ifdef USBCON
static void usb_rx(uint8_t b, uint8_t len) {
#else
static void usart_rx(uint8_t b, uint8_t len) {
#endif
	if (b == '\r') {
#ifdef USART0_ECHO
		Test::tx('\n');
#endif
	}
	else if (b != '\n') {
		return;
	}
	if (len == 1) {
		Test::rx_pop();
		return;
	}
	uint8_t part = Test::rx_read(0);
	if (len == 2 && part == '?') { // {{{
		// Test management: return available test commands.
		Test::tx('?');
#ifdef AVR_TEST_CLOCK
		Test::tx(Clock::testcode);
#endif
#ifdef AVR_TEST_SLEEP
		Test::tx(Sleep::testcode);
#endif
#ifdef AVR_TEST_WDT
		Test::tx(Wdt::testcode);
#endif
#ifdef AVR_TEST_EEPROM
		Test::tx(Eeprom::testcode);
#endif
#ifdef AVR_TEST_GPIO
		Test::tx(Gpio::testcode);
#endif
#ifdef AVR_TEST_INT
		Test::tx(Int::testcode);
#endif
#ifdef AVR_TEST_PCINT
		Test::tx(Pcint::testcode);
#endif
#ifdef AVR_TEST_INFO
		Test::tx(Info::testcode);
#endif
#ifdef AVR_TEST_USART
		Test::tx(Usart::testcode);
#endif
#ifdef AVR_TEST_COUNTER0
		Test::tx(Counter::testcode0);
#endif
#ifdef AVR_TEST_COUNTER1
		Test::tx(Counter::testcode1);
#ifdef TCNT3L
		Test::tx(Counter::testcode3);
#endif
#if defined(TCNT4L) && !defined(TCCR4E)
		Test::tx(Counter::testcode4);
#endif
#ifdef TCNT5L
		Test::tx(Counter::testcode5);
#endif
#endif
#if defined(_AVR_COUNTER2_HH) && defined(AVR_TEST_COUNTER2)
		Test::tx(Counter::testcode2);
#endif
#if defined(_AVR_ADC_HH) && defined(AVR_TEST_ADC)
		Test::tx(Adc::testcode);
#endif
#if defined(_AVR_COMPARATOR_HH) && defined(AVR_TEST_COMPARATOR)
		Test::tx(Comparator::testcode);
#endif
#if defined(_AVR_SPI_HH) && defined(AVR_TEST_SPI)
		Test::tx(Spi::testcode);
#endif
#if defined(_AVR_TWI_HH) && defined(AVR_TEST_TWI)
		Test::tx(Twi::testcode);
#endif
#if defined(_AVR_USB_HH) && defined(AVR_TEST_USB)
		Test::tx(Usb::testcode);
#endif
#if defined(_AVR_USI_HH) && defined(AVR_TEST_USI)
		Test::tx(Usi::testcode);
#endif
		Test::tx('\n');
		Test::rx_pop(len);
		return;
	} // }}}
	uint8_t cmd = Test::rx_read(1);
	Test::rx_pop(2);
	len -= 2;
	if (len < 1 || !(false // {{{
#ifdef AVR_TEST_CLOCK
			|| Clock::test(part, cmd, len - 1)
#endif
#ifdef AVR_TEST_SLEEP
			|| Sleep::test(part, cmd, len - 1)
#endif
#ifdef AVR_TEST_WDT
			|| Wdt::test(part, cmd, len - 1)
#endif
#ifdef AVR_TEST_EEPROM
			|| Eeprom::test(part, cmd, len - 1)
#endif
#ifdef AVR_TEST_GPIO
			|| Gpio::test(part, cmd, len - 1)
#endif
#ifdef AVR_TEST_INT
			|| Int::test(part, cmd, len - 1)
#endif
#ifdef AVR_TEST_PCINT
			|| Pcint::test(part, cmd, len - 1)
#endif
#ifdef AVR_TEST_INFO
			|| Info::test(part, cmd, len - 1)
#endif
#ifdef AVR_TEST_USART
			|| Usart::test(part, cmd, len - 1)
#endif
#ifdef AVR_TEST_COUNTER0
			|| Counter::test0(part, cmd, len - 1)
#endif
#ifdef AVR_TEST_COUNTER1
			|| Counter::test1(part, cmd, len - 1)
#endif
#if defined(_AVR_COUNTER2_HH) && defined(AVR_TEST_COUNTER2)
			|| Counter::test2(part, cmd, len - 1)
#endif
#if defined(_AVR_ADC_HH) && defined(AVR_TEST_ADC)
			|| Adc::test(part, cmd, len - 1)
#endif
#if defined(_AVR_COMPARATOR_HH) && defined(AVR_TEST_COMPARATOR)
			|| Comparator::test(part, cmd, len - 1)
#endif
#if defined(_AVR_SPI_HH) && defined(AVR_TEST_SPI)
			|| Spi::test(part, cmd, len - 1)
#endif
#if defined(_AVR_TWI_HH) && defined(AVR_TEST_TWI)
			|| Twi::test(part, cmd, len - 1)
#endif
#if defined(_AVR_USB_HH) && defined(AVR_TEST_USB)
			|| Usb::test(part, cmd, len - 1)
#endif
#if defined(_AVR_USI_HH) && defined(AVR_TEST_USI)
			|| Usi::test(part, cmd, len - 1)
#endif
		     )) { // }}}
		Test::tx('!');
		Test::tx(part);
		Test::tx(cmd);
		for (uint8_t i = 0; i < len; ++i)
			Test::tx(Test::rx_read(i));
		if (b == '\r')
			Test::tx('\n');
	}
	Test::rx_pop(len);
}

static void setup() {
	// Always enable all usarts when testing.
	Usart::start();
#ifdef USBCON
	Usb::start();
	dbg(">\n");
#else
	Usart::rx_reset();
#endif
	//Test::tx('>');
	//Test::tx('\n');
	sei();
#ifdef AVR_TEST_SLEEP
	Sleep::pending_cmd = 0;
#endif
}

#ifdef AVR_TEST_SLEEP
#define CALL_loop
static void loop() {
	uint8_t cmd = Sleep::pending_cmd;
	if (cmd == 0) {
		Sleep::idle();
		return;
	}
	cli();
	Test::tx('Z');
	Test::tx('[');
	Test::tx('\n');
	sei();
	switch (cmd) {
	case 'i':
		Sleep::idle();
		break;
	case 'd':
		Sleep::power_down();
		break;
#ifdef BODS
	case 'I':
		Sleep::idle_nobod();
		break;
	case 'D':
		Sleep::power_down_nobod();
		break;
#endif
#ifdef SM2
	case 's':
		Sleep::standby();
		break;
	case 'a':
		Sleep::adc_noise_reduction();
		break;
	case 'p':
		Sleep::power_save();
		break;
	case 'e':
		Sleep::extended_standby();
		break;
#ifdef BODS
	case 'S':
		Sleep::standby_nobod();
		break;
	case 'A':
		Sleep::adc_noise_reduction_nobod();
		break;
	case 'P':
		Sleep::power_save_nobod();
		break;
	case 'E':
		Sleep::extended_standby_nobod();
		break;
#endif
	default:
		cli();
		Test::tx('Z');
		Test::tx('!');
		Test::tx('\n');
		sei();
		break;
#endif
	}
	Sleep::pending_cmd = 0;
	cli();
	Test::tx('Z');
	Test::tx(']');
	Test::tx('\n');
	sei();
}
#endif

#endif

/// @endcond

#endif
// vim: set foldmethod=marker :
