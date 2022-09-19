// USART

// Options:
// USART_RX0_SIZE
// USART_STOP2
// USART_FALLING_SAMPLE
// USART_BAUD
// USART_BITS
// USART_PARITY
// USART_MODE
// USART_TX0_SIZE
// USART_TX_SIZE
// USART_RX_SIZE
// USART0_ENABLE_RX
// USART_ENABLE_RX

#ifndef _AVR_USART_HH
#define _AVR_USART_HH

/**
 * @file
 * Universial synchronous/asynchronous receiver/transmitter
 *
 * @author Bas Wijnen <wijnen@debian.org>
 */

/// @cond
// Make sure all names are defined by the same name. {{{
#if !defined(USART0_RX_vect) && defined(USART_RX_vect)
#define USART0_RX_vect USART_RX_vect
#define USART0_UDRE_vect USART_UDRE_vect
#define USART0_TX_vect USART_TX_vect
#endif

#if !defined(UDR0) && defined(UDR)

#define UDR0 UDR
#define UBRR0L UBRRL
#define UBRR0H UBRRH

#define UCSR0A UCSRA
#define RXC0 RXC
#define TXC0 TXC
#define UDRE0 UDRE
#define FE0 FE
#define DOR0 DOR
#define UPE0 UPE
#define U2X0 U2X
#define MPCM0 MPCM

#define UCSR0B UCSRB
#define RXCIE0 RXCIE
#define TXCIE0 TXCIE
#define UDRIE0 UDRIE
#define RXEN0 RXEN
#define TXEN0 TXEN
#define UCSZ02 UCSZ2
#define RXB80 RXB8
#define TXB80 TXB8

#define UCSR0C UCSRC
#define UMSEL00 UMSEL
#define UPM01 UPM1
#define UPM00 UPM0
#define USBS0 USBS
#define UCSZ01 UCSZ1
#define UCSZ00 UCSZ0
#define UCPOL0 UCPOL

#endif

#ifndef USART_RX_vect
#ifdef UDR0
#define USART_RX_vect USART0_RX_vect
#define USART_UDRE_vect USART0_UDRE_vect
#define USART_TX_vect USART0_TX_vect
#else
#define USART_RX_vect USART1_RX_vect
#define USART_UDRE_vect USART1_UDRE_vect
#define USART_TX_vect USART1_TX_vect
#endif

#ifdef UDR0
#define RXD_PIN RXD0_PIN
#define TXD_PIN TXD0_PIN
#define XCK_PIN XCK0_PIN
#else
#define RXD_PIN RXD1_PIN
#define TXD_PIN TXD1_PIN
#define XCK_PIN XCK1_PIN
#endif
#endif
// }}}
/// @endcond

/// Universial synchronous/asynchronous receiver/transmitter
/**
 * Only functions for usart0 are documented. The same functions are available for usart 1, 2 and 3 (if supported in hardware).
 */
namespace Usart {
	/// Operational mode of the usart
	enum Mode {
		/// Asynchronous operation; no clock on XCK.
		ASYNC = 0,
		/// Synchronous master; clock is generated on XCK.
		MASTER = 1,
		/// Synchronous slave; clock is received on XCK.
		SLAVE = 2,
		/// SPI master; clock is generated on XCK.
		SPI = 3
	};

	/// Parity setting.
	enum Parity {
		/// No parity bit.
		P_NONE = 0,
		/// Even parity.
		P_EVEN = 2,
		/// Odd parity.
		P_ODD = 3
	};
#ifdef DOXYGEN

	/// Enable the usart.
	static inline void enable0();

	/// Disable the usart
	static inline void disable0();

	/// Enable the interrupt when a byte is received.
	/**
	 * ISR(USART0_RX_vect) must be defined if this is called.
	 */
	static inline void enable_rxc0();

	/// Disable the interrupt when a byte is received.
	static inline void disable_rxc0();

	/// Enable the interrupt for when sending of a byte has completed.
	/**
	 * ISR(USART0_TX_vect) must be defined if this is called.
	 */
	static inline void enable_txc0();

	/// Disable the interrupt for when sending of a byte has completed.
	static inline void disable_txc0();

	/// Enable the interrupt for when the hardware buffer can accept a byte.
	/**
	 * ISR(USART0_UDRE_vect) must be defined if this is called.
	 */
	static inline void enable_dre0();

	/// Disable the interrupt for when the hardware buffer can accept a byte.
	static inline void disable_dre0();

	/// Busy wait until a byte can be placed in the hardware buffer.
	static inline void tx0_block_ready();

	/// Busy wait until no byte is being sent anymore.
	static inline void tx0_block_done();

	/// Busy wait until a byte is received.
	static inline void rx0_block();

	/// Enable multi processor communication mode.
	static inline void enable_mpcm0();

	/// Disable multi processor communication mode.
	static inline void disable_mpcm0();

	/// Disable power to the usart.
	static inline void off0();

	/// Enable power to the usart.
	static inline void on0();

	/// When USART_RX0_SIZE is defined, this function is called when data is received.
	static void usart_rx0(uint8_t last_byte, uint8_t len);


/// When this is defined, a STREAM_BUFFER named Usart::rx0 is created. @ingroup usemacros
/**
 * It's value is the size of the buffer.
 *
 * The interrupt is enabled as part of enable0().
 *
 * When data is received, usart_rx0() is called.
 *
 * This same macro exists for Usart1, 2 and 3 (if they exist in hardware).
 *
 * USART_RX_SIZE is an alias for the first existing Usart.
 */
#define USART_RX0_SIZE

/// When this is defined, a STREAM_BUFFER named Usart::tx0 is created. @ingroup usemacros
/**
 * It's value is the size of the buffer.
 *
 * The interrupt is enabled when tx0_write() is called.
 *
 * This same macro exists for Usart1, 2 and 3 (if they exist in hardware).
 *
 * USART_TX_SIZE is an alias for the first existing Usart.
 */
#define USART_TX0_SIZE

/// Baud rate.
#define USART0_BAUD 115200

/// Bits per word.
#define USART0_BITS 8

/// Parity type: P_NONE, P_EVEN, or P_ODD.
#define USART0_PARITY

/// Port type: ASYNC, MASTER, SLAVE, or SPI.
#define USART0_MODE ASYNC

/// If this is defined, use 2 stop bits.
#define USART0_STOP2

/// If this is defined for a synchronous mode, sample on falling edge.
#define USART0_FALLING_SAMPLE

/// Echo received data back to port @ingroup usemacros
/**
 * When this is defined, every byte that is received on Usart0 is sent back.
 *
 * This same macro exists for Usart1, 2 and 3 (if they exist in hardware).
 *
 * USART_ECHO is an alias for the first existing Usart.
 */
#define USART0_ECHO

#endif

/// @cond
	// Prepare macros for auto-enabling receive interrupt for buffer. {{{
#ifdef USART_RX0_SIZE
#define _AVR_USART_ENABLE_RXC0 enable_rxc0();
#else
#define _AVR_USART_ENABLE_RXC0
#endif

#ifdef USART_RX1_SIZE
#define _AVR_USART_ENABLE_RXC1 enable_rxc1();
#else
#define _AVR_USART_ENABLE_RXC1
#endif

#ifdef USART_RX2_SIZE
#define _AVR_USART_ENABLE_RXC2 enable_rxc2();
#else
#define _AVR_USART_ENABLE_RXC2
#endif

#ifdef USART_RX3_SIZE
#define _AVR_USART_ENABLE_RXC3 enable_rxc3();
#else
#define _AVR_USART_ENABLE_RXC3
#endif
	// }}}

// Initialize settings. {{{
#ifndef USART_BAUD
#define USART_BAUD 115200
#endif
#ifndef USART_BITS
#define USART_BITS 8
#endif
#ifndef USART_PARITY
#define USART_PARITY P_NONE
#endif
#ifndef USART_MODE
#define USART_MODE ASYNC
#endif
#ifdef USART_STOP2
#define USART_STOP2_VALUE true
#else
#define USART_STOP2_VALUE false
#endif
#ifdef USART_FALLING_SAMPLE
#define USART_FALLING_SAMPLE_VALUE true
#else
#define USART_FALLING_SAMPLE_VALUE false
#endif

#ifdef UDR0
#ifndef USART0_BAUD
#define USART0_BAUD USART_BAUD
#endif
#ifndef USART0_BITS
#define USART0_BITS USART_BITS
#endif
#ifndef USART0_PARITY
#define USART0_PARITY USART_PARITY
#endif
#ifndef USART0_MODE
#define USART0_MODE USART_MODE
#endif
#ifdef USART0_STOP2
#define USART0_STOP2_VALUE true
#else
#define USART0_STOP2_VALUE USART_STOP2_VALUE
#endif
#ifdef USART0_FALLING_SAMPLE
#define USART0_FALLING_SAMPLE_VALUE true
#else
#define USART0_FALLING_SAMPLE_VALUE USART_FALLING_SAMPLE_VALUE
#endif
#endif

#ifdef UDR1
#ifndef USART1_BAUD
#define USART1_BAUD USART_BAUD
#endif
#ifndef USART1_BITS
#define USART1_BITS USART_BITS
#endif
#ifndef USART1_PARITY
#define USART1_PARITY USART_PARITY
#endif
#ifndef USART1_MODE
#define USART1_MODE USART_MODE
#endif
#ifdef USART1_STOP2
#define USART1_STOP2_VALUE true
#else
#define USART1_STOP2_VALUE USART_STOP2_VALUE
#endif
#ifdef USART1_FALLING_SAMPLE
#define USART1_FALLING_SAMPLE_VALUE true
#else
#define USART1_FALLING_SAMPLE_VALUE USART_FALLING_SAMPLE_VALUE
#endif
#endif

#ifdef UDR2
#ifndef USART2_BAUD
#define USART2_BAUD USART_BAUD
#endif
#ifndef USART2_BITS
#define USART2_BITS USART_BITS
#endif
#ifndef USART2_PARITY
#define USART2_PARITY USART_PARITY
#endif
#ifndef USART2_MODE
#define USART2_MODE USART_MODE
#endif
#ifdef USART2_STOP2
#define USART2_STOP2_VALUE true
#else
#define USART2_STOP2_VALUE USART_STOP2_VALUE
#endif
#ifdef USART2_FALLING_SAMPLE
#define USART2_FALLING_SAMPLE_VALUE true
#else
#define USART2_FALLING_SAMPLE_VALUE USART_FALLING_SAMPLE_VALUE
#endif
#endif

#ifdef UDR3
#ifndef USART3_BAUD
#define USART3_BAUD USART_BAUD
#endif
#ifndef USART3_BITS
#define USART3_BITS USART_BITS
#endif
#ifndef USART3_PARITY
#define USART3_PARITY USART_PARITY
#endif
#ifndef USART3_MODE
#define USART3_MODE USART_MODE
#endif
#ifdef USART3_STOP2
#define USART3_STOP2_VALUE true
#else
#define USART3_STOP2_VALUE USART_STOP2_VALUE
#endif
#ifdef USART3_FALLING_SAMPLE
#define USART3_FALLING_SAMPLE_VALUE true
#else
#define USART3_FALLING_SAMPLE_VALUE USART_FALLING_SAMPLE_VALUE
#endif
#endif
// }}}

#define _AVR_USART(idx) /* {{{ */ \
	static inline void enable ## idx(bool rx = true, bool tx = true) { \
		UCSR ## idx ## A = _BV(U2X ## idx); \
		UCSR ## idx ## B = (USART ## idx ## _BITS == 9 ? _BV(UCSZ ## idx ## 2) : 0) | (rx ? _BV(RXEN ## idx) : 0) | (tx ? _BV(TXEN ## idx) : 0); \
		UCSR ## idx ## C = ((USART ## idx ## _MODE == SLAVE ? MASTER : USART ## idx ## _MODE) << UMSEL ## idx ## 0) | (USART ## idx ## _PARITY << UPM ## idx ## 0) | (USART ## idx ## _STOP2_VALUE ? _BV(USBS ## idx) : 0) | ((USART ## idx ## _BITS == 9 ? 3 : USART ## idx ## _BITS - 5) << UCSZ ## idx ## 0) | (USART ## idx ## _FALLING_SAMPLE_VALUE ? _BV(UCPOL ## idx) : 0); \
		if (USART ## idx ## _MODE != SLAVE) { \
			unsigned const ubrr = F_CPU / ((USART ## idx ## _MODE == ASYNC ? 8L : 2L) * (long)USART ## idx ## _BAUD) - 1; \
			UBRR ## idx ## H = ubrr >> 8; \
			UBRR ## idx ## L = ubrr & 0xff; \
		} \
		_AVR_USART_ENABLE_RXC ## idx \
		if (USART ## idx ## _MODE == MASTER || USART ## idx ## _MODE == SPI) \
			Gpio::DDR(XCK ## idx ## _PIN >> 3) |= _BV(XCK ## idx ## _PIN & 0x7); \
		else if (USART ## idx ## _MODE == SLAVE) \
			Gpio::DDR(XCK ## idx ## _PIN >> 3) &= ~_BV(XCK ## idx ## _PIN & 0x7); \
	} \
	static inline void disable ## idx() { UCSR ## idx ## B = 0; } \
	static inline void enable_rxc ## idx() { UCSR ## idx ## B |= _BV(RXCIE ## idx); } \
	static inline void disable_rxc ## idx() { UCSR ## idx ## B &= ~_BV(RXCIE ## idx); } \
	static inline void enable_txc ## idx() { UCSR ## idx ## B |= _BV(TXCIE ## idx); } \
	static inline void disable_txc ## idx() { UCSR ## idx ## B &= ~_BV(TXCIE ## idx); } \
	static inline void enable_dre ## idx() { UCSR ## idx ## B |= _BV(UDRIE ## idx); } \
	static inline void disable_dre ## idx() { UCSR ## idx ## B &= ~_BV(UDRIE ## idx); } \
	static inline void tx ## idx ## _block_ready() { while (!(UCSR ## idx ## A & _BV(UDRE ## idx))) {} } \
	static inline void tx ## idx ## _block_done() { while (!(UCSR ## idx ## A & _BV(TXC ## idx))) {} } \
	static inline void rx ## idx ## _block() { while (!(UCSR ## idx ## A & _BV(RXC ## idx))) {} } \
	static inline void enable_mpcm ## idx() { UCSR ## idx ## A = _BV(U2X ## idx) | _BV(MPCM ## idx); } \
	static inline void disable_mpcm ## idx() { UCSR ## idx ## A = _BV(U2X ## idx); }

#define _AVR_USART_PRR(idx, prr) \
	static inline void off ## idx() { PRR ## prr |= _BV(PRUSART ## idx); } \
	static inline void on ## idx() { PRR ## prr &= ~_BV(PRUSART ## idx); }
	// }}}

	// Define all usarts and set _AVR_NUM_USART {{{
#ifdef UDR3
	_AVR_USART(3)
	_AVR_USART_PRR(3, 1)
#ifndef _AVR_NUM_USART
#define _AVR_NUM_USART 4
#endif
#define _AVR_USART3_PINS , RXD3_PIN, TXD3_PIN, XCK3_PIN
#elif defined(_AVR_NUM_USART)
#define _AVR_USART3_PINS , 0xff, 0xff, 0xff
#else
#define _AVR_USART3_PINS
#endif

#ifdef UDR2
	_AVR_USART(2)
	_AVR_USART_PRR(2, 1)
#ifndef _AVR_NUM_USART
#define _AVR_NUM_USART 3
#endif
#define _AVR_USART2_PINS , RXD2_PIN, TXD2_PIN, XCK2_PIN
#elif defined(_AVR_NUM_USART)
#define _AVR_USART2_PINS , 0xff, 0xff, 0xff
#else
#define _AVR_USART2_PINS
#endif

#ifdef UDR1
	_AVR_USART(1)
#endif
#ifdef PRR0
#ifdef UDR2
	_AVR_USART_PRR(1, 1)
#else
	_AVR_USART_PRR(1, 0)
#endif
#ifndef _AVR_NUM_USART
#define _AVR_NUM_USART 2
#endif
#define _AVR_USART1_PINS , RXD1_PIN, TXD1_PIN, XCK1_PIN
#elif defined(_AVR_NUM_USART)
#define _AVR_USART1_PINS , 0xff, 0xff, 0xff
#else
#define _AVR_USART1_PINS
#endif

#ifdef UDR0
	_AVR_USART(0)
#ifdef PRR0
	_AVR_USART_PRR(0, 0)
#endif
#ifndef _AVR_NUM_USART
#define _AVR_NUM_USART 1
#endif
#define _AVR_USART0_PINS RXD0_PIN, TXD0_PIN, XCK0_PIN
#elif defined(_AVR_NUM_USART)
#define _AVR_USART0_PINS 0xff, 0xff, 0xff
#else
#error "parts/usart.hh is included, but no usart is detected."
#endif

#define _AVR_USART_IDS { _AVR_USART0_PINS _AVR_USART1_PINS _AVR_USART2_PINS _AVR_USART3_PINS } 
	// }}}

	// Use first usart without the index. {{{
#ifdef PRR0
#define _AVR_DEFAULT_USART_PRR(n) \
	static inline void off() { off ## n(); } \
	static inline void on() { on ## n(); }
#else
#define _AVR_DEFAULT_USART_PRR(n)
#endif
#define _AVR_DEFAULT_USART(n) \
	static inline void enable(bool rx = true, bool tx = true) { enable ## n(rx, tx); } \
	static inline void disable() { disable ## n(); } \
	static inline void enable_rxc() { enable_rxc ## n(); } \
	static inline void disable_rxc() { disable_rxc ## n(); } \
	static inline void enable_txc() { enable_txc ## n(); } \
	static inline void disable_txc() { disable_txc ## n(); } \
	static inline void enable_dre() { enable_dre ## n(); } \
	static inline void disable_dre() { disable_dre ## n(); } \
	static inline void tx_block_ready() { tx ## n ## _block_ready(); } \
	static inline void tx_block_done() { tx ## n ## _block_done(); } \
	static inline void rx_block() { rx ## n ## _block(); } \
	static inline void enable_mpcm() { enable_mpcm ## n(); } \
	static inline void disable_mpcm() { disable_mpcm ## n(); } \
	_AVR_DEFAULT_USART_PRR(n)

#ifdef UDR0
_AVR_DEFAULT_USART(0)
#ifdef USART_ENABLE_RX
#define USART0_ENABLE_RX USART_ENABLE_RX
#endif
#elif defined(UDR1)
_AVR_DEFAULT_USART(1)
#ifdef USART_ENABLE_RX
#define USART1_ENABLE_RX USART_ENABLE_RX
#endif
#elif defined(UDR2)
_AVR_DEFAULT_USART(2)
#ifdef USART_ENABLE_RX
#define USART2_ENABLE_RX USART_ENABLE_RX
#endif
#elif defined(UDR3)
_AVR_DEFAULT_USART(3)
#ifdef USART_ENABLE_RX
#define USART3_ENABLE_RX USART_ENABLE_RX
#endif
#else
#error "usart.hh included, but no usart detected."
#endif

#if defined(USART0_ENABLE_RX) && defined(UDR0)
#define USART_RX0_SIZE USART0_ENABLE_RX
#ifndef USART0_SETTINGS
#define USART0_SETTINGS
#endif
#define _AVR_USART0_RX_SETUP \
	Usart::enable0(USART0_SETTINGS); \
	Usart::enable_rxc0();
#else
#define _AVR_USART0_RX_SETUP
#endif

#if defined(USART1_ENABLE_RX) && defined(UDR1)
#define USART_RX1_SIZE USART1_ENABLE_RX
#ifndef USART1_SETTINGS
#define USART1_SETTINGS
#endif
#define _AVR_USART1_RX_SETUP \
	Usart::enable1(USART1_SETTINGS); \
	Usart::enable_rxc1();
#else
#define _AVR_USART1_RX_SETUP
#endif

#if defined(USART2_ENABLE_RX) && defined(UDR2)
#define USART_RX2_SIZE USART2_ENABLE_RX
#ifndef USART2_SETTINGS
#define USART2_SETTINGS
#endif
#define _AVR_USART2_RX_SETUP \
	Usart::enable2(USART2_SETTINGS); \
	Usart::enable_rxc2();
#else
#define _AVR_USART2_RX_SETUP
#endif

#if defined(USART3_ENABLE_RX) && defined(UDR3)
#define USART_RX3_SIZE USART3_ENABLE_RX
#ifndef USART3_SETTINGS
#define USART3_SETTINGS
#endif
#define _AVR_USART3_RX_SETUP \
	Usart::enable3(USART3_SETTINGS); \
	Usart::enable_rxc3();
#else
#define _AVR_USART3_RX_SETUP
#endif

#ifdef AVR_TEST_USART
	// For testing, define all buffers (if the port is unavailable, this will be ignored).
	// Port 0 is not tested, because it is the port used for test management.
#define USART_TX1_SIZE 10
#define USART_TX2_SIZE 10
#define USART_TX3_SIZE 10
#define USART_RX1_SIZE 10
#define USART_RX2_SIZE 10
#define USART_RX3_SIZE 10
#endif

// If this file is included by the MCU, there is at least one usart. If 0 does not exist, 1 does.
#ifdef USART_TX_SIZE
#ifdef UDR0
#define USART_TX0_SIZE USART_TX_SIZE
#else
#define USART_TX1_SIZE USART_TX_SIZE
#endif
#endif

#ifdef USART_RX_SIZE
#ifdef UDR0
#define USART_RX0_SIZE USART_RX_SIZE
#ifdef USART_ECHO
#define USART0_ECHO
#endif
#else
#define USART_RX1_SIZE USART_RX_SIZE
#ifdef USART_ECHO
#define USART1_ECHO
#endif
#endif
#endif
	// }}}
/// @endcond

	// Define tx code. This must only be defined in one source file. {{{
/// @cond
#define _AVR_USART_TX_CODE(idx) /* {{{ */ \
	/* The first _AVR_NOP is to ignore the arguments to can_read; the second is to ignore the invocation of can_write. */ \
	STREAM_BUFFER_WITH_CBS(tx ## idx, USART_TX ## idx ## _SIZE, enable_dre ## idx();_AVR_NOP,) \
	ISR(USART ## idx ## _UDRE_vect) { \
		while (tx ## idx ## _buffer_used() > 0 && (UCSR ## idx ## A & _BV(UDRE ## idx))) { \
			UDR ## idx = tx ## idx ## _read(0); \
			tx ## idx ## _pop(1); \
		} \
		if (tx ## idx ## _buffer_used() == 0) \
			disable_dre ## idx(); \
	} // }}}
/// @endcond

	// Instantiate requested tx code. {{{
#if defined(UDR0) && defined(USART_TX0_SIZE)
	_AVR_USART_TX_CODE(0)
#ifndef _AVR_USART_TX_DEFINED
#define _AVR_USART_TX_DEFINED
	static inline void tx_reset() { return tx0_reset(); }
	static inline uint8_t tx_buffer_allocated_size() { return tx0_buffer_allocated_size(); }
	static inline uint8_t tx_buffer_used() { return tx0_buffer_used(); }
	static inline uint8_t tx_buffer_available() { return tx0_buffer_available(); }
	static inline void tx_pop(uint8_t num = 1) { return tx0_pop(num); }
	static inline uint8_t tx_read(uint8_t pos = 0) { return tx0_read(pos); }
	static inline void tx_move(uint8_t *buffer, uint8_t num) { return tx0_move(buffer, num); }
	static inline bool tx_write(uint8_t data) { return tx0_write(data); }
#endif
#endif

#if defined(UDR1) && defined(USART_TX1_SIZE)
	_AVR_USART_TX_CODE(1)
#ifndef _AVR_USART_TX_DEFINED
#define _AVR_USART_TX_DEFINED
	static inline void tx_reset() { return tx1_reset(); }
	static inline uint8_t tx_buffer_allocated_size() { return tx1_buffer_allocated_size(); }
	static inline uint8_t tx_buffer_used() { return tx1_buffer_used(); }
	static inline uint8_t tx_buffer_available() { return tx1_buffer_available(); }
	static inline void tx_pop(uint8_t num = 1) { return tx1_pop(num); }
	static inline uint8_t tx_read(uint8_t pos = 0) { return tx1_read(pos); }
	static inline void tx_move(uint8_t *buffer, uint8_t num) { return tx1_move(buffer, num); }
	static inline bool tx_write(uint8_t data) { return tx1_write(data); }
#endif
#endif

#if defined(UDR2) && defined(USART_TX2_SIZE)
	_AVR_USART_TX_CODE(2)
#ifndef _AVR_USART_TX_DEFINED
#define _AVR_USART_TX_DEFINED
	static inline void tx_reset() { return tx2_reset(); }
	static inline uint8_t tx_buffer_allocated_size() { return tx2_buffer_allocated_size(); }
	static inline uint8_t tx_buffer_used() { return tx2_buffer_used(); }
	static inline uint8_t tx_buffer_available() { return tx2_buffer_available(); }
	static inline void tx_pop(uint8_t num = 1) { return tx2_pop(num); }
	static inline uint8_t tx_read(uint8_t pos = 0) { return tx2_read(pos); }
	static inline void tx_move(uint8_t *buffer, uint8_t num) { return tx2_move(buffer, num); }
	static inline bool tx_write(uint8_t data) { return tx2_write(data); }
#endif
#endif

#if defined(UDR3) && defined(USART_TX3_SIZE)
	_AVR_USART_TX_CODE(3)
#ifndef _AVR_USART_TX_DEFINED
#define _AVR_USART_TX_DEFINED
	static inline void tx_reset() { return tx3_reset(); }
	static inline uint8_t tx_buffer_allocated_size() { return tx3_buffer_allocated_size(); }
	static inline uint8_t tx_buffer_used() { return tx3_buffer_used(); }
	static inline uint8_t tx_buffer_available() { return tx3_buffer_available(); }
	static inline void tx_pop(uint8_t num = 1) { return tx3_pop(num); }
	static inline uint8_t tx_read(uint8_t pos = 0) { return tx3_read(pos); }
	static inline void tx_move(uint8_t *buffer, uint8_t num) { return tx3_move(buffer, num); }
	static inline bool tx_write(uint8_t data) { return tx3_write(data); }
#endif
#endif
	// }}}
	// }}}

	// Define rx code. This must only be defined in one source file. {{{
/// @cond
	// Define echo values. {{{
#ifdef USART0_ECHO
#ifdef USART_TX0_SIZE
#define _AVR_USART0_ECHO tx0_write(data);
#else
#define _AVR_USART0_ECHO UDR0 = data;
#endif
#else
#define _AVR_USART0_ECHO
#endif

#ifdef USART1_ECHO
#ifdef USART_TX1_SIZE
#define _AVR_USART1_ECHO tx1_write(data);
#else
#define _AVR_USART1_ECHO UDR1 = data;
#endif
#else
#define _AVR_USART1_ECHO
#endif

#ifdef USART2_ECHO
#ifdef USART_TX2_SIZE
#define _AVR_USART2_ECHO tx2_write(data);
#else
#define _AVR_USART2_ECHO UDR2 = data;
#endif
#else
#define _AVR_USART2_ECHO
#endif

#ifdef USART3_ECHO
#ifdef USART_TX3_SIZE
#define _AVR_USART3_ECHO tx3_write(data);
#else
#define _AVR_USART3_ECHO UDR3 = data;
#endif
#else
#define _AVR_USART3_ECHO
#endif
	// }}}

#define _AVR_USART_RX_CODE(idx) /* {{{ */ \
	STREAM_BUFFER_WITH_CBS(rx ## idx, USART_RX ## idx ## _SIZE, usart_rx ## idx, enable_rxc ## idx();) \
	ISR(USART ## idx ## _RX_vect) { \
		_AVR_USART ## idx ## _ECHO; \
		if (!rx ## idx ## _write(UDR ## idx)) \
			disable_rxc ## idx(); \
	} /* }}} */
/// @endcond

	// Instantiate requested rx code. {{{
#if defined(UDR0) && defined(USART_RX0_SIZE)
#ifndef _AVR_USART_RX_DEFINED
#define usart_rx usart_rx0
#endif
} static void usart_rx0(uint8_t data, uint8_t len); namespace Usart {
	_AVR_USART_RX_CODE(0)
#ifndef _AVR_USART_RX_DEFINED
#define _AVR_USART_RX_DEFINED
	static inline void rx_reset() { rx0_reset(); }
	static inline uint8_t rx_buffer_allocated_size() { return rx0_buffer_allocated_size(); }
	static inline uint8_t rx_buffer_used() { return rx0_buffer_used(); }
	static inline uint8_t rx_buffer_available() { return rx0_buffer_available(); }
	static inline void rx_pop(uint8_t num = 1) { rx0_pop(num); }
	static inline uint8_t rx_read(uint8_t pos = 0) { return rx0_read(pos); }
	static inline void rx_move(uint8_t *buffer, uint8_t num) { rx0_move(buffer, num); }
	static inline bool rx_write(uint8_t data) { return rx0_write(data); }
#endif
#endif

#if defined(UDR1) && defined(USART_RX1_SIZE)
#ifndef _AVR_USART_RX_DEFINED
#define usart_rx usart_rx1
#endif
} static void usart_rx1(uint8_t data, uint8_t len); namespace Usart {
	_AVR_USART_RX_CODE(1)
#ifndef _AVR_USART_RX_DEFINED
#define _AVR_USART_RX_DEFINED
	static inline void rx_reset() { rx1_reset(); }
	static inline uint8_t rx_buffer_allocated_size() { return rx1_buffer_allocated_size(); }
	static inline uint8_t rx_buffer_used() { return rx1_buffer_used(); }
	static inline uint8_t rx_buffer_available() { return rx1_buffer_available(); }
	static inline void rx_pop(uint8_t num = 1) { rx1_pop(num); }
	static inline uint8_t rx_read(uint8_t pos = 0) { return rx1_read(pos); }
	static inline void rx_move(uint8_t *buffer, uint8_t num) { rx1_move(buffer, num); }
	static inline bool rx_write(uint8_t data) { return rx1_write(data); }
#endif
#endif

#if defined(UDR2) && defined(USART_RX2_SIZE)
#ifndef _AVR_USART_RX_DEFINED
#define usart_rx usart_rx2
#endif
} static void usart_rx2(uint8_t data, uint8_t len); namespace Usart {
	_AVR_USART_RX_CODE(2)
#ifndef _AVR_USART_RX_DEFINED
#define _AVR_USART_RX_DEFINED
	static inline void rx_reset() { rx2_reset(); }
	static inline uint8_t rx_buffer_allocated_size() { return rx2_buffer_allocated_size(); }
	static inline uint8_t rx_buffer_used() { return rx2_buffer_used(); }
	static inline uint8_t rx_buffer_available() { return rx2_buffer_available(); }
	static inline void rx_pop(uint8_t num = 1) { rx2_pop(num); }
	static inline uint8_t rx_read(uint8_t pos = 0) { return rx2_read(pos); }
	static inline void rx_move(uint8_t *buffer, uint8_t num) { rx2_move(buffer, num); }
	static inline bool rx_write(uint8_t data) { return rx2_write(data); }
#endif
#endif

#if defined(UDR3) && defined(USART_RX3_SIZE)
#ifndef _AVR_USART_RX_DEFINED
#define usart_rx usart_rx3
#endif
} static void usart_rx3(uint8_t data, uint8_t len); namespace Usart {
	_AVR_USART_RX_CODE(3)
#ifndef _AVR_USART_RX_DEFINED
#define _AVR_USART_RX_DEFINED
	static inline void rx_reset() { rx3_reset(); }
	static inline uint8_t rx_buffer_allocated_size() { return rx3_buffer_allocated_size(); }
	static inline uint8_t rx_buffer_used() { return rx3_buffer_used(); }
	static inline uint8_t rx_buffer_available() { return rx3_buffer_available(); }
	static inline void rx_pop(uint8_t num = 1) { rx3_pop(num); }
	static inline uint8_t rx_read(uint8_t pos = 0) { return rx3_read(pos); }
	static inline void rx_move(uint8_t *buffer, uint8_t num) { rx3_move(buffer, num); }
	static inline bool rx_write(uint8_t data) { return rx3_write(data); }
#endif
#endif
	// }}}
	// }}}
}

/// @cond
#define _AVR_SETUP_USART \
	_AVR_USART0_RX_SETUP \
	_AVR_USART1_RX_SETUP \
	_AVR_USART2_RX_SETUP \
	_AVR_USART3_RX_SETUP
/// @endcond

#ifdef AVR_TEST_USART // {{{

#if AVR_TEST == 0
namespace Usart {
	static const uint8_t testcode = 'X';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('\n');
			return true;
		}
		switch (cmd) {
		}
		(void)&len;
		// TODO: implement usart test functions.
		return false;
	}

	// When testing, always enable all usarts.
	static void start() {
#ifdef UDR0
		enable0();
#endif
#ifdef UDR1
		enable1();
#endif
#ifdef UDR2
		enable2();
#endif
#ifdef UDR3
		enable3();
#endif
	}
}

#define _AVR_TEST_USART_RX(N) \
	static void usart_rx ## N(uint8_t data, uint8_t len) { \
		Test::tx(Usart::testcode); \
		Test::send_byte(data); \
		for (uint8_t i = 0; i < len; ++i) \
			Test::send_byte(Usart::rx ## N ## _read(i)); \
		Test::tx('\n'); \
		Usart::rx ## N ## _pop(len); \
	}

#if defined(UDR0) && defined(USBCON)
// Use Usart0 for communication (and not for testing) unless this mcu has USB support.
_AVR_TEST_USART_RX(0)
#endif
#ifdef UDR1
_AVR_TEST_USART_RX(1)
#endif
#ifdef UDR2
_AVR_TEST_USART_RX(2)
#endif
#ifdef UDR3
_AVR_TEST_USART_RX(3)
#endif

#else 

namespace Usart {
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
