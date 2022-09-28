// EEPROM read and write support.

// Options:
// EEPROM_BUFFER_SIZE

#ifndef _AVR_EEPROM_HH
#define _AVR_EEPROM_HH

/** @file
# EEPROM
Example of using the packet buffer to modify EEPROM
```
#define EEPROM_BUFFER_SIZE 32

#include <avr-ll.hh>

void setup() {
	uint8_t base = 5;

	// Write address to packet first.
	Eeprom::buffer_address(base);

	// Write data, which is modified data read from EEPROM.
	for (uint8_t i = 0; i < 10; ++i)
		Eeprom::buffer_write(Eeprom::read(base + i) ^ 0x40);

	// Finalize the packet, triggering the write operation.
	Eeprom::buffer_end();
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

/// @cond
#ifndef EE_READY_vect
#define EE_READY_vect EEPROM_READY_vect
#endif

#ifdef EEARH
#define EEPROM_ADDR_TYPE uint16_t
#else
#define EEPROM_ADDR_TYPE uint8_t
#endif
/// @endcond

/// EEPROM non volatile data access
namespace Eeprom {

#ifdef DOXYGEN
/// Type for the EEPROM address: uint16_t or uint8_t, depending on how much EEPROM there is.
#define EEPROM_ADDR_TYPE

/// Define this to generate a packet buffer for writing to EEPROM.
/**
 * Each packet must consist of 2 bytes start address (regardless of
 * EEPROM_ADDR_TYPE), followed by any number of bytes to write.
 *
 * When a packet is finalized, it is written to EEPROM immediately.
 * More packets can be queued while the packet is being written.
 */
#define EEPROM_BUFFER_SIZE
#endif

	/// Write a byte of data into EEPROM.
	/**
	 * This function busy waits for the EEPROM to be ready.
	 *
	 * To avoid a delay, it should be called from the interrupt.
	 */
	static inline void write(EEPROM_ADDR_TYPE addr, uint8_t data) { // {{{
		uint8_t sreg = SREG;
		cli();
		while (EECR & _BV(EEPE)) {}
#ifdef SELFPRGEN
		while (SPMCSR & _BV(SELFPRGEN)) {}
#endif
#ifdef EEARH
		EEARH = addr >> 8;
		EEARL = addr & 0xff;
#else
		EEAR = addr;
#endif
		EEDR = data;
		uint8_t eecr0 = EECR & _BV(EERIE); // Clear EEPM[01], EEMPE, EEPE, EERE
		asm volatile (
			"out %[eecr], %[eempe]\n"
			"out %[eecr], %[eepe]"
			:: \
				[eecr] "" (_SFR_IO_ADDR(EECR)),
				[eempe] "r" (eecr0 | _BV(EEMPE)),
				[eepe] "r" (eecr0 | _BV(EEMPE) | _BV(EEPE))
		);
		SREG = sreg;
	} // }}}

	/// Enable the interrupt when writing is done.
	/**
	 * ISR(EE_READY_vect) must be defined when using this.
	 */
	static inline void enable_int() { EECR |= _BV(EERIE); }

	/// Disable the interrupt that can trigger when writing is done.
	static inline void disable_int() { EECR &= ~_BV(EERIE); }

	/// Read a byte from the EEPROM.
	/**
	 * This function busy waits for the EEPROM to be ready. There can only
	 * be a delay if there was a recent write operation to EEPROM or flash.
	 *
	 * To avoid the busy wait, this function can be called from the
	 * interrupt handler.
	 */
	static inline uint8_t read(EEPROM_ADDR_TYPE addr) { // {{{
		uint8_t sreg = SREG;
		cli();
		while (EECR & _BV(EEPE)) {}
#ifdef SELFPRGEN
		while (SPMCSR & _BV(SELFPRGEN)) {}
#endif
#ifdef EEARH
		EEARH = addr >> 8;
		EEARL = addr & 0xff;
#else
		EEAR = addr;
#endif
		EECR |= _BV(EERE);
		uint8_t data = EEDR;
		SREG = sreg;
		return data;
	} // }}}

#ifdef EEPROM_BUFFER_SIZE
	/// @cond
	static bool writing = false;
	static EEPROM_ADDR_TYPE next_byte;
	static inline void buffer_address(EEPROM_ADDR_TYPE addr) {
		if (buffer_packet_length() > 0)
			buffer_end();
		buffer_write(addr & 0xff);
		buffer_write((addr >> 8) & 0xff);
	}
	static inline void new_packet() {
		while (!writing && buffer_packets_available() > 0) {
			if (buffer_packet_length() <= 2) {
				buffer_pop();
				continue;
			}
			next_byte = buffer_read(0) | (buffer_read(1) << 8);
			buffer_partial_pop(2);
			writing = true;
			enable_int();
		}
	}
	ISR(EE_READY_vect) {
		write(next_byte++, buffer_read(0));
		buffer_partial_pop(1);
		if (buffer_packet_length() > 0)
			return;
		buffer_pop();
		writing = false;
		disable_int();
		new_packet();
	}
	/// @endcond

#ifndef EEPROM_BUFFER_PACKETS
/// If EEPROM_BUFFER_SIZE is defined, this macro can be defined to set the number of packets in the queue.
#define EEPROM_BUFFER_PACKETS 6
#endif
	PACKET_BUFFER_WITH_CBS(buffer, EEPROM_BUFFER_SIZE, EEPROM_BUFFER_PACKETS,new_packet,)
#endif

}

#ifdef AVR_TEST_EEPROM // {{{

#if AVR_TEST == 0

namespace Eeprom {
	static const uint8_t testcode = 'E';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('r');
			Test::tx('w');
			Test::tx('!');
			Test::tx('.');
			Test::tx('\n');
			return true;
		}
		// Commands:
		//	rXXXX	read address XXXX
		//	wXXXXYY	write YY to address XXXX
		//	!	enable interrupt
		//	.	disable interrupt
		switch (cmd) {
		case 'r':
		{
			if (len != 4)
				return false;
			bool ok = true;
			uint8_t h = Test::read_byte(0, ok);
			uint8_t l = Test::read_byte(2, ok);
			if (!ok)
				return false;
#ifdef EEARH
			EEPROM_ADDR_TYPE addr = (h << 8) | l;
#else
			if (h > 0)
				return false;
			EEPROM_ADDR_TYPE addr = l;
#endif
			uint8_t d = read(addr);
			Test::tx(testcode);
			Test::send_byte(d);
			Test::tx('\n');
			break;
		}
		case 'w':
		{
			if (len != 6)
				return false;
			bool ok = true;
			uint8_t h = Test::read_byte(0, ok);
			uint8_t l = Test::read_byte(2, ok);
			uint8_t d = Test::read_byte(4, ok);
			if (!ok)
				return false;
#ifdef EEARH
			EEPROM_ADDR_TYPE addr = (h << 8) | l;
#else
			if (h > 0)
				return false;
			EEPROM_ADDR_TYPE addr = l;
#endif
			write(addr, d);
			break;
		}
		case '!':
			if (len != 0)
				return false;
			enable_int();
			break;
		case '.':
			if (len != 0)
				return false;
			disable_int();
			break;
		default:
			return false;
		}
		return true;
	}
}

ISR(EE_READY_vect) {
	Test::tx(Eeprom::testcode);
	Test::tx('\n');
}

#else

namespace Eeprom {
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
