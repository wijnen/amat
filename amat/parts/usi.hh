#ifndef _AVR_USI_HH
#define _AVR_USI_HH

/**
 * @file
 * Universial Serial Interface
 *
 * @author Bas Wijnen <wijnen@debian.org>
 */

// Options:
// USI_ENABLE_SPI_MASTER
// USI_ENABLE_SPI_SLAVE
// USI_ENABLE_TWI_MASTER
// USI_ENABLE_TWI_SLAVE
// USI_ENABLE_UART_TX	Enable 6n1 serial output
// USI_TWI_ADDRESS	Only used for TWI slave
// USI_TWI_ADDRESS_MASK
// USI_TX_SIZE
// USI_TX_PACKETS
// USI_RX_SIZE
// USI_RX_PACKETS
// CALL_usi_rx

/*
   For 3-wire mode (SPI):
   - enable as master
   - enable as slave
   - send data using ring buffer
   - receive data using ring buffer

   For 2-wire mode (TWI):
   - enable as master
   - enable as slave with address
   - send data using ring buffer
   - receive data using ring buffer

   For UART_TX:
   - enable
   - send data using ring buffer


   Clock sources (USICS0:1)
00: Software clock strobe on USICLK
01: Timer0 compare match
1x: External
	10: positive edge
	11: negative edge

With external clock:
USICLK == 0: counter clock source is external, both edges
USICLK == 1: counter clock source is software, USITC
   */

#ifdef CALL_usi_rx
static void usi_rx();
#endif

/// Universal Serial Interface
/**
 * Usi is currently not implemented, so there is also no documentation yet.
 */
namespace Usi {
	inline static void disable() { USICR &= ~(3 << USIWM0); }
	inline static void enable_spi_master() {
		Gpio::write(PIN_USCK, false);
		Gpio::input(PIN_DI, false);
		Gpio::write(PIN_DO, false);
		USICR = (USICR & (_BV(USISIE) | _BV(USIOIE))) | (_BV(USIWM0) | _BV(USICS0));
	}
	inline static void enable_spi_slave() {
		Gpio::input(PIN_USCK, false);
		Gpio::input(PIN_DI, false);
		Gpio::write(PIN_DO, false);
		USICR = (USICR & (_BV(USISIE) | _BV(USIOIE))) | (_BV(USIWM0) | _BV(USICS1) | _BV(USICS0));
	}
	inline static void enable_twi_master() {
		Gpio::write(PIN_USCK, false);
		Gpio::write(PIN_DI, false);
		USICR = (USICR & (_BV(USISIE) | _BV(USIOIE))) | (_BV(USIWM0) | _BV(USIWM1) | _BV(USICS0));
	}
	inline static void enable_twi_slave() {
		Gpio::input(PIN_USCK, false);
		Gpio::input(PIN_DI, false);
		USICR = (USICR & (_BV(USISIE) | _BV(USIOIE))) | (_BV(USIWM1) | _BV(USICS1));
	}
	inline static void enable_uart_tx() {
		Gpio::write(PIN_DO, true);
		USICR = (USICR & (_BV(USISIE) | _BV(USIOIE))) | _BV(USICS0);	// Set wire mode to "Outputs"; set to 3-wire mode when sending a byte.
	}
	inline static void enable_start_condition_int() { USICR |= _BV(USISIE); }
	inline static void disable_start_condition_int() { USICR &= ~_BV(USISIE); }
	inline static void enable_overflow_int() { USICR |= _BV(USIOIE); }
	inline static void disable_overflow_int() { USICR &= ~_BV(USIOIE); }
	inline static bool have_start() { return USISR & _BV(USISIF); }
	inline static bool have_overflow() { return USISR & _BV(USIOIF); }
	inline static bool have_stop() { return USISR & _BV(USIPF); }
	inline static void clear_start() { USISR = (USISR & 0xf) | _BV(USISIF); }
	inline static void clear_overflow() { USISR = (USISR & 0xf) | _BV(USIOIF); }
	inline static void clear_stop() { USISR = (USISR & 0xf) | _BV(USIPF); }
	inline static bool collision() { return USISR & _BV(USIDC); }
	inline static void write_counter(uint8_t value) { USISR = value; }	// Write 0 to flags, to leave them intact.
	inline static uint8_t read_counter() { return USISR & (0x0f << USICNT0); }
	inline static void write_data(uint8_t data) { USIDR = data; }
	inline static uint8_t read_data() { return USIBR; }
	inline static uint8_t read_raw_data() { return USIDR; }

#ifdef USI_ENABLE_SPI_MASTER
// TODO {{{
// }}}
#elif defined(USI_ENABLE_SPI_SLAVE)
// TODO {{{
// }}}
#elif defined(USI_ENABLE_TWI_MASTER)
// TODO {{{
// }}}
#elif defined(USI_ENABLE_TWI_SLAVE)
// {{{
#ifndef USI_TWI_ADDRESS_MASK
#define USI_TWI_ADDRESS_MASK 0x7f
#endif
#ifdef USI_TX_SIZE
#ifndef USI_TX_PACKETS
#define USI_TX_PACKETS 2
#endif
	PACKET_BUFFER(tx, USI_TX_SIZE, USI_TX_PACKETS)
#endif
#ifdef USI_RX_SIZE
#ifndef USI_RX_PACKETS
#define USI_RX_PACKETS 2
#endif
#ifdef CALL_usi_rx
	PACKET_BUFFER_WITH_CBS(rx, USI_RX_SIZE, USI_RX_PACKETS,usi_rx();,)
#else
	PACKET_BUFFER(rx, USI_RX_SIZE, USI_RX_PACKETS)
#endif
#endif
	enum UsiTwiState {
		STARTING,
		READING,
		WRITING,
		IDLE,
		SEND_ACK,
		RECV_ACK
	};
	static volatile enum UsiTwiState twi_state;
	ISR(USI_STR_vect) {
		switch (twi_state) {
		case SEND_ACK:
		case READING:
			rx_end();
			break;
		case RECV_ACK:
		case WRITING:
			if (tx_packets_available() > 0)
				tx_pop();
			break;
		default:
			break;
		}
		Gpio::input(PIN_DI, false);
		twi_state = STARTING;
		enable_overflow_int();
		write_counter(0);
		clear_overflow();
		clear_start();
	}
	ISR(USI_OVF_vect) {
		uint8_t data = read_data();
		if (twi_state == STARTING) {
			if ((data & (USI_TWI_ADDRESS_MASK << 1)) != ((USI_TWI_ADDRESS & USI_TWI_ADDRESS_MASK) << 1)) {
				twi_state = IDLE;
				disable_overflow_int();
				return;
			}
			// Send ack bit, get new overflow when done.
			Gpio::write(PIN_DI, false);
			write_counter(0xe);
			// Set state; lie about direction.
			if (data & 1) {
				// Check that we have data to send.
				twi_state = RECV_ACK;	// Master read, they send acks.
			}
			else
				twi_state = SEND_ACK;	// Master write, we send acks.
			// Ready to continue.
		}
		else if (twi_state == SEND_ACK) {
			// Ack has been sent, ready to receive next byte.
			twi_state = READING;
			Gpio::input(PIN_DI, false);
		}
		else if (twi_state == RECV_ACK) {
			// Received ACK (or NACK, but that shouldn't happen and isn't handled).
			// Send next byte.
			if (tx_packets_available() > 0 && tx_packet_length() > 0) {
				write_data(tx_read());
				tx_partial_pop(1);
			}
			twi_state = WRITING;
			Gpio::write(PIN_DI, false);
		}
		else if (twi_state == READING) {
			// Read next byte. Store in buffer and send ack or nack.
			rx_write(read_data());
			Gpio::write(PIN_DI, rx_buffer_available() == 0);
			twi_state = SEND_ACK;
			write_counter(0xe);
		}
		else if (twi_state == WRITING) {
			// Written next byte. Wait for ack.
			Gpio::input(PIN_DI, false);
			twi_state = RECV_ACK;
			write_counter(0xe);
		}
		clear_overflow();
	}
#define _AVR_SETUP_USI \
	Usi::enable_twi_slave(); \
	Usi::clear_start(); \
	Usi::clear_overflow(); \
	Usi::enable_start_condition_int();
// }}}
#elif defined(USI_ENABLE_UART_TX)
// {{{
	STREAM_BUFFER_WITH_CBS(tx, USI_TX_SIZE, can_read,)
	static void try_send(} {
		// First make sure no new transmission is started while we set up.
		USIDR = 0xff;

		// If there is no more data in the buffer, stop the clock and return.
		if (tx_buffer_available() < 1) {
			USICR &= ~(_BV(USIWM0));
			return;
		}
		USIDR = ((tx_read() & 0x3f) << 1) ^ 0x7f;
		TCNT0 = 0;	// Restart bit counter.
		USISR = 0;	// Restart overflow counter.
		USICR |= _BV(USIWM0);	// Set 3-wire mode.
		// All done; remove byte from buffer.
		tx_pop();
	}
	static void can_read(uint8_t data, uint8_t len) {
		// Data is available in the buffer.

		// If a byte is being sent, the next will follow automatically; nothing to do here.
		if (USICR & _BV(USIWM0))
			return;

		// Attempt to send another byte.
		try_send();
	}
	ISR(USI_OVF_vect) {
		// Counter overflow on USI timer: data has been sent.
		// Try sending another byte.
		try_send();
	}
// }}}
#endif
}

#ifdef AVR_TEST_USI // {{{

#if AVR_TEST_INDEX == 0
namespace Usi {
	static const uint8_t testcode = 'Y';
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
		// TODO: Implement Usi tests.
		return false;
	}
}

#else 

namespace Usi {
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
