// Common code for all mcu header files.

// Options:
// DBG_ENABLE
// DBG0_ENABLE
// DBG1_ENABLE
// DBG2_ENABLE
// DBG3_ENABLE

#ifndef _AVR_COMMON_HH
#define _AVR_COMMON_HH

/** @file
# Common code for all mcu models
Example code using both a user defined packet buffer instead of the standard
stream buffer that can be enabled with USART_RX_SIZE.

Every second, if there is at least one packet, one packet is dumped back to the
serial port.
```
// Enable a ms clock using Counter 0.
#define SYSTEM_CLOCK0_ENABLE
// Use the interrupt system.
#define CALL_system_clock_interrupt

#include <avr-ll.hh>

// This packet buffer holds 128 bytes in 14 (or fewer) packets.
PACKET_BUFFER(usart, 128, 14)
// Next event time. Type is uint16_t by default.
SYSTEM_CLOCK_TYPE next;

void setup() {
	// Enable the first usart and its receive interrupt.
	Usart::enable();
	Usart::enable_rxc();
	// Initialize the packet buffer. This is not required; it is reset at boot.
	usart_reset();
	// Schedule first timed event.
	next = Counter::get_time() + 1000;
	system_clock_set_interrupt(next);
}

ISR(USART_RX_vect) {
	// If received byte is a newline, finalize the packet.
	// Else, add it to the packet.
	uint8_t udr = UDR;
	if (udr == '\n') {
		usart_end();
		// Optionally, code to handle the new packet can be placed here.
		// Note that this code will be called with interrupts disabled.
	}
	else
		usart_write(udr);
}

void system_clock_interrupt() {
	// Schedule next event.
	next += 1000;
	Counter::system_clock_set_interrupt(next);

	// If there are no finalized packets, do nothing.
	if (usart_packets_available() == 0)
		return;

	// Dump first packet to serial port.
	for (uint8_t i = 0; i < usart_packet_length(); ++i) {
		dbg_char(':');
		dbg_byte(usart_read(i);
	}
	dbg_char('\n');
	// Remove the packet from the buffer.
	usart_pop();
}
```

@author Bas Wijnen <wijnen@debian.org>
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#ifndef F_CPU
#warning "F_CPU should be defined explicitly. Using default of 8MHz"
/// This should be defined by the Makefile, not by the code.
#define F_CPU 8000000L
#endif

/// @cond
namespace Avr {
	union Word { // {{{
		uint16_t w;
		uint8_t b[sizeof(uint16_t)];
	}; // }}}
	union DWord { // {{{
		uint32_t dw;
		uint8_t b[sizeof(uint32_t)];
	}; // }}}
}
/// @endcond

// Define code for buffers. {{{
// Actual code is unreadable for documentation, so use a duplicate version that
// is normally unused.
#ifdef DOXYGEN // {{{
/**
 * @defgroup buffers Convenience code for ring buffers
 *
 * These buffers are used internally, but user code may also define new
 * buffers.
 *
 * Defining a buffer is done by using the macro. This will define all the
 * functions listed here.
 *
 * The prefix (stream_buffer or packet_buffer) is replaced by the name that was
 * passed to the macro.
 *
 * Note that these buffers don't do bounds checking, so reading past the end of
 * the written bytes, or writing when the buffer is full, gives undefined
 * results.
 * @{
 */

/// @name STREAM_BUFFER
/// @{

/// Define a new byte (ring) buffer.
/**
 * @param name: prefix for the buffer functions.
 * @param size: number of bytes to allocate. Buffer can contain size - 1 bytes.
 *
 * In this documentation, "stream_buffer" is used as name.
 *
 * Note that no bounds checking is done by this code; this must be done by user
 * code if it is required.
 */
#define STREAM_BUFFER(name, size)

/// Buffer data. Should normally not be directly referenced by user code.
static uint8_t stream_buffer_buffer[size];

/// Next byte to read. Should normally not be directly referenced by user code.
static uint8_t stream_buffer_head = 0;

/// Next byte to write. Should normally not be directly referenced by user code.
static uint8_t stream_buffer_tail = 0;

/// Initialize and clear the buffer.
static inline void stream_buffer_reset();

/// Return size that was passed when creating the buffer.
static inline uint8_t stream_buffer_buffer_allocated_size();

/// Return number of valid bytes currently in the buffer.
static inline uint8_t stream_buffer_buffer_used();

/// Return number of bytes that can currently be written to the buffer.
static inline uint8_t stream_buffer_buffer_available();

/// Remove one or more bytes from the front of the buffer.
static inline void stream_buffer_pop(uint8_t num = 1);

/// Read one byte from the buffer. The byte remains in the buffer.
static inline uint8_t stream_buffer_read(uint8_t pos = 0);

/// Copy num bytes to memory, then remove them from the ring buffer.
static inline void stream_buffer_move(uint8_t *buffer, uint8_t num);

/// Write a byte to the buffer. This must only be called when there is space.
static inline bool stream_buffer_write(uint8_t data);

/// @}

/// @name PACKET_BUFFER
/// @{

/// Define a new packet (ring) buffer.
/**
 * @param name: prefix for the buffer functions.
 * @param size: number of bytes to allocate. Buffer can contain size - 1 bytes.
 * @param num_packets: Maximum number of packets in the buffer.
 * It is most efficient if num_packets + 2 is a power of 2.
 *
 * In this documentation, "packet_buffer" is used as name.
 *
 * A packet buffer can hold several packets. When writing new data, it is added
 * to the end of the last packet. That packet can be finalized by calling
 * packet_buffer_end(). Data is always read from the first packet, which is
 * usually finalized when it is read from.
 *
 * packet_buffer_pop() removes the first (reading) packet;
 * packet_buffer_partial_pop() only removes some bytes from it.
 *
 * Note that no bounds checking is done by this code; this must be done by user
 * code if it is required.
 */
#define PACKET_BUFFER(name, size, num_packets)


/// Buffer data. Should normally not be directly referenced by user code.
static uint8_t packet_buffer_buffer[size];

/// Next byte to read. Should normally not be directly referenced by user code.
static uint8_t packet_buffer_head[num_packets + 2];

/// Current packet for reading. Should normally not be directly referenced by user code.
static uint8_t packet_buffer_first_packet;

/// Current packet for writing. Should normally not be directly referenced by user code.
static uint8_t packet_buffer_last_packet;

/// Initialize and clear the buffer.
static inline void packet_buffer_reset();

/// Return size that was passed when creating the buffer.
static inline uint8_t packet_buffer_buffer_allocated_size();

/// Return number of finalized packets.
static inline uint8_t packet_buffer_packets_available();

/// Return number of valid bytes currently in the current packet.
static inline uint8_t packet_buffer_packet_length();

/// Return number of bytes that can currently be written to the buffer.
static inline uint8_t packet_buffer_buffer_available();

/// Read one byte from the buffer. The byte remains in the buffer.
static inline uint8_t packet_buffer_read(uint8_t pos = 0);

/// Write a byte to the buffer. This must only be called when there is space.
static inline bool packet_buffer_write(uint8_t data);

/// Remove one or more bytes from the front of the buffer.
static inline void packet_buffer_partial_pop(uint8_t n);

/// Drop the current reading packet, moving to the next.
static inline void packet_buffer_pop();

/// Finalize current writing packet, starting a new one.
static inline void packet_buffer_end();

/// @}

/// @}
#else // }}}

#define _AVR_NOP(...)
#define STREAM_BUFFER_WITH_CBS(name, size, can_read, can_write) /* {{{ */ \
	static uint8_t name ## _buffer[(size)]; \
	static uint8_t name ## _head = 0; \
	static uint8_t name ## _tail = 0; \
	static inline void name ## _reset() { /* {{{ */ \
		/* Initialize ring buffer. */ \
		name ## _head = 0; \
		name ## _tail = 0; \
	} /* }}} */ \
	static inline uint8_t name ## _buffer_allocated_size() { /* {{{ */ \
		return size; \
	} /* }}} */ \
	static inline uint8_t name ## _buffer_used() { /* {{{ */ \
		return (name ## _tail - name ## _head + (size)) % (size); \
	} /* }}} */ \
	static inline uint8_t name ## _buffer_available() { /* {{{ */ \
		return (size) - name ## _buffer_used() - 1; \
	} /* }}} */ \
	static inline void name ## _pop(uint8_t num = 1) { /* {{{ */ \
		name ## _head = (name ## _head + num) % (size); \
		/* Notify that buffer can be written to. */ \
		can_write \
	} /* }}} */ \
	static inline uint8_t name ## _read(uint8_t pos = 0) { /* {{{ */ \
		return name ## _buffer[(name ## _head + pos) % (size)]; \
	} /* }}} */ \
	static inline void name ## _move(uint8_t *buffer, uint8_t num) { /* {{{ */ \
		for (uint8_t i = 0; i < num; ++i) \
			buffer[i] = name ## _read(i); \
		name ## _pop(num); \
	} /* }}} */ \
	static inline bool name ## _write(uint8_t data) { /* {{{ */ \
		/* This must only be called when there is room in the buffer. */ \
		uint8_t next = (name ## _tail + 1) % (size); \
		name ## _buffer[name ## _tail] = data; \
		name ## _tail = next; \
		/* Notify that buffer contains new data. */ \
		can_read(data, name ## _buffer_used()); \
		return (name ## _tail + 1) % (size) != name ## _head; \
	} /* }}} */ \
	// }}}
// User friendly version:
#define STREAM_BUFFER(name, size) STREAM_BUFFER_WITH_CBS(name, (size), _AVR_NOP,)

/* Packet operations:
   - write byte
   - finish writing; move to next buffer
   - read byte
   - finish reading; move to next buffer
*/
#define PACKET_BUFFER_WITH_CBS(name, size, num_packets, new_packet, free_packet) /* {{{ */ \
	static uint8_t name ## _buffer[(size)]; \
	static uint8_t name ## _head[(num_packets) + 2]; \
	static uint8_t name ## _first_packet; \
	static uint8_t name ## _last_packet; \
	static inline void name ## _reset() { /* {{{ */ \
		/* Initialize ring buffer. */ \
		name ## _head[0] = 0; \
		name ## _head[1] = 0; \
		name ## _first_packet = 0; \
		name ## _last_packet = 0; \
	} /* }}} */ \
	static inline uint8_t name ## _buffer_allocated_size() { /* {{{ */ \
		return (size); \
	} /* }}} */ \
	static inline uint8_t name ## _packets_available() { /* {{{ */ \
		return (name ## _last_packet - name ## _first_packet + (num_packets)) % (num_packets); \
	} /* }}} */ \
	static inline uint8_t name ## _packet_length() { /* {{{ For reading. */ \
		return (name ## _head[(name ## _first_packet + 1) % ((num_packets) + 2)] - name ## _head[name ## _first_packet] + (size)) % (size); \
	} /* }}} */ \
	static inline uint8_t name ## _buffer_available() { /* {{{ For writing */ \
		return (size) - (name ## _head[name ## _first_packet] - name ## _head[(name ## _last_packet + 1) % ((num_packets) + 2)] + (size)) % (size) - 1; \
	} /* }}} */ \
	static inline uint8_t name ## _read(uint8_t pos = 0) { /* {{{ */ \
		return name ## _buffer[(name ## _head[name ## _first_packet] + pos) % (size)]; \
	} /* }}} */ \
	static inline bool name ## _write(uint8_t data) { /* {{{ */ \
		/* This must only be called when there is room in the buffer. */ \
		uint8_t packet = (name ## _last_packet + 1) % ((num_packets) + 2); \
		name ## _buffer[name ## _head[packet]] = data; \
		uint8_t next = (name ## _head[packet] + 1) % (size); \
		name ## _head[packet] = next; \
		return (next + 1) % (size) != name ## _head[name ## _first_packet]; \
	} /* }}} */ \
	static inline void name ## _partial_pop(uint8_t n) { /* {{{ Done some reading. */ \
		name ## _head[name ## _first_packet] = (name ## _head[name ## _first_packet] + n) % ((num_packets) + 2); \
	} /* }}} */ \
	static inline void name ## _pop() { /* {{{ Done reading. */ \
		name ## _first_packet = (name ## _first_packet + 1) % ((num_packets) + 2); \
		free_packet \
	} /* }}} */ \
	static inline void name ## _end() { /* {{{ Done writing. */ \
		uint8_t packet = (name ## _last_packet + 1) % ((num_packets) + 2); \
		name ## _last_packet = packet; \
		name ## _head[(packet + 1) % ((num_packets) + 2)] = name ## _head[packet]; \
		new_packet \
	} /* }}} */ \
	// }}}
// User friendly version:
#define PACKET_BUFFER(name, size, num_packets) PACKET_BUFFER_WITH_CBS(name, (size), num_packets,,)
#endif // Doxygen switch.
// }}}

#ifdef DOXYGEN
/// Enable dbg() macro to send things to serial port.
/**
 * Defining this macro will choose the first serial port (0 or 1, depending on the device).
 *
 * This macro is automatically defined if any of the DBG*_ENABLE macros is defined.
 *
 * No more than 1 DBG*_ENABLE must be defined.
 */
#define DBG_ENABLE

/// Enable dbg() macro to send things to serial port 0.
/**
 * No more than 1 DBG*_ENABLE must be defined.
 */

#define DBG0_ENABLE
/// Enable dbg() macro to send things to serial port 1.
/**
 * No more than 1 DBG*_ENABLE must be defined.
 */
#define DBG1_ENABLE

/// Enable dbg() macro to send things to serial port 2.
/**
 * No more than 1 DBG*_ENABLE must be defined.
 */
#define DBG2_ENABLE

/// Enable dbg() macro to send things to serial port 3.
/**
 * No more than 1 DBG*_ENABLE must be defined.
 */
#define DBG3_ENABLE

#endif
/// @cond
// DBG internals. {{{
#ifdef DBG_ENABLE
#ifdef UDR0
#define DBG0_ENABLE
#else
#define DBG1_ENABLE
#endif
#endif

#ifdef DBG0_ENABLE
#define USART_TX0_SIZE 250
namespace Usart { static bool tx0_write(uint8_t c); static void tx0_block_ready(); }
#define dbg_char(c) do { if (!Usart::tx0_write(c)) Usart::tx0_block_ready(); } while (false)
#define _AVR_SETUP_DBG Usart::enable0();
#ifndef DBG_ENABLE
#define DBG_ENABLE
#endif
#endif

#ifdef DBG1_ENABLE
#define USART_TX1_SIZE 250
namespace Usart { static bool tx1_write(uint8_t c); static void tx1_block_ready(); }
#define dbg_char(c) do { if (!Usart::tx1_write(c)) Usart::tx1_block_ready(); } while (false)
#define _AVR_SETUP_DBG Usart::enable1();
#ifndef DBG_ENABLE
#define DBG_ENABLE
#endif
#endif

#ifdef DBG2_ENABLE
#define USART_TX2_SIZE 250
namespace Usart { static bool tx2_write(uint8_t c); static void tx2_block_ready(); }
#define dbg_char(c) do { if (!Usart::tx2_write(c)) Usart::tx2_block_ready(); } while (false)
#define _AVR_SETUP_DBG Usart::enable2();
#ifndef DBG_ENABLE
#define DBG_ENABLE
#endif
#endif

#ifdef DBG3_ENABLE
#define USART_TX3_SIZE 250
namespace Usart { static bool tx3_write(uint8_t c); static void tx3_block_ready(); }
#define dbg_char(c) do { if (!Usart::tx3_write(c)) Usart::tx3_block_ready(); } while (false)
#define _AVR_SETUP_DBG Usart::enable3();
#ifndef DBG_ENABLE
#define DBG_ENABLE
#endif
#endif
// }}}
/// @endcond

#ifdef DBG_ENABLE // {{{
/// Send a (hex) digit to debugging serial port. Parameter must be <= 0xf.
#define dbg_digit(d) dbg_char((d) < 10 ? '0' + (d) : 'a' - 10 + (d))
/// Send a byte to debugging serial port as 2 hex digits.
#define dbg_byte(b) do { dbg_digit(((b) >> 4) & 0xf); dbg_digit((b) & 0xf); } while (false)
/// Send a string to debugging serial port, without any replacements. A newline is sent after the string.
#define dbg_msg(msg) do { char const *m = (msg); while (*m) dbg_char(*m++); dbg_char('\n'); } while (false)
#include <stdio.h>
/// Send a string to debugging serial port, with any replacements. A newline is sent after the string.
/**
 * When the string contains a '#', it is replaced with the next parameter using
 * dbg_byte. If the string contains a '*', it is replaced with the next
 * parameter using dbg_byte for the high byte, then for the low byte.
 */
static inline void dbg(char const *format, ...) {
	va_list args;
	va_start(args, format);
	while (*format) {
		if (*format == '#') {
			uint8_t data = va_arg(args, int);
			dbg_byte(data);
		}
		else if (*format == '*') {
			Avr::Word data;
			data.w = va_arg(args, int);
			dbg_byte(data.b[1]);
			dbg_byte(data.b[0]);
		}
		else
			dbg_char(*format);
		++format;
	}
	va_end(args);
	dbg_char('\n');
}
#else
#define dbg_char(c)
#define dbg_digit(d)
#define dbg_byte(b)
#define dbg_msg(m)
#define dbg(...)
#endif // }}}

// Include this on all devices.
#include "test.hh"	// Include this first; if any AVR_TEST_* macro is defined, other files need code from here.
#include "gpio.hh"
#include "interrupt.hh"
#include "counter0.hh"
#include "counter1.hh"
#include "clock.hh"
#include "sleep.hh"
#include "eeprom.hh"
#include "wdt.hh"

#endif

// Main page documentation. {{{
/** @mainpage
# How to use this documentation
Use that tabs at the top to navigate to the item of interest.

  - Main Page: this page
  - Modules: Useful collections of related items, such as all macros that trigger features to be included
  - Namespaces: Not very useful, because the code uses a lot of preprocessor macros, which are not part of the namespace
  - Classes: Not very useful; it just lists some structs that Usb uses
  - Files: Useful list of available functionality, split up by hardware part. Contains both the functions in the namespace and the preprocessor macros.

# Introduction
Avr-ll (avr-low-level) is a system for building firmware for AVR devices made
by Microchip (originally by Atmel).

The design goals of this system are:

  1. Avoid putting anything in flash or SRAM unless the user requests it.
  2. Avoid limiting users in what they can do.
  3. Avoid busy waits: allow using interrupts where possible.
  4. Fix inconsistencies between devices; use the same names for equal features (for example, alias Usart registers such as UDR to UDR0).
  5. Provide good defaults for everything, so users only need to specify details when they are not trivial. This is mostly relevant for the Makefile and Usb.

# Makefile
To use the system, `make` should be used to build the firmware. The Makefile
needs to look like this:

```
# This defines which projects are built by default
TARGETS = robot-ninja

# For each project that can be built (whether it is listed in TARGETS or not),
# provide a list of sources.
#
# Files with a .cc extension are compiled, others trigger a recompile of all
# files when they are changed.
robot_ninja_SOURCES = main.cc shuriken.cc ninja.hh japanese.txt

# Define target hardware. This may be overridden from the commandline.
MCU = atmega328p
F_CPU = 16000000
# Settings for uploading the firmware to the device
PROTOCOL = arduino
BAUD = 57600

# Finally, include the magic.
include /usr/share/avr-ll/avr-ll.mk
```

# Source Code
The firmware source code needs to include `<avr-ll.hh>`. Before that, it should
define preprocessor macros for any features it wants to enable. Such features
will define data that will be stored either in flash or in memory. If no macros
are defined, no such data will be generated.

This means that for projects which are split in multiple `.cc` files, only one
of them must define each macro. The side effect of that is that only that file
can use the functionality that it defines. If this is a problem, the source
should be compiled as a single .cc file. But there is no requirement that the
source is in a single file; for example, if the real source is in several files
named *.cpp, the single file which can be named main.cc can simply include all
of them. This would be bad design for a large code base, but due to the
limitations on the flash memory of avr devices, it is never a problem for them.

# Compiling and Uploading
When the source directory is set up as described above, all targets listed in
the Makefile's TARGETS variable can be made using `make` with no arguments. If
some settings need to be set to non-default values, they can be added using
e.g. `make MCU=atmega32u4`. Because the MCU and F_CPU values are part of the
resulting file name, it is possible to run `make` multiple times to produce
output for several hardware options.

For uploading, the command `make upload` is used. This will (re)build the first
target from TARGETS if it isn't up to date, and use avrdude to flash it into
the device. If a different target should be used, the TARGETS variable can be
set on the commandline, just like any other variable:
`make upload TARGETS=robot-femboy`.
*/
// }}}

// vim: set foldmethod=marker :
