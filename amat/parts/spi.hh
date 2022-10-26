// Serial Peripheral Interface

// This file must always be included after gpio.h.

// Options: TODO: handle test option settings.
// SPI_ENABLE_MASTER
// SPI_ENABLE_SLAVE
// SPI_ENABLE_BOTH
// SPI_TX_SIZE
// SPI_RX_SIZE
// CALL_spi_send_done

/** @file
# Serial Peripheral Interface
Example sending a message over Spi:
```
#define SPI_TX_SIZE 32

#include <amat.hh>

void setup() {
	char const *msg = "Hello, world!";
	for (char const *p = msg; *p; ++p)
		Spi::tx_write(*p);
	Spi::tx_end();
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

#ifndef _AVR_SPI_HH
#define _AVR_SPI_HH

#ifdef DOXYGEN

/// Define this to use Spi in master mode. @ingroup usemacros
#define SPI_ENABLE_MASTER

/// Define this to use Spi in slave mode. @ingroup usemacros
#define SPI_ENABLE_SLAVE

/// Define this instead of SPI_ENABLE_MASTER and SPI_ENABLE_SLAVE if you want both. @ingroup usemacros
#define SPI_ENABLE_BOTH

/// Size of write packet buffer.
/** @ingroup usemacros
 * If this is defined, a PACKET_BUFFER is defined for data to be sent over
 * Spi. SPI_TX_PACKETS can be defined to set the number of packets
 * that the buffer can hold.
 */
#define SPI_TX_SIZE

/// Size of read packet buffer.
/** @ingroup usemacros
 * If this is defined, a PACKET_BUFFER is defined for data to be received
 * over Spi. SPI_RX_PACKETS can be defined to set the number of
 * packets that the buffer can hold.
 */
#define SPI_RX_SIZE

/// Define this and spi_send_done() to be notified when the last Spi packet is sent. @ingroup usemacros
#define CALL_spi_send_done

#endif

#ifdef AVR_TEST_SPI // {{{
#define SPI_ENABLE_BOTH
#define SPI_TX_SIZE 10
#define SPI_RX_SIZE 10
#define CALL_spi_send_done
#endif // }}}

/*
Notes: {{{
- SPI as implemented in hardware is not usable for a write buffer in the slave.
- Therefore a read buffer in the master is also not needed.
- There are 2 types of transaction:
	- single byte master command (or dummy data) for reading slave reply; user code must handle SS pin deactivation.
		Slave needs to have enough time to prepare data for master.
		More likely, this is an interrupt-like system, where the master periodically polls to see if the slave sends a "LAM"-signal.
	- multi byte master command, while receiving dummy slave data

- interrupt reasons:
	- for master+slave:
		spi_master_lost();
	- for master:
		for single byte master command: spi_master_done(uint8_t slave_data);
		for multi-byte master command: spi_master_done(uint8_t dummy);
	- for slave:
		After every received received: spi_slave_done(uint8_t byte);
		The data is also stored in the buffer if it is defined.

SPI Use Cases:
- As Master: In control of clock, so no issues with timing.
	Prepare buffer and send it (while receiving slave data).
	Read 1 byte slave data while not sending.
	or (without send buffer)
	Send single byte, receive callback with slave byte when done.

- As Slave: Not in control of clock; sending is time-critical.
	Receive master data in buffer without sending anything.
	Reply single byte to master (while receiving master data).
	or (without read buffer)
	Prepare single byte (or not), receive callback when master data was received.

- As Master/Slave:
	Both of the above sections apply depending on setup.
	Also
	As Master, receive callback when other master takes over.

Interface: (outdated) {{{

Always defined:
	static inline void enable(uint8_t freq_divider_log2 = 0, bool be = false, bool sample_last = false, bool low_idle = false);
		Enable the SPI system. This must always be called first.
	static inline void disable();
		Disable the SPI system. After this, the pins can be used for other functions again.

	static inline void setup_slave_pins();
		Setup pins for slave SPI operation.

When writing, you must #define SPI_TX_SIZE to a positive number between 2 and 127.
Then the following functions are defined:
	static inline void setup_master_pins(bool low_idle = true);
		Setup pins for master SPI operation.

	static inline bool check_master();
		Returns true if we are still master.

	static inline uint8_t send_buffer_available();
		Returns the number of bytes that can be written to the buffer.

	static inline void write1(uint8_t data);
		Store 1 byte in the write buffer and send it when the line is ready.
		Note that you should check that there is room in the buffer before calling this function.

	static inline void write(uint8_t *buffer, uint8_t num);
		Store num bytes in the write buffer and send them when the line is ready.
		Note that you should check that there is room in the buffer before calling this function.
User code needs to define the functions:
	static void spi_lost_master();
		Called when we were set up to be master, but someone else on the line took over as master.
		If this cannot happen on the hardware, this function can be #define'd to "do {} while (false)".

	static void spi_write_done();
		Called when the buffer is empty after a completed write operation.

When reading, you must #define SPI_RX_SIZE to a positive number between 2 and 127. Then the following functions are defined:
	static inline uint8_t receive_buffer_length();
		Returns the number of bytes that can be read from the buffer.

	static inline void receive_buffer_pop(uint8_t num = 1);
		Discard num bytes from the front of the read buffer.

	static inline uint8_t read1(uint8_t pos = 0);
		Read one byte from the front of the read buffer (plus pos).

	static inline void read(uint8_t *buffer, uint8_t num);
		Read num bytes from the front of the read buffer and pop them from the buffer.

User code needs to define the function:
	static void spi_read_done();
		Called when a byte is received.

Protocol:
	Slave:
		Optional:
			write data
			check WCOL
			if set, store data for after transfer
		wait (through interrupt) for completed transfer
		In interrupt:
			write pending data, if available
			read and handle received data
	Master:
		while there is no data to send, be idle.
		Optional: interrupt to inform of losing master status.
		reset SS
		write data to start transmission
		wait for interrupt to signal end of transmission
		pullup SS

	There is a read and a write buffer.

	If neither buffer length is defined, this file produces no code (other than PRR code).
	}}}
	}}}
*/

#ifdef PRR0
namespace Spi {

	/// Disable power to the Spi system.
	static inline void off() { PRR0 |= _BV(PRSPI); }

	/// Enable power to the Spi system.
	static inline void on() { PRR0 &= ~_BV(PRSPI); }

}
#endif

// Make defines more usable. {{{
#ifdef SPI_ENABLE_BOTH
#define SPI_ENABLE_MASTER
#define SPI_ENABLE_SLAVE
#endif

#if defined(SPI_ENABLE_MASTER) || defined(SPI_ENABLE_SLAVE)

#if defined(SPI_ENABLE_MASTER) && defined(SPI_ENABLE_SLAVE) && !defined(SPI_ENABLE_BOTH)
#define SPI_ENABLE_BOTH
#endif

#if !defined(SPI_ENABLE_MASTER) && defined(SPI_TX_SIZE)
#warning "Defining SPI_TX_SIZE but not SPI_ENABLE_MASTER makes no sense; undefining it."
#undef SPI_TX_SIZE
#endif

#if !defined(SPI_ENABLE_SLAVE) && defined(SPI_RX_SIZE)
#warning "Defining SPI_RX_SIZE but not SPI_ENABLE_SLAVE makes no sense; undefining it."
#undef SPI_RX_SIZE
#endif
// }}}

#ifdef SPI_ENABLE_BOTH
/// When SPI_ENABLE_BOTH is defined, this function also needs to be defined. It is called when the device loses Spi master status.
static void spi_lost_master();
#endif

#ifdef SPI_RX_SIZE
/// When defining SPI_RX_SIZE, also define this function to be notified when a packet is received.
static void spi_received();
#endif

#ifdef CALL_spi_send_done
/// If you define CALL_spi_send_done and this function, it will be called when the last packet was completely sent.
static void spi_send_done();
#endif

/// Serial peripheral interface
namespace Spi {
#ifdef SPI_ENABLE_BOTH
/// @cond
	static volatile bool be_master;
/// @endcond

	/// Return true if we are (still) master.
	static inline bool check_master() { // {{{
		// Should be used by interrupt handler in master.
		return SPCR & _BV(MSTR);
	} // }}}
#endif

	/// Enable Spi as master and set up pins
	/**
	 * Normally called from enable(), but can also be called by user code when switching roles.
	 */
	static inline void setup_master_pins(bool low_idle = true) { // {{{
		Gpio::write(MOSI_PIN, false);
		Gpio::write(SCK_PIN, !low_idle);
#ifdef SPI_ENABLE_BOTH
		Gpio::input(SS_PIN, true);
		be_master = true;
#else
		Gpio::write(SS_PIN, true);
#endif
		SPCR |= _BV(MSTR);
	} // }}}

	/// Enable Spi as slave and set up pins
	/**
	 * Normally called from enable(), but can also be called by user code when switching roles.
	 */
	static inline void setup_slave_pins() { // {{{
		Gpio::write(MISO_PIN, false);
#ifdef SPI_ENABLE_BOTH
		be_master = false;
#endif
		SPCR &= ~_BV(MSTR);
	} // }}}

	/// Enable Spi and call setup_master_pins() or setup_slave_pins()
	static inline void enable(uint8_t freq_divider_log2 = 1, bool msb_first = false, bool sample_last = false, bool low_idle = false) { // {{{
		// divider is 1 (f_osc / 4) by default, which is the highest speed that slave can handle if it's on the same clock frequency.
		SPCR = _BV(SPIE) |
			_BV(SPE) |
			(msb_first ? 0 : _BV(DORD)) |
#ifndef SPI_ENABLE_SLAVE
			// Only start out as master if slave is not available.
			_BV(MSTR) |
#endif
			(low_idle ? 0 : _BV(CPOL)) |
			(sample_last ? _BV(CPHA) : 0) |
			((freq_divider_log2 >> 1) << SPR0);
		SPSR = (~freq_divider_log2 & 1) << SPI2X;
#ifndef SPI_ENABLE_SLAVE
		setup_master_pins(low_idle);
#else
		setup_slave_pins();
#endif
	} // }}}

	/// Disable Spi
	static inline void disable() { SPCR = 0; }

#ifdef SPI_ENABLE_MASTER // {{{

#ifdef SPI_TX_SIZE
	// Functions that are only valid if the write buffer is available.

#ifndef SPI_TX_PACKETS
/// If SPI_TX_SIZE is defined, this can be defined to specify the maximum number of packets it holds. @ingroup usemacros
#define SPI_TX_PACKETS 6
#endif

/// @cond
	static inline void _new_packet_sent();
	PACKET_BUFFER_WITH_CBS(send_buffer, SPI_TX_SIZE, SPI_TX_PACKETS,_new_packet_sent();,)

	static inline void _new_packet_sent() { // {{{
		if ((send_buffer_first_packet + 1) % send_buffer_buffer_allocated_size() != send_buffer_last_packet) {
			// This was not the first packet, so a sent operation is in progress.
			// This packet will be sent after all previous packets have been sent.
			return;
		}
		// No send in progress, this means that the new packet is the only packet. Start sending it.
		uint8_t len = send_buffer_packet_length();
		if (len == 0) {
			// New packet is empty. Pop it and return.
			send_buffer_pop();
			return;
		}
		SPDR = send_buffer_read(0);
		send_buffer_partial_pop(1);
	} // }}}
/// @endcond
#endif

#if !defined(SPI_TX_SIZE) || defined(DOXYGEN)
	/// Send a single byte. (Only available when SPI_TX_SIZE is not defined.)
	/**
	 * This function is only available if SPI_TX_SIZE is NOT defined.
	 * 
	 * This will fail if a byte is currently being sent.
	 *
	 * Returns true if a byte was sent.
	 *
	 * This function does not change the state of the SS pin.
	 */
	static inline bool send(uint8_t data) { // {{{
		SPSR;	// This clears WCOL if it was set.
		SPDR = data;
		return !(SPSR & _BV(WCOL));
	} // }}}
#endif

#endif // }}}

#ifdef SPI_RX_SIZE

#ifndef SPI_RX_PACKETS
/// If SPI_RX_SIZE is defined, this can be defined to specify the maximum number of packets it holds. @ingroup usemacros
#define SPI_RX_PACKETS 6
#endif

	PACKET_BUFFER_WITH_CBS(receive_buffer, SPI_RX_SIZE, SPI_RX_PACKETS, spi_received();,)
#endif

/// @cond
#ifdef SPI_ENABLE_BOTH
	static inline bool handle_check_master() {
		if (!Spi::check_master()) {
			// Lost master status.
			Spi::be_master = false;
			Spi::setup_slave_pins();
			spi_lost_master();
			return false;
		}
		return true;
	}
#endif
/// @endcond
}

/// @cond
ISR(SPI_STC_vect) { // {{{
#ifdef SPI_ENABLE_BOTH
	if (Spi::be_master) {
		if (!Spi::handle_check_master())
			return;
	}
#endif

#ifdef SPI_RX_SIZE
	Spi::receive_buffer_write(SPDR);
#endif

#if defined(SPI_ENABLE_MASTER) && defined(SPI_TX_SIZE)
	if (Spi::send_buffer_packet_length() == 0) {
		// Done with this packet.
#ifdef SPI_ENABLE_BOTH
		Gpio::input(SS_PIN, true);
#else
		Gpio::write(SS_PIN, true);
#endif
		Spi::send_buffer_pop();
#ifdef SPI_RX_SIZE
		Spi::receive_buffer_end();
#endif
		if (Spi::send_buffer_first_packet == Spi::send_buffer_last_packet) {
			// Done with packets.
#ifdef CALL_spi_send_done
			spi_send_done();
#endif
			return;
		}
	}

#ifdef SPI_ENABLE_BOTH
	if (Spi::be_master) {
		if (!Spi::handle_check_master())
			return;
	}
#endif

	// Send next byte.
	Gpio::write(SS_PIN, false);
	SPDR = Spi::send_buffer_read(0);
	Spi::send_buffer_partial_pop(1);
#else
	// No send buffer; just notify user code.
#ifdef CALL_spi_send_done
	spi_send_done();
#endif
#endif
} // }}}
/// @endcond

#endif	// SPI_ENABLE_MASTER or SPI_ENABLE_SLAVE is defined.

#ifdef AVR_TEST_SPI // {{{

#if AVR_TEST_INDEX == 0
namespace Spi {
	static const uint8_t testcode = 'S';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('e');
			Test::tx('d');
			Test::tx('=');
			Test::tx('m');
			Test::tx('s');
			Test::tx('r');
			Test::tx('w');
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	eDMSL	enable	(divider, msb_first, sample_last, low_idle)
		//	d	disable
		//	=	check master
		//	mL	setup master (low idle)
		//	s	setup slave
		//	r...	read commands
		//	w...	write commands
		switch (cmd) {
		case 'e': // enable
		{
			if (len != 4) 
				return false;
			uint8_t div = Test::read_digit(0);
			uint8_t msb_first = Test::read_digit(1);
			uint8_t sample_last = Test::read_digit(2);
			uint8_t low_idle = Test::read_digit(3);
			if (div >= 8 || msb_first >= 2 || sample_last >= 2 || low_idle >= 2)
				return false;
			enable(div, msb_first, sample_last, low_idle);
			break;
		}
		case 'd': // disable
			if (len != 0)
				return false;
			disable();
			break;
		case '=': // check
		{
			if (len != 0)
				return false;
			bool m = check_master();
			Test::tx(testcode);
			Test::tx(m ? '1' : '0');
			Test::tx('\n');
			break;
		}
		case 'm': // master
		{
			if (len != 1)
				return false;
			uint8_t low_idle = Test::read_digit(0);
			setup_master_pins(low_idle);
			break;
		}
		case 's':
			if (len != 0)
				return false;
			setup_slave_pins();
			break;
		case 'r': // TODO
		case 'w': // TODO
		default:
			return false;
		}
		return true;
	}
}

static void spi_lost_master() {
	Spi::setup_slave_pins();
	Spi::send_buffer_reset();
	Spi::receive_buffer_reset();
	Test::tx(Spi::testcode);
	Test::tx('L');
	Test::tx('\n');
}

static void spi_received() {
	Test::tx(Spi::testcode);
	Test::tx('r');
	for (uint8_t i = 0; i < Spi::receive_buffer_packet_length(); ++i)
		Test::tx(Spi::receive_buffer_read(i));
	Test::tx('\n');
	Spi::receive_buffer_pop();
}

static void spi_send_done() {
	Test::tx(Spi::testcode);
	Test::tx('w');
	Test::tx('\n');
}

#else 

namespace Spi {
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

#endif	// Include guard.

// vim: set foldmethod=marker :
