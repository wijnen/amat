// Two wire interface

// Options:
// CALL_twi_master_done
// CALL_twi_failed
// CALL_twi_partial_master
// CALL_twi_partial_slave
// TWI_MASTER_TX_SIZE
// TWI_ADDRESS

#ifndef _AVR_TWI_HH
#define _AVR_TWI_HH

/**
 * @file
 * Two wire interface

 * @author Bas Wijnen <wijnen@debian.org>
 */

#ifdef DOXYGEN

/// Address for Twi interface.
/**
 * The address is in the upper 7 bit.
 *
 * Bit 0 is set if the general call should be answered.
 *
 * Address 0000 000 is reserved for the "general call" (broadcast address).
 *
 * No calls are answered if this is not set.
 *
 * Setting this to 1 means only the general call is answered.
 *
 * Adresses 1111 xxx are reserved.
 */
#define TWI_ADDRESS

#endif

#ifdef AVR_TEST_TWI
#endif

// Always define power reduction functions.
#ifdef PRR0
/// Two wire interface
namespace Twi {
	/// Disable power to Twi
	static inline void off() { PRR0 |= _BV(PRTWI); }
	/// Enable power to Twi
	static inline void on() { PRR0 &= ~_BV(PRTWI); }
}
#endif

#ifdef CALL_twi_master_done
/// User code needs to define this. It is called when a master transaction is completed by Twi.
/**
 * This is only called if CALL_twi_master_done is defined.
 *
 * The reply is stored in the Twi::tx_reply stream buffer.
 */
static void twi_master_done();
#endif

#ifdef TWI_ADDRESS
/// User code needs to define this if TWI_ADDRESS is defined. It is called when a packet is received for this device.
/**
 * This function must store the reply in the Twi::rx_reply stream buffer.
 */
static void twi_rx();
#endif

#ifdef CALL_twi_failed
/// User code needs to defined this if CALL_twi_failed is defined. It is called when a NACK is received on a SLA+W request.
static void twi_failed();
#endif

#ifdef CALL_twi_partial_master
/// User code needs to defined this if CALL_twi_partial_master is defined. It is called when a NACK is received before a master transmission is complete.
static bool twi_partial_master(uint8_t len);
#endif

#ifdef CALL_twi_partial_slave
/// User code needs to defined this if CALL_twi_partial_slave is defined. It is called when a NACK is received before a slave transmission is complete.
static void twi_partial_slave(uint8_t len);
#endif

#ifndef TWI_BAUD
/// Set the Twi baud rate for master operations
#define TWI_BAUD 16384
#endif

#ifndef TWI_ADDRESS_MASK
/// Address mask for Twi interface.
/**
 * Setting any bits in this mask to 1 will make the device ignore them in the
 * address, thus responding to more than one address.
 */
#define TWI_ADDRESS_MASK 0
#endif

#ifdef DOXYGEN
#ifndef TWI_MASTER_TX_SIZE
/// Set Twi transmit buffer size.
#define TWI_MASTER_TX_SIZE 32
#endif
#endif

#ifndef TWI_MASTER_TX_PACKETS
/// Set number of packets in Twi transmit buffer.
#define TWI_MASTER_TX_PACKETS 6
#endif

#ifndef TWI_MASTER_RX_SIZE
/// Set Twi receive buffer size.
#define TWI_MASTER_RX_SIZE 32
#endif

#ifndef TWI_MASTER_RX_PACKETS
/// Set number of packets in Twi receive buffer.
#define TWI_MASTER_RX_PACKETS 6
#endif

#ifndef TWI_REPLY_SIZE
/// Set Twi slave transmit buffer size.
#define TWI_REPLY_SIZE 32
#endif

#ifndef TWI_RX_SIZE
/// Set Twi slave receive buffer size.
#define TWI_RX_SIZE 32
#endif

#ifndef TWI_RX_PACKETS
/// Set number of packets in Twi slave receive buffer.
#define TWI_RX_PACKETS 6
#endif

namespace Twi {
/// @cond
#ifdef TWI_MASTER_TX_SIZE
	// Variables. {{{
	volatile uint8_t target;
	volatile uint8_t len;
	volatile uint8_t current;
	volatile bool busy;
	// }}}

	static inline void try_transmit(bool check_busy = true);
	PACKET_BUFFER_WITH_CBS(tx, TWI_MASTER_TX_SIZE, TWI_MASTER_TX_PACKETS, try_transmit();,)
	STREAM_BUFFER(tx_reply, TWI_MASTER_RX_SIZE)

#endif

#ifdef TWI_ADDRESS
	STREAM_BUFFER(rx, TWI_SLAVE_RX_SIZE)
	STREAM_BUFFER(rx_reply, TWI_SLAVE_TX_SIZE)
#endif
/// @endcond

	/// Enable Twi.
	static inline void enable() { // {{{
		// SCL = F_CPU / (16 + 2 * TWBR * prescale)
		// TWBR * prescale = F_CPU / (2 * SCL) - 8
		uint16_t target_baud = F_CPU / (2L * TWI_BAUD) - 8;
		uint8_t shift;
		for (shift = 0; shift < 3; ++shift) {
			if (target_baud < uint16_t(256) << (2 * shift))
				break;
		}
		TWSR = shift;
		if (target_baud >= uint16_t(256) << (2 * shift))
			TWBR = 0xff;
		else
			TWBR = target_baud >> (2 * shift);
#ifdef TWI_ADDRESS
		TWAR = TWI_ADDRESS;
		TWAMR = TWI_ADDRESS_MASK;
#else
		TWAR = 0;
#endif
#ifdef TWI_MASTER_TX_SIZE
		busy = false;
#endif
		// Clear interrupt, enable acknowledge, do not send start or stop, enable Twi, Enable Twi interrupt.
		TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);
	} // }}}

	/// Disable Twi.
	static inline void disable() { // {{{
		TWCR = 0;
	} // }}}
	// Master commands.

	/// @cond
#ifdef TWI_MASTER_TX_SIZE
	static inline void try_transmit(bool check_busy) { // {{{
		// A new packet is available in the master transmit buffer.
		if (check_busy) {
			if (busy)
				return;
			busy = true;
		}
		target = tx_read(0);
		len = tx_read(1);
		TWCR = 0xe5;	// Send start.
	} // }}}
#endif
	/// @endcond
}

/// @cond
// Interrupt handling.
#if defined(TWI_MASTER_TX_SIZE) || defined(TWI_ADDRESS)
ISR(TWI_vect) { // {{{
	switch(TWSR & 0xf8) {
		case 0x00:	// Bus error. {{{
			TWCR = 0xd5;	// Send fake stop to clear error condition; no stop is sent on the bus.
			break;
		// }}}

#ifdef TWI_MASTER_TX_SIZE
			// Master

		// 0x08, 0x10: (Repeated) start has been transmitted. {{{
		case 0x08:	// Start has been transmitted.
		case 0x10:	// Repeated start has been transmitted.
			TWDR = Twi::target;
			TWCR = 0xc5; // Clear start condition.
			Twi::current = 2;
			Twi::tx_reply_reset();
			break;
		// }}}

		case 0x30:	// data has been transmitted, NACK has been received. {{{
			// Slave cannot receive more data.
			if (Twi::current < Twi::tx_packet_length()) {
				// Message was incomplete.
#ifdef CALL_twi_partial_master
				if (!twi_partial_master(Twi::current)) {
					// Abort this session, possibly start a new one.
					// If twi_partial() did not pop() the current packet, it will be retried.
					if (Twi::tx_packets_available() > 0)
						Twi::try_transmit(false);
					else {
						Twi::busy = false;
						TWCR = 0xd5;	// Send stop.
					}
					break;
				}
#endif
				// Partial message is treated as complete message.
				Twi::current = Twi::tx_packet_length();
			}
		// }}}
			// Fall through.
		// 0x18, 0x28: SLA+W or data has been transmitted, ACK has been received. {{{
		case 0x18:	// SLA+W has been transmitted, ACK has been received.
		case 0x28:	// data has been transmitted, ACK has been received.
			if (Twi::current == Twi::tx_packet_length()) {
				// Done sending message.
				Twi::tx_pop();
				if (Twi::len > 0) {
					TWCR = 0xe5;	// Send repeated start for receive.
					Twi::target |= 1;
				}
				else if (Twi::tx_packets_available() > 0)
					Twi::try_transmit(false);
				else {
					Twi::busy = false;
					TWCR = 0xd5;	// Send stop.
				}
				break;
			}
			// More data to send.
			TWDR = Twi::tx_read(Twi::current++);
			TWCR = 0xc5;	// Clear interrupt.
			break;
		// }}}

		case 0x20:	// SLA+W has been transmitted, NACK has been received. {{{
			// Failed to reach slave.
			TWCR = 0xd5;	// Send stop.
			Twi::busy = false;
#ifdef CALL_twi_failed
			twi_failed();
#endif
			break;
		// }}}

		case 0x38:	// arbitration lost during transmission of SLA+W, SLA+R, NACK, or data. {{{
			// Retry sending start. This will happen at some point when the line is available again.
			TWCR = 0xe5;
			break;
		// }}}

		case 0x40:	// SLA+R has been transmitted, ACK has been received. {{{
			if (Twi::len == 1)
				TWCR = 0x85;
			else
				TWCR = 0xc5;
			break;
		// }}}

		case 0x48:	// SLA+R has been transmitted, NACK has been received. {{{
			// Failed to reach slave.
			// Respond as success with empty buffer.
			Twi::busy = false;
#ifdef CALL_twi_master_done
			twi_master_done();	// Done receiving reply.
#endif
			if (!Twi::busy) {
				if (Twi::tx_packets_available() > 0)
					Twi::try_transmit(false);
				else
					TWCR = 0xd5;	// Send stop.
			}
			break;
		// }}}

		case 0x50:	// data has been received, ACK has been sent. {{{
			Twi::tx_reply_write(TWDR);
			if (Twi::tx_reply_buffer_used() + 1 == Twi::len)
				TWCR = 0x85;
			else
				TWCR = 0xc5;
			break;
		// }}}

		case 0x58:	// data has been received, NACK has been sent. {{{
			Twi::tx_reply_write(TWDR);
			Twi::busy = false;
#ifdef CALL_twi_master_done
			twi_master_done();	// Done receiving reply.
#endif
			if (!Twi::busy) {
				if (Twi::tx_packets_available() > 0)
					Twi::try_transmit(false);
				else
					TWCR = 0xd5;	// Send stop.
			}
			break;
		// }}}
#endif

#ifdef TWI_ADDRESS
			// Slave receive

		// 0x60, 0x68, 0x70, 0x78: Own SLA+W, general call, or data has been received, ACK has been sent. {{{
		case 0x60:	// SLA+W has been received for client address; ACK has been sent.
		case 0x68:	// Arbitration lost; SLA+W has been received for client address; ACK has been sent.
		case 0x70:	// SLA+W has been received for general address; ACK has been sent.
		case 0x78:	// Arbitration lost; SLA+W has been received for general address; ACK has been sent.
			Twi::rx_reset();
		// }}}
			// Fall through.
		// 0x80, 0x90: Data has been received, ACK has been sent. {{{
		case 0x80:	// While receiving client data, data has been received, ACK has been sent.
		case 0x90:	// While receiving general data, data has been received, ACK has been sent.
			Twi::rx_read(TWDR);
			if (Twi::rx_buffer_available() == 1)
				TWCR = 0x85;
			else
				TWCR = 0xc5;
			break;
		// }}}

		// 0x88, 0x98: Data has been received, NACK has been sent. {{{
		case 0x88:	// While receiving client data, data has been received, NACK has been sent.
		case 0x98:	// While receiving general data, data has been received, NACK has been sent.
			Twi::rx_read(TWDR);
		// }}}
			// Fall through.
		case 0xa0:	// While receiving data, STOP or REPEATED START has been received. {{{
			twi_rx();
			if (Twi::tx_packets_available() > 0)
				TWCR = 0xe5;
			else
				TWCR = 0xc5;
			break;
		// }}}

			// Slave send

		// 0xa8, 0xb0: SLA+R has been received, ACK has been sent. {{{
		case 0xa8:	// SLA+R has been received for client address; ACK has been sent.
		case 0xb0:	// Arbitration lost; SLA+R has been received for client address; ACK has been sent.
		{
			Twi::current = 1;
			uint8_t used = Twi::rx_buffer_used();
			TWDR = (used > 0 ? Twi::rx_read(0) : 0xff);
			uint8_t start = (Twi::tx_packets_available() > 0 ? 0x20 : 0)
			if (used <= 1)
				TWCR = 0x85 | start;
			else
				TWCR = 0xc5 | start;
		} // }}}

		case 0xb8:	// Data has been transmitted in response to SLA+R; ACK has been received. {{{
		{
			TWDR = Twi::rx_read(Twi::current++);
			uint8_t start = (Twi::tx_packets_available() > 0 ? 0x20 : 0)
			if (Twi::current + 1 >= Twi::rx_buffer_used())
				TWCR = 0x85 | start;
			else
				TWCR = 0xc5 | start;
		}
		// }}}

		case 0xc0:	// Data has been transmitted in response to SLA+R; NACK has been received. {{{
		{
			TWDR = Twi::rx_read(Twi::current++);
#ifdef CALL_twi_partial_slave
			if (Twi::current < Twi::rx_buffer_used()) {
				twi_partial_slave(Twi::current);
				// Slave cannot do any recovery, so just accept it.
			}
#endif
			uint8_t start = (Twi::tx_packets_available() > 0 ? 0x20 : 0)
			TWCR = 0xc5 | start;
		} // }}}

		case 0xc8:	// Last data byte has been transmitted in response to SLA+R; ACK has been received. {{{
		{
			uint8_t start = (Twi::tx_packets_available() > 0 ? 0x20 : 0)
			TWCR = 0xc5 | start;
		} // }}}
#endif

		default:
			break;
		/*
		case 0xd0:	// Unused.
		case 0xd8:	// Unused.
		case 0xe0:	// Unused.
		case 0xe8:	// Unused.
		case 0xf0:	// Unused.
		case 0xf8:	// No state information available.
		*/
	}
} // }}}
#endif
/// @endcond

#ifdef AVR_TEST_TWI // {{{

#if AVR_TEST == 0
namespace Twi {
	static const uint8_t testcode = 'T';
}

static void twi_write_done() {
	// A write operation has completed.
	Test::tx(Twi::testcode);
	Test::tx('W');
	Test::tx('\n');
}

static void twi_read_done(uint8_t len, uint8_t const *buffer) {
	// A read operation has completed.
	Test::tx(Twi::testcode);
	Test::tx('R');
	for (uint8_t i = 0; i < len; ++i)
		Test::send_byte(buffer[i]);
	Test::tx('\n');
}

static void twi_failed() {
	// A read or write operation has failed.
	Test::tx(Twi::testcode);
	Test::tx('F');
	Test::tx('\n');
}

static void twi_recv(uint8_t len, uint8_t const *buffer, uint8_t target) {
	// Data is sent by the bus.
	Test::tx(Twi::testcode);
	Test::tx('r');
	Test::send_byte(target);
	for (uint8_t i = 0; i < len; ++i)
		Test::send_byte(buffer[i]);
	Test::tx('\n');
}

static uint8_t twi_reply(uint8_t *buffer, uint8_t target) {
	// Data is requested by the bus. Buffer must be filled and the length returned.
	Test::tx(Twi::testcode);
	Test::tx('s');
	Test::send_byte(target);
	Test::tx('\n');
	buffer[0] = 'R';
	buffer[1] = 'e';
	buffer[2] = 'p';
	buffer[3] = 'l';
	buffer[4] = 'y';
	buffer[5] = '!';
	return 6;
}

uint8_t readbuffer[16];
namespace Twi {
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('e');
			Test::tx('d');
			Test::tx('r');
			Test::tx('w');
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	eAAMMFFFF	enable (addr, mask, freq)
		//	d		disable
		//	rTTLL		read LL bytes from target TT
		//	wTTD		write stock string D to target TT
		switch (cmd) {
		case 'e':
		{
			if (len != 8)
				return false;
			bool ok = true;
			uint8_t addr = Test::read_byte(0, ok);
			uint8_t mask = Test::read_byte(2, ok);
			uint8_t h = Test::read_byte(4, ok);
			uint8_t l = Test::read_byte(6, ok);
			if (!ok)
				return false;
			uint16_t freq = (h << 8) | l;
			enable(addr, mask, freq);
			break;
		}
		case 'd':
			if (len != 0)
				return false;
			disable();
			break;
		case 'r':
		{
			if (len != 4)
				return false;
			bool ok = true;
			uint8_t target = Test::read_byte(0, ok);
			uint8_t l = Test::read_byte(2, ok);
			if (!ok || l > 16)
				return false;
			read(target, l, readbuffer);
			break;
		}
		case 'w':
		{
			if (len != 3)
				return false;
			bool ok = true;
			uint8_t target = Test::read_byte(0, ok);
			uint8_t idx = Test::read_digit(2);
			if (!ok || idx >= _AVR_TEST_NUM_STRINGS)
				return false;
			write(target, Test::stringlengths[idx], reinterpret_cast <uint8_t const *>(Test::strings[idx]));
			break;
		}
		default:
			return false;
		}
		return true;
	}
}

#else 

namespace Twi {
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

#endif	// include guard.

// vim: set foldmethod=marker :
