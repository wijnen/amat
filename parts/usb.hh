// Universal Serial Bus

// Options:
// USB_ENABLE_SIMPLE_HID_SETUP
// USB_ENABLE_KEYBOARD
// USB_ENABLE_MOUSE
// USB_ENABLE_SERIAL
// USB_SERIAL_SIZE
// USB_ENABLE
// CALL_usb_hid_set_report
// USB_CAN_WAKEUP
// USB_ENABLE_HID
// CALL_usb_hid_physical
// USB_ENABLE_BOOT_KEYBOARD_REPORT_DESCRIPTOR
// USB_ENABLE_BOOT_MOUSE_REPORT_DESCRIPTOR
// CALL_usb_out
// CALL_usb_nak_in
// CALL_usb_overflow
// CALL_usb_underflow
// CALL_usb_reset
// CALL_usb_configured
// CALL_usb_suspended
// CALL_usb_wakeup
// CALL_usb_sof
// USB_RX1_BULK
// USB_RX1_INTERRUPT
// CALL_usb_get_descriptor
// CALL_usb_standard_setup0
// CALL_usb_setup0
// CALL_usb_setup
// CALL_usb_out0
// CALL_usb_in0
// CALL_usb_in
// CALL_usb_stalled
// CALL_usb_nak_out
// CALL_usb_deconfigured
// CALL_usb_resuming
// CALL_usb_resumed
// CALL_usb_pll_wait
// USB_TX1_BULK
// USB_TX1_INTERRUPT
// USB_LOW_SPEED
// CALL_usb_resuming
// CALL_usb_resumed
// CALL_usb_setup
// CALL_usb_deconfigured
// CALL_usb_standard_setup0
// CALL_usb_hid_get_idle
// CALL_usb_hid_get_protocol
// CALL_usb_hid_set_report
// CALL_usb_hid_set_idle
// CALL_usb_hid_set_protocol

#ifndef _AVR_USB_HH
#define _AVR_USB_HH

/// @cond
// Comment out to enable dbg for usb.
#define udbg(...) do {} while (false)
/// @endcond

/**
 * @file
 * Universial Serial Bus
 *
 * @author Bas Wijnen <wijnen@debian.org>
 */

#ifdef DOXYGEN

/// This must be defined to enable USB features @ingroup usemacros
/**
 * Other macros need to be defined to enable more features.
 *
 * If this macro is not defined, none of the other features can be enabled.
 */
#define USB_ENABLE

/// If defined, the device is configured to be low speed.
#define USB_LOW_SPEED

/// Defining this macro enables the human interface device code for USB @ingroup usemacros
/**
 * Type definitions are still enabled without this macro.
 *
 * This macro only enables code parts, such as HID-specific GET_DESCRIPTOR responses.
 *
 * When using USB_SIMPLE_HID_SETUP, this is defined automatically.
 */
#define USB_ENABLE_HID

/// Define this and usb_hid_physical to return physical descriptors on GET_DESCRIPTOR requests.
#define CALL_usb_hid_physical

/// Store the report descriptor for the boot protocol keyboard in PROGMEM
/**
 * It is named boot_keyboard_report_descriptor
 */
#define USB_ENABLE_BOOT_KEYBOARD_REPORT_DESCRIPTOR

/// Store the report descriptor for the boot protocol mouse in PROGMEM
/**
 * It is named boot_mouse_report_descriptor
 */
#define USB_ENABLE_BOOT_MOUSE_REPORT_DESCRIPTOR

/// Set up everything for boot keyboard device.
#define USB_ENABLE_KEYBOARD

/// Set up everything for boot keyboard device.
#define USB_ENABLE_MOUSE

/// Set up everything for boot keyboard device.
#define USB_ENABLE_SERIAL

#endif

/// @cond
#ifndef udbg
#define udbg dbg
#endif
/// @endcond

// Use =, not &=, because setting bits to 1 has no effect and otherwise there
// is a race condition where another flag is cleared because it was not set
// during the read operation.
// Use the cast to 8 bit to avoid an error when flag == 7.
/// Clear an endpoint interrupt flag without affecting the other flags.
/**
 * This sets RXOUTI/KILLBK to 1 (unless it is the bit that is cleared), which
 * means it should not be used on IN endpoints (or control endpoints in the IN
 * phase), as it will kill the pending memory bank.
 *
 * @sa USB_CLEAR_IN_INT
 */
#define USB_CLEAR_OUT_INT(bit) \
	do { \
		/*udbg("Clearing bit #", bit);*/ \
		UEINTX = uint8_t(~_BV(bit)); \
	} while (false)

/// Clear an endpoint interrupt flag without affecting the other flags.
/**
 * This sets RXOUTI/KILLBK to its current value, which means it should not be
 * used on OUT endpoints (or control endpoints in the OUT phase), because it
 * creates a race condition where if an OUT packet is received after the
 * current value of the bit is read, but before it is written, the interrupt
 * will immediately be acknowledged, and it will not be handled.
 *
 * @sa USB_CLEAR_OUT_INT
 */
#define USB_CLEAR_IN_INT(bit) \
	do { \
		/*udbg("Clearing bit (preserve KILLBK) #", bit);*/ \
		UEINTX = (UEINTX | ~_BV(RXOUTI)) & uint8_t(~_BV(bit)); \
	} while (false)

/// Clear setup interrupt; set all other bits (except those that should be cleared) so they are not inadvertently changed due to race condition.
//#define USB_CLEAR_SETUP() do { UEINTX = ~(_BV(RXSTPI) | _BV(NAKINI) | _BV(NAKOUTI) | _BV(STALLEDI)); } while (false)
#define USB_CLEAR_SETUP() do { UEINTX &= ~(_BV(RXSTPI) | _BV(NAKINI) | _BV(NAKOUTI) | _BV(STALLEDI)); } while (false)

#ifdef AVR_TEST_USB
#define USB_ENABLE
#define USB_RX2_BULK 32
#define USB_TX3_BULK 32
#define USB_DOUBLE_BANK(configuration) 0x02
#define USB_MAX_INTERFACES 2

// Alias send and receive functions for test system.
#define test_tx tx3_write
#define usb_read Usb::rx2_read
#define usb_rx_pop Usb::rx2_pop
#define usb_rx usb_recv2
#endif

// Handle convenience setup macros

#ifdef USB_ENABLE_SIMPLE_HID_SETUP
#define USB_ENABLE
#define USB_ENABLE_HID
#define USB_TX1_INTERRUPT 32
#define USB_RX2_INTERRUPT 32
#define USB_EP2_SIZE 8
#endif

#ifndef USB_HID_SET_REPORT_SIZE
/// Define this to override the default buffer size for the packet buffer for SET_REPORT.
/**
 * The buffer is named Usb::hid_set_report. It is only created when CALL_usb_hid_set_report is defined.
 *
 * @sa USB_HID_SET_REPORT_PACKETS, CALL_usb_hid_set_report.
 */
#define USB_HID_SET_REPORT_SIZE 8
#endif

#ifndef USB_HID_SET_REPORT_PACKETS
/// Define this to override the default number of packets in the SET_REPORT packet buffer.
/**
 * If USB_HID_SET_REPORT_SIZE is not defined, this value is not used.
 *
 * @sa USB_HID_SET_REPORT_SIZE, CALL_usb_hid_set_report.
 */
#define USB_HID_SET_REPORT_PACKETS 6
#endif

#if defined(USB_ENABLE_KEYBOARD) || defined(USB_ENABLE_MOUSE) || defined(USB_ENABLE_SERIAL)

namespace Usb {}

/// @cond
#define _AVR_SET_INTERRUPT_BUFFER(oldname, newname) \
	static inline void newname ## _reset() { oldname ## _reset(); } \
	static inline uint8_t newname ## _buffer_allocated_size() { return oldname ## _buffer_allocated_size(); } \
	static inline uint8_t newname ## _packet_length() { return oldname ## _packet_length(); } \
	static inline uint8_t newname ## _buffer_available() { return oldname ## _buffer_available(); } \
	static inline uint8_t newname ## _read(uint8_t pos = 0) { return oldname ## _read(pos); } \
	static inline bool newname ## _write(uint8_t data) { return oldname ## _write(data); } \
	static inline void newname ## _partial_pop(uint8_t n) { oldname ## _partial_pop(n); } \
	static inline void newname ## _pop() { oldname ## _pop(); } \
	static inline void newname ## _end() { oldname ## _end(); }

#define _AVR_SET_BULK_BUFFER(oldname, newname) \
	static inline void newname ## _reset() { oldname ## _reset(); } \
	static inline uint8_t newname ## _buffer_allocated_size() { return oldname ## _buffer_allocated_size(); } \
	static inline uint8_t newname ## _buffer_used() { return oldname ## _buffer_used(); } \
	static inline uint8_t newname ## _buffer_available() { return oldname ## _buffer_available(); } \
	static inline void newname ## _pop(uint8_t num = 1) { return oldname ## _pop(num); } \
	static inline uint8_t newname ## _read(uint8_t pos = 0) { return oldname ## _read(pos); } \
	static inline void newname ## _move(uint8_t *buffer, uint8_t num) { oldname ## _move(buffer, num); } \
	static inline bool newname ## _write(uint8_t data) { return oldname ## _write(data); }
/// @endcond

#define USB_ENABLE

#ifdef USB_ENABLE_KEYBOARD // {{{

#define USB_ENABLE_HID
#ifndef USB_KEYBOARD_REPORT_DESCRIPTOR
#define USB_ENABLE_BOOT_KEYBOARD_REPORT_DESCRIPTOR

/// Override this definition to use a custom keyboard descriptor
/**
 * When USB_ENABLE_KEYBOARD is defined, it will used
 * Usb::boot_keyboard_report_descriptor as its report descriptor by default. If
 * another descriptor should be used instead, it must be stored in PROGMEM and
 * this macro must be defined as the variable name.
 *
 * Note that the keyboard must still follow the boot protocol when
 * Usb::keyboard_protocol is set to 0.
 */
#define USB_KEYBOARD_REPORT_DESCRIPTOR Usb::boot_keyboard_report_descriptor
#endif

namespace Usb {
	/// Current keyboard protocol
	/**
	 * Current protocol for the keyboard (0 means boot protocol, 1 means
	 * report protocol). This is mostly relevant when
	 * USB_KEYBOARD_REPORT_DESCRIPTOR is defined to a non-default report
	 * descriptor.
	 */
	static uint8_t keyboard_protocol;
}
/// @cond
#define _AVR_USB_KEYBOARD_SETUP Usb::keyboard_protocol = 1;
#define USB_TX1_INTERRUPT 32

#define _AVR_CONFIGURATION_KEYBOARD_TYPE \
		Usb::Interface interface_keyboard; \
		Usb::HIDSimple hid_keyboard; \
		Usb::Endpoint ep_keyboard;

#define _AVR_CONFIGURATION_KEYBOARD \
		interface_keyboard: USB_BUILD_INTERFACE(0, 0, 1, Usb::CLASS_HID, Usb::HID_BOOT_INTERFACE, Usb::HID_KEYBOARD, 0), \
		hid_keyboard: USB_SIMPLE_HID(sizeof(USB_KEYBOARD_REPORT_DESCRIPTOR)), \
		ep_keyboard: USB_SIMPLE_ENDPOINT(0x81, Usb::INTERRUPT),

#ifdef USB_ENABLE_SERIAL
#define _AVR_DEVICE_DESCRIPTOR USB_SIMPLE_IA_DEVICE
#else
#define _AVR_DEVICE_DESCRIPTOR USB_SIMPLE_DEVICE
#endif

#ifdef USB_ENABLE_MOUSE
#define _AVR_HID_DESCRIPTORS \
static uint8_t const *usb_hid_descriptor(uint8_t configuration, uint8_t interface) { \
	(void)&configuration; \
	return (interface == 0 ? reinterpret_cast <uint8_t const *>(&_avr_hid_configuration.hid_keyboard) : reinterpret_cast <uint8_t const *>(&_avr_hid_configuration.hid_mouse)); \
} \
static uint16_t usb_hid_descriptor_size(uint8_t configuration, uint8_t interface) { \
	(void)&configuration; \
	return (interface == 0 ? sizeof(_avr_hid_configuration.hid_keyboard) : sizeof(_avr_hid_configuration.hid_mouse)); \
} \
static uint8_t const *usb_hid_report_descriptor(uint8_t configuration, uint8_t interface) { \
	(void)&configuration; \
	return (interface == 0 ? reinterpret_cast <uint8_t const *>(USB_KEYBOARD_REPORT_DESCRIPTOR) : reinterpret_cast <uint8_t const *>(USB_MOUSE_REPORT_DESCRIPTOR)); \
} \
static uint16_t usb_hid_report_descriptor_size(uint8_t configuration, uint8_t interface) { \
	(void)&configuration; \
	return (interface == 0 ? sizeof(USB_KEYBOARD_REPORT_DESCRIPTOR) : sizeof(USB_MOUSE_REPORT_DESCRIPTOR)); \
} \
static void usb_hid_set_protocol(uint8_t configuration, uint8_t interface, uint8_t protocol) { \
	(void)&configuration; \
	if (interface == 0) \
		Usb::keyboard_protocol = protocol; \
	else \
		Usb::mouse_protocol = protocol; \
} \
static uint8_t usb_hid_get_protocol(uint8_t configuration, uint8_t interface) { \
	(void)&configuration; \
	return (interface == 0 ? Usb::keyboard_protocol : Usb::mouse_protocol); \
}
#else
#define _AVR_HID_DESCRIPTORS \
	SINGLE_HID_DESCRIPTOR(_avr_hid_configuration.hid_keyboard, USB_KEYBOARD_REPORT_DESCRIPTOR) \
	static void usb_hid_set_protocol(uint8_t configuration, uint8_t interface, uint8_t protocol) { (void)&configuration; (void)&interface; Usb::keyboard_protocol = protocol; } \
	static uint8_t usb_hid_get_protocol(uint8_t configuration, uint8_t interface) { (void)&configuration; (void)&interface; return Usb::keyboard_protocol; }
#endif
/// @endcond

/// Define this and usb_hid_set_protocol to get a callback when a SET_PROTOCOL request is received.
/**
 * This macro and the callback function are both defined when USB_ENABLE_KEYBOARD or USB_ENABLE_MOUSE are defined.
 */
#define CALL_usb_hid_set_protocol

/// Define this and usb_hid_get_protocol to get a callback when a GET_PROTOCOL request is received.
/**
 * This macro and the callback function are both defined when USB_ENABLE_KEYBOARD or USB_ENABLE_MOUSE are defined.
 */
#define CALL_usb_hid_get_protocol

/// Define this and usb_hid_set_report to get a callback when a SET_REPORT request is received.
/**
 * This macro and the callback function are both defined when USB_ENABLE_KEYBOARD is defined.
 */
#define CALL_usb_hid_set_report

/// @cond
#define _AVR_MOUSE_EP 2
#define _AVR_MOUSE_INTERFACE 1

#define USB_TX1_INTERRUPT 32
#define _AVR_USB_DELAYED_KEYBOARD_SETUP \
	namespace Usb { _AVR_SET_INTERRUPT_BUFFER(tx1, keyboard) }
/// @endcond

#else

#define _AVR_USB_DELAYED_KEYBOARD_SETUP
#define _AVR_CONFIGURATION_KEYBOARD_TYPE
#define _AVR_CONFIGURATION_KEYBOARD
#define _AVR_USB_KEYBOARD_SETUP
#define _AVR_MOUSE_EP 1
#define _AVR_MOUSE_INTERFACE 0

#endif // }}}

#ifdef USB_ENABLE_MOUSE // {{{
#define USB_ENABLE_BOOT_MOUSE_REPORT_DESCRIPTOR

#ifndef USB_ENABLE_HID
#define USB_ENABLE_HID
#endif

#ifndef USB_MOUSE_REPORT_DESCRIPTOR
#define USB_ENABLE_BOOT_MOUSE_REPORT_DESCRIPTOR
/// Override this definition to use a custom mouse descriptor
/**
 * When USB_ENABLE_MOUSE is defined, it will used
 * Usb::boot_mouse_report_descriptor as its report descriptor by default. If
 * another descriptor should be used instead, it must be stored in PROGMEM and
 * this macro must be defined as the variable name.
 *
 * Note that the mouse must still follow the boot protocol when
 * Usb::mouse_protocol is set to 0.
 */
#define USB_MOUSE_REPORT_DESCRIPTOR Usb::boot_mouse_report_descriptor
#endif

namespace Usb {
	/// Current mouse protocol
	/**
	 * Current protocol for the mouse (0 means boot protocol, 1 means
	 * report protocol). This is mostly relevant when
	 * USB_MOUSE_REPORT_DESCRIPTOR is defined to a non-default report
	 * descriptor.
	 */
	static uint8_t mouse_protocol;
}
/// @cond
#define _AVR_USB_MOUSE_SETUP Usb::mouse_protocol = 1;

#define _AVR_SERIAL_EP (_AVR_MOUSE_EP + 1)
#define _AVR_SERIAL_INTERFACE (_AVR_MOUSE_INTERFACE + 1)

#define _AVR_CONFIGURATION_MOUSE_TYPE \
		Usb::Interface interface_mouse; \
		Usb::HIDSimple hid_mouse; \
		Usb::Endpoint ep_mouse;

#define _AVR_CONFIGURATION_MOUSE \
		interface_mouse: USB_BUILD_INTERFACE(_AVR_MOUSE_INTERFACE, 0, 1, Usb::CLASS_HID, Usb::HID_BOOT_INTERFACE, Usb::HID_MOUSE, 0), \
		hid_mouse: USB_SIMPLE_HID(sizeof(USB_MOUSE_REPORT_DESCRIPTOR)), \
		ep_mouse: USB_SIMPLE_ENDPOINT(_AVR_MOUSE_EP | 0x80, Usb::INTERRUPT),

#ifdef USB_ENABLE_KEYBOARD

#define USB_TX2_INTERRUPT 32
#define _AVR_USB_DELAYED_MOUSE_SETUP namespace Usb { _AVR_SET_INTERRUPT_BUFFER(tx2, mouse) }

#else

#define USB_TX1_INTERRUPT 32
#define _AVR_USB_DELAYED_MOUSE_SETUP namespace Usb { _AVR_SET_INTERRUPT_BUFFER(tx1, mouse) }

#ifdef USB_ENABLE_SERIAL
#define _AVR_DEVICE_DESCRIPTOR USB_SIMPLE_IA_DEVICE
#else
#define _AVR_DEVICE_DESCRIPTOR USB_SIMPLE_DEVICE
#endif

#define _AVR_HID_DESCRIPTORS \
	SINGLE_HID_DESCRIPTOR(_avr_hid_configuration.hid_mouse, USB_MOUSE_REPORT_DESCRIPTOR) \
	static void usb_hid_set_protocol(uint8_t configuration, uint8_t interface, uint8_t protocol) { (void)&configuration; (void)&interface; Usb::mouse_protocol = protocol; } \
	static uint8_t usb_hid_get_protocol(uint8_t configuration, uint8_t interface) { (void)&configuration; (void)&interface; return Usb::mouse_protocol; }

#define CALL_usb_hid_set_protocol
#define CALL_usb_hid_get_protocol

#endif
/// @endcond

#else

#define _AVR_USB_MOUSE_SETUP
#define _AVR_USB_DELAYED_MOUSE_SETUP
#define _AVR_CONFIGURATION_MOUSE_TYPE
#define _AVR_CONFIGURATION_MOUSE
#define _AVR_SERIAL_EP _AVR_MOUSE_EP
#define _AVR_SERIAL_INTERFACE _AVR_MOUSE_INTERFACE

#endif // }}}

/// @cond
#ifdef USB_ENABLE_SERIAL // {{{

#if !defined(USB_ENABLE_KEYBOARD) && !defined(USB_ENABLE_MOUSE)
#define _AVR_DEVICE_DESCRIPTOR USB_SIMPLE_DEVICE
#define _AVR_IA_CONFIG_TYPE
#define _AVR_IA_CONFIG
#else
#define _AVR_IA_CONFIG_TYPE Usb::USBIA usb_ia;
#define _AVR_IA_CONFIG usb_ia: USB_BUILD_IA(_AVR_SERIAL_INTERFACE, 2, 2, 2, 0),
#endif

#ifdef USB_SERIAL_SIZE
#define USB_SERIAL_RX_SIZE USB_SERIAL_SIZE
#define USB_SERIAL_TX_SIZE USB_SERIAL_SIZE
#endif

#ifndef USB_SERIAL_RX_SIZE
#define USB_SERIAL_RX_SIZE 32
#endif

#ifndef USB_SERIAL_TX_SIZE
#define USB_SERIAL_TX_SIZE 32
#endif

#if _AVR_SERIAL_EP == 1

#define USB_RX2_BULK USB_SERIAL_RX_SIZE
#define usb_serial_rx usb_recv2
#define USB_TX3_BULK USB_SERIAL_TX_SIZE
#define _AVR_USB_DELAYED_SERIAL_SETUP namespace Usb { _AVR_SET_BULK_BUFFER(rx2, serial_rx) _AVR_SET_BULK_BUFFER(tx3, serial_tx) }

#elif _AVR_SERIAL_EP == 2

#define USB_RX3_BULK USB_SERIAL_RX_SIZE
#define usb_serial_rx usb_recv3
#define USB_TX4_BULK USB_SERIAL_TX_SIZE
#define _AVR_USB_DELAYED_SERIAL_SETUP namespace Usb { _AVR_SET_BULK_BUFFER(rx3, serial_rx) _AVR_SET_BULK_BUFFER(tx4, serial_tx) }

#elif _AVR_SERIAL_EP == 3

#define USB_RX4_BULK USB_SERIAL_RX_SIZE
#define usb_serial_rx usb_recv4
#define USB_TX5_BULK USB_SERIAL_TX_SIZE
#define _AVR_USB_DELAYED_SERIAL_SETUP namespace Usb { _AVR_SET_BULK_BUFFER(rx4, serial_rx) _AVR_SET_BULK_BUFFER(tx5, serial_tx) }

#endif

#define _AVR_CONFIGURATION_SERIAL_TYPE \
		Usb::Interface interface_serial_manage; \
		Usb::CDCHeader cdc_header; \
		Usb::CDCCallManagement cdc_call_management; \
		Usb::CDCACM cdc_acm; \
		Usb::CDCUnion cdc_union; \
		Usb::Endpoint ep_serial_notify; \
		Usb::Interface interface_serial_data; \
		Usb::Endpoint ep_serial_out; \
		Usb::Endpoint ep_serial_in;

#define _AVR_CONFIGURATION_SERIAL \
		interface_serial_manage: USB_BUILD_INTERFACE(_AVR_SERIAL_INTERFACE, 0, 1, Usb::CLASS_CDC, Usb::CDC_ABSTRACT_CONTROL_MODEL, 0, 0), \
		cdc_header: USB_BUILD_CDC_HEADER(), \
		cdc_call_management: USB_BUILD_CDC_CALL_MANAGEMENT(1, _AVR_SERIAL_INTERFACE + 1), \
		cdc_acm: USB_BUILD_CDC_ACM(0), \
		cdc_union: USB_BUILD_CDC_UNION(_AVR_SERIAL_INTERFACE, _AVR_SERIAL_INTERFACE + 1), \
		ep_serial_notify: USB_SIMPLE_ENDPOINT(_AVR_SERIAL_EP | 0x80, Usb::INTERRUPT), \
		interface_serial_data: USB_BUILD_INTERFACE(_AVR_SERIAL_INTERFACE + 1, 0, 2, Usb::CLASS_CDC_DATA, 0, 0, 0), \
		ep_serial_out: USB_SIMPLE_ENDPOINT(_AVR_SERIAL_EP + 1, Usb::BULK), \
		ep_serial_in: USB_SIMPLE_ENDPOINT((_AVR_SERIAL_EP + 2) | 0x80, Usb::BULK),

#define _AVR_USB_NUM_INTERFACES _AVR_SERIAL_INTERFACE + 2

#else

#define _AVR_IA_CONFIG_TYPE
#define _AVR_IA_CONFIG
#define _AVR_CONFIGURATION_SERIAL_TYPE
#define _AVR_CONFIGURATION_SERIAL
#define _AVR_USB_NUM_INTERFACES _AVR_SERIAL_INTERFACE
#define _AVR_USB_DELAYED_SERIAL_SETUP

#endif // }}}

#define _AVR_SETUP_USB \
	Usb::disable(); \
	_AVR_USB_KEYBOARD_SETUP \
	_AVR_USB_MOUSE_SETUP \
	Usb::enable();

#ifndef _AVR_HID_DESCRIPTORS
#define _AVR_HID_DESCRIPTORS
#endif

#define _AVR_PRE_SETUP_USB \
	_AVR_USB_DELAYED_KEYBOARD_SETUP \
	_AVR_USB_DELAYED_MOUSE_SETUP \
	_AVR_USB_DELAYED_SERIAL_SETUP \
	_AVR_DEVICE_DESCRIPTOR(); \
	struct { \
		Usb::Configuration configuration; \
		_AVR_IA_CONFIG_TYPE \
		_AVR_CONFIGURATION_KEYBOARD_TYPE \
		_AVR_CONFIGURATION_MOUSE_TYPE \
		_AVR_CONFIGURATION_SERIAL_TYPE \
	} const _avr_hid_configuration PROGMEM = { \
		USB_BUILD_CONFIGURATION(sizeof(_avr_hid_configuration), _AVR_USB_NUM_INTERFACES, 1, 0, USB_CAN_WAKEUP_VALUE, USB_MAX_POWER), \
		_AVR_IA_CONFIG \
		_AVR_CONFIGURATION_KEYBOARD \
		_AVR_CONFIGURATION_MOUSE \
		_AVR_CONFIGURATION_SERIAL \
	}; \
	USB_SIMPLE_SINGLE_CONFIGURATION(_avr_hid_configuration); \
	_AVR_HID_DESCRIPTORS
/// @endcond
#endif

#ifdef USB_ENABLE

#ifdef CALL_usb_hid_set_report
/// This function must be defined when CALL_usb_hid_set_report is defined.
static void usb_hid_set_report();
#endif

// Definitions. {{{

#ifdef DOXYGEN

/// Define this macro to set the wakeup bit in Configuration descriptors.
/**
 * If this macro is defined, USB_CAN_WAKEUP_VALUE is set to true, otherwise it
 * is set to false. That macro is used in all variations of
 * USB_SIMPLE_CONFIGURATION.
 */
#define USB_CAN_WAKEUP

/// Value to use in USB_BUILD_CONFIGURATION to get the default value.
#define USB_CAN_WAKEUP_VALUE true

/// Set this value to the maximum bus power that the device uses, in units of 2mA.
/**
 * It is used by all variants of USB_SIMPLE_CONFIGURATION. It can be used in
 * USB_BUILD_CONFIGURATION to get the same value.
 */
#define USB_MAX_POWER 10

#else
#ifdef USB_CAN_WAKEUP
#define USB_CAN_WAKEUP_VALUE true
#else
#define USB_CAN_WAKEUP_VALUE false
#endif

#ifndef USB_MAX_POWER
#define USB_MAX_POWER 10
#endif
#endif

#ifdef USB_ENABLE_HID

/// When USB_ENABLE_HID is defined, this needs to be defined to allow GET_DESCRIPTOR to return it.
/**
 * The returned structure must be the same that is also sent along with the
 * configuration descriptor.
 *
 * This macro must be a pointer to the PROGMEM data that contains the hid struct.
 *
 * When using USB_SIMPLE_HID_SETUP, this is defined automatically.
 */
static uint8_t const *usb_hid_descriptor(uint8_t configuration, uint8_t interface);

/// When USB_ENABLE_HID is defined, this needs to be defined to allow GET_DESCRIPTOR to return it.
/**
 * When using USB_SIMPLE_HID_SETUP, this is defined automatically.
 */
static uint16_t usb_hid_descriptor_size(uint8_t configuration, uint8_t interface);

/// When USB_ENABLE_HID is defined, this needs to be defined to allow GET_DESCRIPTOR to return it.
/**
 * When using USB_SIMPLE_HID_SETUP, this is defined automatically.
 */
static uint8_t const *usb_hid_report_descriptor(uint8_t configuration, uint8_t interface);

/// When USB_ENABLE_HID is defined, this needs to be defined to allow GET_DESCRIPTOR to return it.
/**
 * When using USB_SIMPLE_HID_SETUP, this is defined automatically.
 */
static uint16_t usb_hid_report_descriptor_size(uint8_t configuration, uint8_t interface);

/// Define required functions for setup with a single HID interface.
/**
 * In most cases, a HID device will only contain one HID interface.
 *
 * This macro can be used to define the required usb_hid_* functions for that
 * situation.
 *
 * The first parameter must be the hid descriptor, the second parameter must be
 * the report descriptor array.
 *
 * Neither of them may be pointers; the first must be a struct, the second
 * an array. In particular, sizeof() must work properly on both.
 *
 * hid_descriptor should be part of the configuration descriptor.
 */
#define SINGLE_HID_DESCRIPTOR(hid_descriptor, report_descriptor) \
static uint8_t const *usb_hid_descriptor(uint8_t conf, uint8_t intf) { \
	(void)&conf; \
	(void)&intf; \
	return reinterpret_cast <uint8_t const *>(&hid_descriptor); \
} \
static uint16_t usb_hid_descriptor_size(uint8_t conf, uint8_t intf) { \
	(void)&conf; \
	(void)&intf; \
	return sizeof(hid_descriptor); \
} \
static uint8_t const *usb_hid_report_descriptor(uint8_t conf, uint8_t intf) { \
	(void)&conf; \
	(void)&intf; \
	return report_descriptor; \
} \
static uint16_t usb_hid_report_descriptor_size(uint8_t conf, uint8_t intf) { \
	(void)&conf; \
	(void)&intf; \
	return sizeof(report_descriptor); \
}

#ifdef CALL_usb_hid_physical
/// This function is called when a GET_DESCRIPTOR request is received specifying a physical descriptor.
static void usb_hid_physical(uint8_t index, uint8_t configuration, uint16_t interface);
#endif
#endif

// Set default values for user constants. {{{
#ifndef USB_VENDOR
/// Vendor code that is used in the Device descriptor.
#define USB_VENDOR 0xfffe
#endif

#ifndef USB_PRODUCT
/// Product code that is used in the Device descriptor.
#define USB_PRODUCT 0xffff
#endif

#ifndef USB_NUM_LANGS
/// Number of languages for String descriptors.
#define USB_NUM_LANGS 1
#endif

#ifndef USB_NUM_STRINGS
/// Number of String descriptors. If 0, all string descriptor code is omitted.
#define USB_NUM_STRINGS 0
#endif

#ifndef USB_MAX_INTERFACES
/// Maximum number of interfaces in any configuration.
#define USB_MAX_INTERFACES 1
#endif

#ifndef USB_DOUBLE_BANK
/// Bitmask of which endpoints use double memory banks.
/**
 * Default value is all endpoints except 0 have a double bank.
 */
#define USB_DOUBLE_BANK(configuration) 0x7e
#endif

#ifndef USB_DEFAULT_PACKETS
/// Default value for PACKET_BUFFER number of packets created for interrupt endpoints.
/**
 * Array size is 2 larger, and % SIZE operations are faster if that is a power of 2.
 *
 * User code can define this, or per endpoint canstants such as USB_RX1_INTERRUPT_PACKETS.
 */
#define USB_DEFAULT_PACKETS 14
#endif
// }}}

/// Full definition (Device, Configuration, Interface, Endpoint) of simple HID system.
#define USB_SIMPLE_HID_SETUP(report_descriptor, subclass, protocol) \
USB_SIMPLE_DEVICE(); \
/* Configuration descriptor (full). {{{ */ \
struct { \
	Usb::Configuration configuration; \
	Usb::Interface interface; \
	Usb::HIDSimple hid; \
	Usb::Endpoint ep_in, ep_out; \
} const simple_hid_configuration PROGMEM = { \
	USB_SIMPLE_CONFIGURATION(sizeof(simple_hid_configuration)), \
	USB_BUILD_INTERFACE(0, 0, 2, Usb::CLASS_HID, subclass, protocol, 0), \
	USB_SIMPLE_HID(sizeof(report_descriptor)), \
	USB_SIMPLE_ENDPOINT(0x81, Usb::INTERRUPT), \
	USB_SIMPLE_ENDPOINT(0x02, Usb::INTERRUPT) \
}; \
USB_SIMPLE_SINGLE_CONFIGURATION(simple_hid_configuration); \
/* }}} */ \
SINGLE_HID_DESCRIPTOR(simple_hid_configuration.hid, report_descriptor)

namespace Usb {

	// Constants. {{{
	/// Descriptor type, taken from USB standard.
	enum DescriptorType {
		/// Device descriptor
		TYPE_DEVICE = 1,
		/// Configuration descriptor
		TYPE_CONFIGURATION = 2,
		/// String descriptor
		TYPE_STRING = 3,
		/// Interface descriptor
		TYPE_INTERFACE = 4,
		/// Endpoint descriptor
		TYPE_ENDPOINT = 5,
		/// Device qualifier descriptor (device descriptor of high/full speed when operating at the other speed)
		TYPE_DEVICE_QUALIFIER = 6,
		/// Other speed configuration descriptor (configuration descriptor of high/full speed when operating at the other speed)
		TYPE_OTHER_SPEED_CONFIGURATION = 7,
		/// Interface power descriptor
		TYPE_INTERFACE_POWER = 8,

		// Class-defined types.
		/// Human interface device descriptor
		TYPE_HID = 0x21
	};
	/// Request Code, taken from USB standard.
	/**
	 * The meaning of GET and SET is from the host's point of view, like everything in the USB standard.
	 */
	enum RequestCode {
		/// Get device, interface, or endpoint status
		GET_STATUS = 0,
		/// Clear device, interface, or endpoint feature
		CLEAR_FEATURE = 1,
		/// Set device, interface, or endpoint feature
		SET_FEATURE = 3,
		/// Set device address
		SET_ADDRESS = 5,
		/// Get descriptor (target must be device)
		GET_DESCRIPTOR = 6,
		/// Set descriptor (target must be device)
		SET_DESCRIPTOR = 7,
		// Get configuration (target must be device)
		GET_CONFIGURATION = 8,
		// Set configuration (target must be device)
		SET_CONFIGURATION = 9,
		// Set interface (target must be device)
		GET_INTERFACE = 10,
		// Set interface (target must be device)
		SET_INTERFACE = 11,
		// Get synchronization frame information
		SYNCH_FRAME = 12
	};
	/// Request Code, taken from USB communication device class standard.
	enum CDC_RequestCode {
		/// Set communications feature
		SET_COMM_FEATURE = 0x02,
		/// Get communications feature
		GET_COMM_FEATURE = 0x03,
		/// Clear communications feature
		CLEAR_COMM_FEATURE = 0x04,
		/// Connect or disconnect secondary phone jack
		SET_AUX_LINE_STATE = 0x10,
		/// Set line to on-hook, off-hook, or snooping for caller ID
		SET_HOOK_STATE = 0x11,
		/// Prepare or finish pulse dialing
		PULSE_SETUP = 0x12,
		/// Dial a number by sending a series of pulses
		SEND_PULSE = 0x13,
		/// Set the timing of pulses for pulse dialing
		SET_PULSE_TIME = 0x14,
		/// Send rings to secondary phone jack
		RING_AUX_JACK = 0x15,
		/// Set serial properties (speed, data bits, parity, stop bits)
		SET_LINE_CODING = 0x20,
		/// Get serial properties (speed, data bits, parity, stop bits)
		GET_LINE_CODING = 0x21,
		/// Set RTS and DTR states
		SET_CONTROL_LINE_STATE = 0x22,
		/// Send a break signal
		SEND_BREAK = 0x23,
		/// Set ringer type and volume
		SET_RINGER_PARMS = 0x30,
		/// Get ringer type and volume, and whether a ringer is present
		GET_RINGER_PARMS = 0x31,
		/// Set whether phone can dial itself, or lets the host dial
		SET_OPERATION_PARMS = 0x32,
		/// Get whether phone can dial itself, or lets the host dial
		GET_OPERATION_PARMS = 0x33,
		/// Add or remove calls or ringing on the line, or change the active call
		SET_LINE_PARMS = 0x34,
		/// Get all information about ringer, all calls on the line and their states
		GET_LINE_PARMS = 0x35,
		/// Dial digits using DTMF
		DIAL_DIGITS = 0x36
	};
	/// Request Code, taken from USB human interface device class standard.
	enum HID_RequestCode {
		GET_REPORT = 0x01,
		GET_IDLE = 0x02,
		GET_PROTOCOL = 0x03,
		SET_REPORT = 0x09,
		SET_IDLE = 0x0a,
		SET_PROTOCOL = 0x0b
	};
	/// Features for SET_FEATURE and CLEAR_FEATURE, taken from USB standard.
	enum Features {
		/// An endpoint with HALT enabled will respond STALL to all requests
		ENDPOINT_HALT = 0,
		/// If this is enabled, the device is allowed to send a remote wakeup to the host
		DEVICE_REMOTE_WAKEUP = 1,
		/// High speed capable devices must allow setting the line in certain testing states
		/**
		 * Avr devices are not high speed capable and do not need to support this feature.
		 * They also do not have the ability to support it, because D+ and D- are not overloaded as Gpio pins.
		 */
		TEST_MODE = 2
	};
	/// Targets for SETUP packets, taken from USB standard.
	enum Targets {
		/// Device target
		TARGET_DEVICE = 0,
		/// Interface target
		TARGET_INTERFACE = 1,
		/// Endpoint target
		TARGET_ENDPOINT = 2
	};
	/// Types for Endpoint descriptors, taken from USB standard.
	enum EndpointType {
		/// Control endpoint
		CONTROL = 0,
		/// Isochronous endpoint
		ISOCHRONOUS = 1,
		/// Bulk endpoint
		BULK = 2,
		/// Interrupt endpoint
		INTERRUPT = 3
	};
	/// Direction of SETUP request, taken from USB standard.
	enum Direction {
		/// In direction: towards host
		IN = 0,
		/// Out direction: towards device
		OUT = 1
	};
	/// Device or Interface class for their descriptors, taken from USB standard.
	enum Class {
		/// No class specified
		CLASS_NONE = 0,
		/// Audio device class
		CLASS_AUDIO = 1,
		/// Communications device class
		CLASS_CDC = 2,
		/// Human interface device class
		CLASS_HID = 3,
		/// Data interface class, from USB CDC standard.
		CLASS_CDC_DATA = 0xa
	};
	// Defined in HID class.
	/// HID Interface subclass for descriptors, taken from USB HID standard.
	enum HIDSubClass {
		/// No HID subclass
		HID_NO_SUBCLASS = 0,
		/// HID boot interface
		HID_BOOT_INTERFACE = 1
	};
	// Defined in CDC class.
	/// CDC Interface subclass for descriptors, taken from USB CDC standard.
	enum CDCSubClass {
		/// ACM
		CDC_DIRECT_LINE_CONTROL_MODEL = 0x1,
		CDC_ABSTRACT_CONTROL_MODEL = 0x2,
		CDC_TELEPHONE_CONTROL_MODEL = 0x3,
		CDC_MULTI_CHANNEL_CONTROL_MODEL = 0x4,
		CDC_CAPI_CONTROL_MODEL = 0x5,
		CDC_ETHERNET_NETWORKING_CONTROL_MODEL = 0x6,
		CDC_ATM_NETWORKING_CONTROL_MODEL = 0x7,
		CDC_WIRELESS_HANDSET_CONTROL_MODEL = 0x8,
		CDC_DEVICE_MANAGEMENT = 0x9,
		CDC_MOBILE_DIRECT_LINE_MODEL = 0xa,
		CDC_OBEX = 0xb,
		CDC_ETHERNET_EMULATION_MODEL = 0xc,
		CDC_NETWORK_CONTROL_MODEL = 0xd
	};
	/// HID Interface protocol for descriptors, taken from USB HID standard.
	enum HIDProtocol {
		/// No HID protocol
		HID_NO_PROTOCOL = 0,
		/// Keyboard protocol
		HID_KEYBOARD = 1,
		/// Mouse protocol
		HID_MOUSE = 2
	};
	/// HID Descriptor types for GET_DESCRIPTOR, taken from USB HID standard.
	enum HIDDescriptor {
		/// Main HID descriptor, also sent with configuration
		HID_MAIN = 0x21,
		/// Report descriptor
		HID_REPORT = 0x22,
		/// Physical descriptor
		HID_PHYSICAL = 0x23
	};
	// }}}

/// @cond
	static bool in_interrupt = false;
	static uint8_t saved_ueienx[7];
/// @endcond

	/// SETUP packet format, taken from USB standard.
	struct Setup { // {{{
		/// Request Type
		uint8_t request_type;
		/// Request
		uint8_t request;
		/// Value
		Avr::Word value;
		/// Index
		Avr::Word index;
		/// Length
		Avr::Word length;
	}; // }}}

	// Descriptor definitions
	/// Endpoint descriptor format, taken from USB standard.
	struct Endpoint { // {{{
		/// Descriptor length
		uint8_t bLength;
		/// Descriptor type: Usb::TYPE_ENDPOINT
		uint8_t bDescriptorType;
		/// Endpoint address and direction
		uint8_t bEndpointAddress;
		/// Endpoint type
		uint8_t bmAttributes;
		/// Maximum packet size for this endpoint
		uint16_t wMaxPacketSize;
		/// Polling interval for interrupt endpoints
		uint8_t bInterval;
	};
	/// Constructor helper for Endpoint.
#define USB_BUILD_ENDPOINT(address, attributes, max_packet_size, interval) \
	Usb::Endpoint { \
		bLength: sizeof(Usb::Endpoint), \
		bDescriptorType: Usb::TYPE_ENDPOINT, \
		bEndpointAddress: address, \
		bmAttributes: attributes, \
		wMaxPacketSize: max_packet_size, \
		bInterval: interval \
	}
	/// Alternative to USB_BUILD_ENDPOINT with reasonable defaults filled in.
#define USB_SIMPLE_ENDPOINT(address, type) USB_BUILD_ENDPOINT(address, type, 8, 1)
	// }}}

// HID descriptors. {{{
	/// HID descriptor item used at the end of the HID descriptor.
	/**
	 * @sa USB_BUILD_HID_TYPE
	 */
	struct HIDDescriptorItem {
		/// HID descriptor type
		uint8_t bDescriptorType;
		/// HID descriptor length
		uint16_t wDescriptorLength;
	};
	/// Type creator for HID descriptor.
	/**
	 * The HID descriptor is variable length due to the list of HIDDescriptorItem at the end.
	 *
	 * This macro creates a type for the given number of descriptors at the end.
	 *
	 * @sa HIDSimple and HIDMax are predefined types with 1 and 255 descriptors respectively.
	 * USB_BUILD_HID (and USB_SIMPLE_HID) use the types that are created with this macro.
	 */
#define USB_BUILD_HID_TYPE(name, num_descriptors) /* {{{ */ \
	struct name { \
		uint8_t bLength; \
		uint8_t bDescriptorType; \
		uint16_t bcdHID; \
		uint8_t bCountryCode; \
		uint8_t bNumDescriptors; \
		Usb::HIDDescriptorItem descriptor[num_descriptors]; \
	} \
	/* }}} */
/// @cond
	USB_BUILD_HID_TYPE(HIDSimple, 1);
	USB_BUILD_HID_TYPE(HIDMax, 0xff);
/// @endcond
	/// HID type created with USB_BUILD_HID_TYPE, containing 1 descriptor.
	/**
	 * This can be used for simple cases, where there is only a Report descriptor.
	 */
	struct HIDSimple;
	/// HID type created with USB_BUILD_HID_TYPE, containing 255 descriptors.
	/**
	 * This is not instantiated, but used as the target type of pointers with unknown length.
	 */
	struct HIDMax;
	/// Constructor helper for types created with USB_BUILD_HID_TYPE, such as HIDSimple.
	/**
	 * @sa USB_SIMPLE_HID
	 */
#define USB_BUILD_HID(type_name, country, report_length, num_extra_descriptors, ...) /* {{{ */ \
	type_name { \
		bLength: 9 + 3 * num_extra_descriptors, \
		bDescriptorType: Usb::HID_MAIN, \
		bcdHID: 0x0101, \
		bCountryCode: country, \
		bNumDescriptors: num_extra_descriptors + 1, \
		{ Usb::HID_REPORT, report_length }, \
		## __VA_ARGS__ \
	} /* }}} */
	/// Alternative to USB_BUILD_HID with reasonable defaults filled in.
#define USB_SIMPLE_HID(report_length) USB_BUILD_HID(Usb::HIDSimple, 0, report_length, 0)

	// Report descriptors.

/// @cond
#define USB_HID0(typetag)		(typetag | 0)
#define USB_HID1(typetag, d)	(typetag | 1), (d)
#define USB_HID2(typetag, d)	(typetag | 2), (d) & 0xff, ((d) >> 8) & 0xff
#define USB_HID4(typetag, d)	(typetag | 3), (d) & 0xff, ((d) >> 8) & 0xff, ((d) >> 16) & 0xff, ((d) >> 24) & 0xff
#define USB_HID_LONG_HEADER(type, tag, len)	(0xf0 | type | 2), (len), (tag)
/// @endcond

	/// Tags for HID Report descriptors, taken from USB HID standard.
	enum HIDReportItem {
		/// Input (main item)
		/**
		 * This causes Report_Count items of Report_Size bits each to
		 * be added to the input report.
		 *
		 * Logical_Minimum and Logical_Maximum define the range that can be reported.
		 *
		 * Physical_Minimum and Physical_Maximum define how that maps to reality.
		 *
		 * Usage_Minimum and Usage_Maximum defines what Usage the values are about:
		 *
		 * For data items, Usage_Maximum - Usage_Minimum + 1 should be equal to Report_Count. Each field gives the value of one Usage from the range.
		 *
		 * For array items, Usage_Minimum defines the Usage of the Logical_Minimum and
		 * Usage_Maximum - Usage_Minimum should be equal to Logical_Maximum - Logical_Minimum.
		 * Arrays can only have "on" or "off" values for their Usages.
		 */
		Input = 0x80,
		/// Output (main item)
		/**
		 * @sa Ranges are described in Input.
		 */
		Output = 0x90,
		/// Feature (main item)
		/**
		 * @sa Ranges are described in Input.
		 */
		Feature = 0xb0,
		/// Collection (main item)
		Collection = 0xa0,
		/// End collection (main item)
		End_Collection = 0xc0,

		/// Upper bits of Usage (global item)
		Usage_Page = 0x04,
		/// Minimum valid reported value (global item)
		Logical_Minimum = 0x14,
		/// Maximum valid reported value (global item)
		Logical_Maximum = 0x24,
		/// Physical value of Logical_Minimum (global item)
		Physical_Minimum = 0x34,
		/// Physical value of Logical_Maximum (global item)
		Physical_Maximum = 0x44,
		/// Base 10 value of unit exponent (global item)
		Unit_Exponent = 0x54,
		/// Unit (global item)
		Unit = 0x64,
		/// Number of fields in following main item(s) (global item)
		Report_Size = 0x74,
		/// ID for report (global item)
		Report_ID = 0x84,
		/// Number of repeats for following main item(s) (global item)
		Report_Count = 0x94,
		/// Place a copy of global item table on the stack
		Push = 0xa4,
		/// Replace the global item table with the top of the stack, which is removed
		Pop = 0xb4,

		// Local items.
		/// Lower bits of Usage, or all of the bits (local item)
		Usage = 0x08,
		/// Minimum Usage code (local item)
		/**
		 * For array items: Usage code for index Logical_Minimum.
		 *
		 * For data items: Usage code for first reported field.
		 */
		Usage_Minimum = 0x18,
		/// Maximum Usage code (local item)
		/**
		 * For array items: Usage code for index Logical_Maximum.
		 *
		 * For data items: Usage code for last reported field.
		 */
		Usage_Maximum = 0x28,
		/// Index to designator in physical descriptor (local item)
		Designator_Index = 0x38,
		/// Minimum designator index (local item)
		/**
		 * For array items: Designator index for index Logical_Minimum.
		 *
		 * For data items: Designator index for first reported field.
		 */
		Designator_Minimum = 0x48,
		/// Maximum designator index (local item)
		/**
		 * For array items: Designator index for index Logical_Maximum.
		 *
		 * For data items: Designator index for last reported field.
		 */
		Designator_Maximum = 0x58,
		/// String descriptor index describing the control(s) (local item)
		String_Index = 0x78,
		/// Minimum String index(local item)
		/**
		 * For array items: String index for index Logical_Minimum.
		 *
		 * For data items: String index for first reported field.
		 */
		String_Minimum = 0x88,
		/// Maximum String index (local item)
		/**
		 * For array items: String index for index Logical_Maximum.
		 *
		 * For data items: String index for last reported field.
		 */
		String_Maximum = 0x98,
		/// Usage grouping (local item)
		/**
		 * Multiple Usages can be specified for one data item by using
		 * a start delimiter, then all the Usage options, then an end
		 * delimiter.
		 */
		Delimiter = 0xa8
	};

#ifdef USB_ENABLE_BOOT_KEYBOARD_REPORT_DESCRIPTOR
#ifdef DOXYGEN
	/// Report descriptor for default (boot protocol) keyboard.
	/**
	 * This descriptor is used for the keyboard when USB_ENABLE_KEYBOARD is
	 * defined, unless USB_KEYBOARD_REPORT_DESCRIPTOR is defined. If this
	 * descriptor is used, the keyboard report format is the same for boot
	 * protocol and report protocol.
	 *
	 * This report is: 1 byte for all modifier keys, 1 zero byte, up to 6
	 * bytes of key codes for currently pressed keys.
	 *
	 * When a report is sent that does not contain a key as pressed, it is
	 * released.
	 */
	uint8_t const boot_keyboard_report_descriptor[];
#endif
/// @cond
	// This report descriptor is copied verbatim from the USB HID standard.
	uint8_t const boot_keyboard_report_descriptor[] PROGMEM = {
		USB_HID1(Usage_Page, 1),	// Generic Desktop
		USB_HID1(Usage, 6),		// Keyboard
		USB_HID1(Collection, 1),	// Collection Application

			USB_HID1(Report_Size, 1),
			USB_HID1(Report_Count, 8),
			USB_HID1(Usage_Page, 7),	// Key Codes
			USB_HID1(Usage_Minimum, 224),	// Modifiers
			USB_HID1(Usage_Maximum, 231),
			USB_HID1(Logical_Minimum, 0),
			USB_HID1(Logical_Maximum, 1),
			USB_HID1(Input, 2),		// Data, variable, absolute

			USB_HID1(Report_Count, 1),	// Reserved byte
			USB_HID1(Report_Size, 8),
			USB_HID1(Input, 1),		// Constant

			USB_HID1(Report_Count, 5),
			USB_HID1(Report_Size, 1),
			USB_HID1(Usage_Page, 8),	// LEDs
			USB_HID1(Usage_Minimum, 1),
			USB_HID1(Usage_Maximum, 5),
			USB_HID1(Output, 2),		// Data, variable, absolute

			USB_HID1(Report_Count, 1),
			USB_HID1(Report_Size, 3),
			USB_HID1(Output, 1),		// Constant

			USB_HID1(Report_Count, 6),
			USB_HID1(Report_Size, 8),
			USB_HID1(Logical_Minimum, 0),
			USB_HID1(Logical_Maximum, 255),
			USB_HID1(Usage_Page, 7),	// Key Codes
			USB_HID1(Usage_Minimum, 0),
			USB_HID1(Usage_Maximum, 255),
			USB_HID1(Input, 0),

		USB_HID0(End_Collection)
	};
/// @endcond
#endif

#ifdef USB_ENABLE_BOOT_MOUSE_REPORT_DESCRIPTOR
#ifdef DOXYGEN
	/// Report descriptor for default (boot protocol) mouse.
	/**
	 * This descriptor is used for the mouse when USB_ENABLE_MOUSE is
	 * defined, unless USB_MOUSE_REPORT_DESCRIPTOR is defined. If this
	 * descriptor is used, the mouse report format is the same for boot
	 * protocol and report protocol.
	 *
	 * This report is: 1 byte for buttons (only lower 3 bits used), 1 byte
	 * for X displacement (range -127 to 127) and 1 byte for Y displacement
	 * (range -127 to 127).
	 */
	uint8_t const boot_mouse_report_descriptor[];
#endif
/// @cond
	// This report descriptor is copied verbatim from the USB HID standard.
	uint8_t const boot_mouse_report_descriptor[] PROGMEM = {
		USB_HID1(Usage_Page, 1),	// Generic Desktop
		USB_HID1(Usage, 2),		// Mouse
		USB_HID1(Collection, 1),	// Collection Application

			USB_HID1(Usage, 1),		// Pointer
			USB_HID1(Collection, 0),	// Collection Physical

				USB_HID1(Report_Count, 3),
				USB_HID1(Report_Size, 1),
				USB_HID1(Usage_Page, 9),	// Buttons
				USB_HID1(Usage_Minimum, 1),
				USB_HID1(Usage_Maximum, 3),
				USB_HID1(Logical_Minimum, 0),
				USB_HID1(Logical_Maximum, 1),
				USB_HID1(Input, 2),		// Data, variable, absolute

				USB_HID1(Report_Count, 1),	// Padding
				USB_HID1(Report_Size, 5),
				USB_HID1(Input, 1),		// Constant

				USB_HID1(Report_Size, 8),
				USB_HID1(Report_Count, 2),
				USB_HID1(Usage_Page, 1),	// Generic Desktop
				USB_HID1(Usage, 0x30),		// X
				USB_HID1(Usage, 0x31),		// Y
				USB_HID1(Logical_Minimum, 0x81),
				USB_HID1(Logical_Maximum, 0x7f),
				USB_HID1(Input, 6),		// Data, variable, relative

			USB_HID0(End_Collection),
		USB_HID0(End_Collection)
	};
/// @endcond
#endif

	// }}}

	/// Interface descriptor format, taken from USB standard.
	struct Interface { // {{{
		/// Descriptor length
		uint8_t bLength;		// default
		/// Descriptor type: Usb::TYPE_INTERFACE
		uint8_t bDescriptorType;	// default
		/// Interface number, first interface is 0
		uint8_t bInterfaceNumber;	// user
		/// Alternate setting, default setting is 0
		uint8_t bAlternateSetting;	// user
		/// Number of endpoints in this interface
		uint8_t bNumEndpoints;		// user
		/// Interface class
		uint8_t bInterfaceClass;	// user
		/// Interface subclass
		uint8_t bInterfaceSubClass;	// user
		/// Interface protocol
		uint8_t bInterfaceProtocol;	// user
		/// String index for interface description
		uint8_t iInterface;		// user
	};
	/// Constructor helper for Interface.
#define USB_BUILD_INTERFACE(id, alternate, num_endpoints, interface_class, interface_subclass, interface_protocol, interface) \
	Usb::Interface { \
		bLength: sizeof(Usb::Interface), \
		bDescriptorType: Usb::TYPE_INTERFACE, \
		bInterfaceNumber: id, \
		bAlternateSetting: alternate, \
		bNumEndpoints: num_endpoints, \
		bInterfaceClass: interface_class, \
		bInterfaceSubClass: interface_subclass, \
		bInterfaceProtocol: interface_protocol, \
		iInterface: interface \
	}
	/// Alternative to USB_BUILD_INTERFACE with reasonable defaults filled in.
#define USB_SIMPLE_INTERFACE(id, endpoints) USB_BUILD_INTERFACE(id, 0, endpoints, 0, 0, 0, 0)
	/// Alternative to USB_BUILD_INTERFACE with reasonable defaults filled in for HID interface.
#define USB_SIMPLE_HID_INTERFACE(id, endpoints) USB_BUILD_INTERFACE(id, 0, endpoints, Usb::CLASS_HID, Usb::HID_NO_SUBCLASS, Usb::HID_NO_PROTOCOL, 0)
	// }}}

	/// Configuration descriptor format, taken from USB standard.
	struct Configuration { // {{{
		/// Descriptor length, not including descriptors that follow
		uint8_t bLength;		// default
		/// Descriptor type: Usb::TYPE_CONFIGURATION
		uint8_t bDescriptorType;	// default
		/// Descriptor length, including all descriptors that follow
		uint16_t wTotalLength;		// computed
		/// Number of interfaces in this configuration
		uint8_t bNumInterfaces;		// user
		/// Value for this configuration, first is 1
		uint8_t bConfigurationValue;	// computed
		/// String index for description of this configuration
		uint8_t iConfiguration;		// user
		/// Configuration attributes: self-powered or not, can remote wakeup or not
		uint8_t bmAttributes;		// computed from user
		/// Maximum bus power taken. Should be 0 for self powered device, otherwise it uses units of 2mA
		uint8_t bMaxPower;		// computed
	};
	/// Constructor helper for Configuration.
#define USB_BUILD_CONFIGURATION(total_size, num_interfaces, configuration_value, configuration, can_wakeup, max_power) \
	Usb::Configuration { \
		bLength: sizeof(Usb::Configuration), \
		bDescriptorType: Usb::TYPE_CONFIGURATION, \
		wTotalLength: total_size, \
		bNumInterfaces: num_interfaces, \
		bConfigurationValue: configuration_value, \
		iConfiguration: configuration, \
		bmAttributes: 0x80 | (max_power == 0 ? 0x40 : 0) | (can_wakeup ? 0x20 : 0), \
		bMaxPower: max_power \
	}
	/// Alternative to USB_BUILD_CONFIGURATION with reasonable defaults filled in.
#define USB_SIMPLE_CONFIGURATION(total_size) USB_BUILD_CONFIGURATION(total_size, 1, 1, 0, USB_CAN_WAKEUP_VALUE, USB_MAX_POWER)
// }}}

	/// Create a list of configurations for the case that there is only one configuration.
#define USB_SIMPLE_SINGLE_CONFIGURATION(name) Usb::Configuration const *const usb_configuration[1] PROGMEM = { reinterpret_cast <Usb::Configuration const *>(&name) };

	/// Device descriptor format, taken from USB standard.
	struct Device { // {{{
		/// Descriptor length
		uint8_t bLength;		// default
		/// Descriptor type: Usb::TYPE_DEVICE
		uint8_t bDescriptorType;	// default
		/// bcd encoded usb version (2.00 is 0x200)
		uint16_t bcdUSB;		// default
		/// Device class
		uint8_t bDeviceClass;		// default
		/// Device subclass
		uint8_t bDeviceSubClass;	// default
		/// Device protocol
		uint8_t bDeviceProtocol;	// default
		/// Maximum packet size for endpoint 0
		uint8_t bMaxPacketSize0;	// user
		/// Vendor ID; official values are assigned to USB members only
		uint16_t idVendor;		// user
		/// Product ID; assigned by owner of idVendor
		uint16_t idProduct;		// user
		/// bcd encoded device version
		uint16_t bcdDevice;		// user
		/// String index for manufacturer name
		uint8_t iManufacturer;		// user
		/// String index for product name
		uint8_t iProduct;		// user
		/// String index for serial number
		uint8_t iSerialNumber;		// user
		/// Number of configurations in the device
		uint8_t bNumConfigurations;	// user
	};
	/// Constructor helper for Device.
#define USB_BUILD_DEVICE(name, device_class, device_subclass, device_protocol, max_packet_size_0, device_version, manufacturer, product, serial_number, num_configurations) \
	Usb::Device const name PROGMEM = { \
		bLength: sizeof(Usb::Device), \
		bDescriptorType: Usb::TYPE_DEVICE, \
		bcdUSB: 0x0200, \
		bDeviceClass: device_class, \
		bDeviceSubClass: device_subclass, \
		bDeviceProtocol: device_protocol, \
		bMaxPacketSize0: max_packet_size_0, \
		idVendor: USB_VENDOR, \
		idProduct: USB_PRODUCT, \
		bcdDevice: device_version, \
		iManufacturer: manufacturer, \
		iProduct: product, \
		iSerialNumber: serial_number, \
		bNumConfigurations: num_configurations \
	}
	/// Alternative to USB_BUILD_DEVICE with reasonable defaults filled in.
#define USB_SIMPLE_DEVICE() USB_BUILD_DEVICE(usb_device, 0, 0, 0, 8, 0, 0, 0, 0, 1)
// }}}

	/// String descriptor format, taken from USB standard.
	struct String { // {{{
		/// Descriptor length
		uint8_t bLength;
		/// Descriptor type: Usb::TYPE_STRING
		uint8_t bDescriptorType;
		/// String data (variable length).
		uint16_t bString[0];
	};
	/// Constructor helper for String.
#define USB_BUILD_STRING(name, length, ...) \
	struct { \
		uint8_t bLength; \
		uint8_t bDescriptorType; \
		uint16_t bString[length]; \
	} const \
	_avr_notype_ ## name PROGMEM { \
		bLength: 2 + 2 * length, \
		bDescriptorType: Usb::TYPE_STRING, \
		bString: __VA_ARGS__ \
	}; \
	Usb::String const *const name = reinterpret_cast <Usb::String const *>(&_avr_notype_ ## name)
	// }}}

	// Interface Association. {{{
	/// Interface Association descriptor format, taken from USB Interface Association standard.
	struct USBIA {
		/// Descriptor length
		uint8_t bLength;
		/// Descriptor type: 0xb
		uint8_t bDescriptorType;
		/// First interface that is part of the association
		uint8_t bFirstInterface;
		/// Number of interfaces in the association
		uint8_t bInterfaceCount;
		/// Class of the association (same values as bClass in Device descriptor)
		uint8_t bFunctionClass;
		/// Subclass of the association (same values as bSubClass in Device descriptor)
		uint8_t bFunctionSubClass;
		/// Protocol of the association (same values as bProtocol in Device descriptor)
		uint8_t bFunctionProtocol;
		/// String index describing the association
		uint8_t iFunction;
	};
	/// Constructor helper for Interface Association.
#define USB_BUILD_IA(interface, num_interfaces, iaclass, iasubclass, iaprotocol) \
	Usb::USBIA { \
		bLength: sizeof(Usb::USBIA), \
		bDescriptorType: 0xb, \
		bFirstInterface: interface, \
		bInterfaceCount: num_interfaces, \
		bFunctionClass: iaclass, \
		bFunctionSubClass: iasubclass, \
		bFunctionProtocol: iaprotocol, \
		iFunction: 0 \
	}
	/// Alternative to USB_BUILD_DEVICE for Interface Association.
#define USB_BUILD_IA_DEVICE(name, max_packet_size_0, device_version, manufacturer, product, serial_number, num_configurations) USB_BUILD_DEVICE(name, 0xef, 2, 1, max_packet_size_0, device_version, manufacturer, product, serial_number, num_configurations)
	/// Alternative to USB_SIMPLE_DEVICE for Interface Association.
#define USB_SIMPLE_IA_DEVICE() USB_BUILD_IA_DEVICE(usb_device, 8, 0, 0, 0, 0, 1);
	// }}}

	// CDC structs. {{{
	/// Descriptor subtypes for CDC descriptors.
	enum CDCSubtypes {
		/// CDC header type
		CDC_HEADER = 0,
		/// CDC Call Management type
		CDC_CALL_MANAGEMENT = 1,
		/// CDC Abstract Control Model type
		CDC_ACM = 2,
		/// CDC Union type
		CDC_UNION = 6
	};

	/// Descriptor types for CDC descriptors.
	enum CDCDescriptorType {
		/// Class Specific Interface type
		CS_INTERFACE = 0x24,
		/// Class Specific Endpoint type
		CS_ENDPOINT = 0x25
	};

	/// CDC Header descriptor.
	struct CDCHeader {
		/// Descriptor length
		uint8_t bFunctionLength;
		/// Descriptor type: Usb::CS_INTERFACE
		uint8_t bDescriptorType;
		/// Descriptor subtype: Usb::CDC_HEADER
		uint8_t bDescriptorSubtype;
		/// bcd encoded version of CDC that is implemented
		uint16_t bcdCDC;
	};
	/// Constructor helper for CDCHeader.
#define USB_BUILD_CDC_HEADER() \
	Usb::CDCHeader { \
		bFunctionLength: sizeof(Usb::CDCHeader), \
		bDescriptorType: Usb::CS_INTERFACE, \
		bDescriptorSubtype: Usb::CDC_HEADER, \
		bcdCDC: 0x110 \
	}

	/// CDC Union descriptor.
	struct CDCUnion {
		/// Descriptor length
		uint8_t bFunctionLength;
		/// Descriptor type: Usb::CS_INTERFACE
		uint8_t bDescriptorType;
		/// Descriptor subtype: Usb::CDC_UNION
		uint8_t bDescriptorSubtype;
		/// CDC control interface
		uint8_t bControlInterface;
		/// CDC data inderface
		uint8_t bSubordinateInterface;
	};
	/// Constructor helper for CDCUnion.
#define USB_BUILD_CDC_UNION(control, subordinate) \
	Usb::CDCUnion { \
		bFunctionLength: sizeof(Usb::CDCUnion), \
		bDescriptorType: Usb::CS_INTERFACE, \
		bDescriptorSubtype: Usb::CDC_UNION, \
		bControlInterface: control, \
		bSubordinateInterface: subordinate \
	}

	/// CDC Call Management descriptor.
	struct CDCCallManagement {
		/// Descriptor length
		uint8_t bFunctionLength;
		/// Descriptor type: Usb::CS_INTERFACE
		uint8_t bDescriptorType;
		/// Descriptor subtype: Usb::CDC_CALL_MANAGEMENT
		uint8_t bDescriptorSubtype;
		/// Bitmask of control capabilities
		/**
		 * Bit 0: Device handles call management (1) or not (0)
		 * Bit 1: Device only uses control interface for call management (0) or it can use either control or data interface (1)
		 * All other bits must always be 0.
		 */
		uint8_t bmCapabilities;
		/// Interface number of data interface
		uint8_t bDataInterface;
	};
	/// Constructor helper for CDCCallManagement.
#define USB_BUILD_CDC_CALL_MANAGEMENT(capabilities, datainterface) \
	Usb::CDCCallManagement { \
		bFunctionLength: sizeof(Usb::CDCCallManagement), \
		bDescriptorType: Usb::CS_INTERFACE, \
		bDescriptorSubtype: Usb::CDC_CALL_MANAGEMENT, \
		bmCapabilities: capabilities, \
		bDataInterface: datainterface \
	}

	/// CDC ACM descriptor.
	struct CDCACM {
		/// Descriptor length
		uint8_t bFunctionLength;
		/// Descriptor type: Usb::CS_INTERFACE
		uint8_t bDescriptorType;
		/// Descriptor subtype: Usb::CDC_ACM
		uint8_t bDescriptorSubtype;
		/// Bitmask of ACM capabilities
		/**
		 * Bit 0: If set, support SET_COMM_FEATURE, CLEAR_COMM_FEATURE and GET_COMM_FEATURE.
		 * Bit 1: If set, support SET_LINE_CODING, SET_CONTROL_LINE_STATE, GET_LINE_CODING and SERIAL_STATE notifications.
		 * Bit 2: If set, support SEND_BREAK.
		 * Bit 3: If set, support NETWORK_CONNECTION notifications.
		 * All other bits must always be 0.
		 */
		uint8_t bmCapabilities;
	};
	/// Constructor helper for CDCACM.
#define USB_BUILD_CDC_ACM(capabilities) \
	Usb::CDCACM { \
		bFunctionLength: sizeof(Usb::CDCACM), \
		bDescriptorType: Usb::CS_INTERFACE, \
		bDescriptorSubtype: Usb::CDC_ACM, \
		bmCapabilities: capabilities, \
	}

	/// Constructor helper for full configuration of a CDC device.
#define USB_SIMPLE_CDC_CONFIGURATION(name) \
struct { \
	Usb::Configuration configuration; \
	Usb::USBIA usb_ia; \
	Usb::Interface manage_interface; \
	Usb::CDCHeader cdc_header; \
	Usb::CDCCallManagement cdc_call_management; \
	Usb::CDCACM cdc_acm; \
	Usb::CDCUnion cdc_union; \
	Usb::Endpoint ep_notify; \
	Usb::Interface data_interface; \
	Usb::Endpoint ep_out, ep_in; \
} const name PROGMEM = { \
	configuration: USB_BUILD_CONFIGURATION(sizeof(test_configuration), 2, 1, 0, USB_CAN_WAKEUP_VALUE, USB_MAX_POWER), \
	usb_ia: USB_BUILD_IA(0, 2, 2, 2, 0), \
	manage_interface: USB_BUILD_INTERFACE(0, 0, 1, 0x2, 2, 0, 0), \
	cdc_header: USB_BUILD_CDC_HEADER(), \
	cdc_call_management: USB_BUILD_CDC_CALL_MANAGEMENT(1, 0), \
	cdc_acm: USB_BUILD_CDC_ACM(0), \
	cdc_union: USB_BUILD_CDC_UNION(0, 1), \
	ep_notify: USB_SIMPLE_ENDPOINT(0x81, Usb::INTERRUPT), \
	data_interface: USB_BUILD_INTERFACE(1, 0, 2, 0xa, 0, 0, 0), \
	ep_out: USB_SIMPLE_ENDPOINT(0x02, Usb::BULK), \
	ep_in: USB_SIMPLE_ENDPOINT(0x83, Usb::BULK) \
}
// }}}

/// @cond
#define _AVR_USB_RX_BULK_OUT(ep) /* {{{ */ \
	do { \
		udbg("rx bulk out ##", UEBCHX, UEBCLX); \
		bool stop = false; \
		while (!stop && (UEINTX & _BV(RXOUTI))) { \
			USB_CLEAR_OUT_INT(RXOUTI); \
			while (UEINTX & _BV(RWAL)) { \
				uint8_t data = UEDATX; \
				if (!Usb::rx ## ep ## _write(data)) { \
					stop = true; \
					udbg("stop rx #", ep); \
					Usb::disable_rxouti(); \
					break; \
				} \
			} \
			if (!stop) \
				USB_CLEAR_OUT_INT(FIFOCON); \
		} \
		udbg("done rx #", ep); \
	} while (false) \
/* }}} */
#define _AVR_USB_RX_INTERRUPT_OUT(ep, size) /* {{{ */ \
	do { \
		uint16_t count = (UEBCHX << 8) | UEBCLX; \
		udbg("rx interrupt out *", count); \
		while (UEINTX & _BV(RXOUTI)) { \
			if (Usb::rx ## ep ## _buffer_available() < count) { \
				Usb::disable_rxouti(); /* No room in buffer to store the packet. */ \
				break; \
			} \
			bool last = count < size; \
			USB_CLEAR_OUT_INT(RXOUTI); \
			while (UEINTX & _BV(RWAL)) { \
				uint8_t data = UEDATX; \
				Usb::rx ## ep ## _write(data); \
			} \
			USB_CLEAR_OUT_INT(FIFOCON); \
			if (last) \
				Usb::rx ## ep ## _end(); \
		} \
		udbg("done rx #", ep); \
	} while (false) \
/* }}} */

#define _AVR_USB_TX_BULK_IN(ep) /* {{{ */ \
	do { \
		bool stop = false; \
		while (!stop && (UEINTX & _BV(TXINI))) { \
			USB_CLEAR_IN_INT(TXINI); \
			while (UEINTX & _BV(RWAL)) { \
				if (Usb::tx ## ep ## _buffer_used() == 0) { \
					udbg("disable #", ep); \
					Usb::disable_txini(); \
					stop = true; \
					break; \
				} \
				uint8_t c = Usb::tx ## ep ## _read(0); \
				Usb::tx ## ep ## _pop(1); \
				/*dbg_char(c);*/ \
				UEDATX = c; \
			} \
			udbg("flip tx #: #", ep, UEINTX); \
			USB_CLEAR_IN_INT(FIFOCON); \
		} \
		udbg("done tx #", ep); \
	} while (false) \
/* }}} */
#define _AVR_USB_TX_INTERRUPT_IN(ep) /* {{{ */ \
	do { \
		while (Usb::tx ## ep ## _first_packet != Usb::tx ## ep ## _last_packet && UEINTX & _BV(TXINI)) { \
			USB_CLEAR_IN_INT(TXINI); \
			uint8_t n = 0; \
			bool next = false; \
			while (UEINTX & _BV(RWAL)) { \
				if (Usb::tx ## ep ## _packet_length() <= n) { \
					USB_CLEAR_IN_INT(FIFOCON); \
					udbg("pop"); \
					Usb::tx ## ep ## _pop(); \
					next = true; \
					break; \
				} \
				uint8_t c = Usb::tx ## ep ## _read(n++); \
				udbg("#\\", c); \
				UEDATX = c; \
			} \
			if (next) \
				continue; \
			udbg("partial pop #", n); \
			Usb::tx ## ep ## _partial_pop(n); \
			USB_CLEAR_IN_INT(FIFOCON); \
		} \
		udbg("done tx #", ep); \
		if (Usb::tx ## ep ## _first_packet == Usb::tx ## ep ## _last_packet) \
			Usb::disable_txini(); \
	} while (false) \
/* }}} */
/// @endcond

} // namespace Usb
// }}}

// These need to be defined in user code. {{{
// Constants:
// USB_VENDOR: Vendor ID for this device. Defaults to 0xfffe.
// USB_PRODUCT: Product ID for this device. Defaults to 0xffff.
// USB_NUM_LANGS: Number of languages for strings. Defaults to 1.
// USB_NUM_STRINGS: Number of strings. Defaults to 0.
// USB_MAX_INTERFACES: Maximum number of interfaces for any defined configuration. Defaults to 1.
// USB_DOUBLE_BANK(configuration): Bit mask specifying which endpoinds have double memory banks. Defaults to 0.
// USB_LOW_SPEED: If this is defined, device will be created as low speed; otherwise as full speed.

/// Device descriptor (in PROGMEM).
/** This should be created using the macro:
 * USB_BUILD_DEVICE(name, max_packet_size_0, device_version, manufacturer, product, serial_number, num_configurations);
 * This defines a Usb::Device named "name" in PROGMEM.
 * To use defaults for everything, you can instead use:
 * USB_SIMPLE_DEVICE(); (or USB_SIMPLE_IA_DEVICE(); if you want interface association.)
 */
extern Usb::Device const usb_device PROGMEM;

#ifdef DOXYGEN
/// Array of pointers to Configuration descriptors. Both the pointers and the descriptors they point to are in PROGMEM. {{{
/** The Configuration descriptor should be created like this:
 *
 * struct {
 *	Usb::Configuration configuration;
 *	Usb::Interface interface;
 *	Usb::Endpoint ep1, ep2;
 * } const test_configuration PROGMEM = {
 *	configuration: USB_SIMPLE_CONFIGURATION(sizeof(test_configuration)),
 *	interface: USB_SIMPLE_INTERFACE(0, 2),
 *	ep1: USB_SIMPLE_ENDPOINT(0x81, Usb::INTERRUPT),
 *	ep2: USB_SIMPLE_ENDPOINT(0x02, Usb::INTERRUPT)
 * };
 *
 * To use explicit values for all the settings, use these instead of the USB_SIMPLE_* versions:
 * USB_BUILD_CONFIGURATION(total_size, num_interfaces, configuration_value, configuration, can_wakeup, max_power)
 * USB_BUILD_INTERFACE(id, alternate, num_endpoints, interface_class, interface_subclass, interface_protocol, interface)
 * USB_BUILD_ENDPOINT(address, attributes, max_packet_size, interval)
 *
 * For HID devices, use CLASS_HID for the interface. To use defaults for everything else, use USB_SIMPLE_HID_INTERFACE(id, endpoints) instead of USB_SIMPLE_INTERFACE.
 *
 * For a HID interface, a HID descriptor needs to be placed immediately after the Interface descriptor. To create a HID descriptor, use:
 * USB_BUILD_HID(type_name, country, report_length, num_extra_descriptors, ...)
 * If you only need one report descriptors (num_extra_descriptors == 0), then:
 * * there are no extra parameters to USB_BUILD_HID.
 * * type_name should be set to Usb::HIDSimple.
 * To use all defaults, you can create a HID descriptor using:
 * USB_SIMPLE_HID(report_length)
 *
 * Otherwise:
 * * Create a custom type using:
 *	 USB_BUILD_HID_TYPE(name, num_descriptors);
 * * Use the name as type_name in USB_BUILD_HID.
 * * Add { descriptor_type, descriptor length } parameters to USB_BUILD_HID, one per extra descriptor.
 * 	The descriptor_type is HID_REPORT for a report descriptor.
 *
 * A Configuration with a HID interface looks like:
 * struct my_report {
 *	// report definition.
 * };
 * struct {
 *	Usb::Configuration configuration;
 *	Usb::Interface interface;
 *	Usb::HIDSimple hid;
 *	Usb::Endpoint ep1, ep2;
 *	my_report report;
 * } const test_configuration PROGMEM = {
 *	configuration: USB_SIMPLE_CONFIGURATION(sizeof(test_configuration)),
 *	interface: USB_SIMPLE_HID_INTERFACE(0, 1),
 *	hid: USB_SIMPLE_HID(sizeof(my_report)),
 *	ep1: USB_SIMPLE_ENDPOINT(0x81, Usb::INTERRUPT),
 *	ep2: USB_SIMPLE_ENDPOINT(0x02, Usb::INTERRUPT),
 *	report: { report initializers. }
 * };
 */
extern Usb::Configuration const *const usb_configuration[];
#endif
/// @cond
extern Usb::Configuration const *const usb_configuration[] PROGMEM;
/// @endcond
// }}}

#if (USB_NUM_STRINGS > 0)
#ifdef DOXYGEN
/// Array of arrays to pointers to strings.
/** usb_string[lang_idx] is an array of strings for language with index lang_idx (this is not the language code; use string 0 to find that).
 * usb_string[lang_idx][string_idx] is a pointer to the string.
 *
 * This is only defined if USB_NUM_STRINGS > 0.
 */
extern Usb::String const *const usb_string[USB_NUM_LANGS][USB_NUM_STRINGS + 1];
#endif
/// @cond
extern Usb::String const *const usb_string[USB_NUM_LANGS][USB_NUM_STRINGS + 1] PROGMEM;
/// @endcond
#endif

/**
 * @name User defined functions
 *
 * Define CALL_usb_* when defining any of these functions.
 * @{
 */
// Endpoint-level events.
#ifdef CALL_usb_out
/// An OUT packet was received on any other endpoint.
static void usb_out();
#endif

#ifdef CALL_usb_nak_in
/// A NAK reply was sent to an IN request. This means the host asked for IN data.
static void usb_nak_in();
#endif

#ifdef CALL_usb_overflow
/// An overflow occurred on an isochronous endpoint.
static void usb_overflow();
#endif

#ifdef CALL_usb_underflow
/// An underflow occurred on an isochronous endpoint.
static void usb_underflow();
#endif

// Device-level events.
#ifdef CALL_usb_reset
/// The controller has reset and is ready (but set to configuration 0).
static void usb_reset();
#endif

#ifdef CALL_usb_configured
/// The controller been configured after SET_CONFIGURATION or SET_INTERFACE, and the configuration is not 0.
static void usb_configured();
#endif

#ifdef CALL_usb_suspended
/// The bus has been suspended due to bus inactivity. Clocks have been stopped. Remote wakeup is now possible (if active).
static void usb_suspended();
#endif

#ifdef CALL_usb_wakeup
/// There is activity on the line. This interrupt triggers all the time when not suspended.
static void usb_wakeup();
#endif

#ifdef CALL_usb_sof
/// Start of frame packet received (happens every ms).
static void usb_sof();
#endif

// Buffer receive callbacks. Calls to these function are only enabled when USB_RX*_{BULK|INTERRUPT} is defined, so they have no default implementation.
#ifdef USB_RX1_BULK
/// Handle BULK packets received on endpoint 1
/**
 * USB_RX1_BULK must be defined along with this function.
 *
 * Functions for endpoints 2-6 are also available.
 */
static void usb_recv1(uint8_t c, uint8_t len);
#endif
#ifdef USB_RX1_INTERRUPT
/// Handle INTERRUPT packets received on endpoint 1
/**
 * USB_RX1_INTERRUPT must be defined along with this function.
 *
 * Functions for endpoints 2-6 are also available.
 */
static void usb_recv1();
#endif
#ifdef USB_RX2_BULK
static void usb_recv2(uint8_t c, uint8_t len);
#elif defined(USB_RX2_INTERRUPT)
static void usb_recv2();
#endif
#if USB_RX3_BULK
static void usb_recv3(uint8_t c, uint8_t len);
#elif defined(USB_RX3_INTERRUPT)
static void usb_recv3();
#endif
#if USB_RX4_BULK
static void usb_recv4(uint8_t c, uint8_t len);
#elif defined(USB_RX4_INTERRUPT)
static void usb_recv4();
#endif
#if USB_RX5_BULK
static void usb_recv5(uint8_t c, uint8_t len);
#elif defined(USB_RX5_INTERRUPT)
static void usb_recv5();
#endif
#if USB_RX6_BULK
static void usb_recv6(uint8_t c, uint8_t len);
#elif defined(USB_RX6_INTERRUPT)
static void usb_recv6();
#endif

// These are unlikely to be useful, but are available for special configurations:
// Endpoint-level events.

#ifdef CALL_usb_get_descriptor
/// An unhandled GET_DESCRIPTOR packet was received on endpoint 0. Data is in Usb::setup.
static void usb_get_descriptor();
#endif

#ifdef CALL_usb_standard_setup0
/// An unhandled standard setup packet was received on endpoint 0. Data is in Usb::setup.
static void usb_standard_setup0();
#endif

#ifdef CALL_usb_setup0
/// An unhandled non-standard setup packet was received on endpoint 0. Data is in Usb::setup.
static void usb_setup0();
#endif

#ifdef CALL_usb_setup
/// A setup packet was received on any other endpoint. Data is in Usb::setup.
static void usb_setup();
#endif

#ifdef CALL_usb_out0
/// A non-standard OUT packet was received on endpoint 0.
static void usb_out0();
#endif

#ifdef CALL_usb_in0
/// A non-standard IN packet may be prepared for endpoint 0.
static void usb_in0();
#endif

#ifdef CALL_usb_in
/// An IN packet may be prepared for any other endpoint.
static void usb_in();
#endif

#ifdef CALL_usb_stalled
/// A STALL reply has been sent to the host.
static void usb_stalled();
#endif

#ifdef CALL_usb_nak_out
/// A NAK reply was sent to an OUT request. This means the host attempted to send OUT data.
static void usb_nak_out();
#endif

// Device-level events.
#ifdef CALL_usb_deconfigured
/// The controller been deconfigured after SET_CONFIGURATION(0).
static void usb_deconfigured();
#endif

#ifdef CALL_usb_resuming
/// Resume has been requested by the controller.
static void usb_resuming();
#endif

#ifdef CALL_usb_resumed
/// Resume request has completed; communication with host has resumed.
static void usb_resumed();
#endif

#ifdef CALL_usb_pll_wait
/// The PLL needs "several ms" to lock. This function is called repeatedly while waiting for that.
/**
 * The avr devices need to wait for the PLL lock before enabling the USB
 * interface. Unfortunately, there is no interrupt to signal that the PLL is
 * locked. Because of this, the only solution is a poll or a busy wait loop.
 * According to the datasheet, it should take "several ms". This code should
 * not interfere with the other parts of the device, so polling on a timer is
 * not an option. For the case where the device should not be idle during this
 * wait, the code calls this function repeatedly until the PLL is locked.
 *
 * This function is probably never used by anyone; waiting for a few ms during
 * USB startup is pretty much always acceptable.
 */
static void usb_pll_wait();
#endif

// The above functions are only called when their interrupt is enabled.
// Interrupts for endpoint 0 may be enabled at any time by the standard
// request handling and must not be disabled by user code.

/// Convenience macro for responding to a request with a STALL reply.
#define USB_STALL do { udbg("Stall *", __LINE__); UECONX |= _BV(STALLRQ); } while (false)

/// @}
// }}}

/// Universal Serial Bus
namespace Usb {
	// Internals. {{{
/// @cond
	// State for handling SETUP packets. {{{
	static uint8_t setup_size;
	static Setup setup;
	static bool remote_wakeup_active;
	static uint16_t progmem_in_pending;
	static uint8_t const *progmem_in_pointer;
	static uint8_t current_configuration;
#if USB_MAX_INTERFACES > 1
	static uint8_t current_interface[USB_MAX_INTERFACES];
#endif
	// }}}

	static inline void enable_PLL() { // {{{
		// Configure PLL interface.
		PLLFRQ = (0xa << PDIV0)
			|(2 << PLLTM0)
			| _BV(PLLUSB)
#if F_CPU != 8000000 && F_CPU != 16000000
			| _BV(PINMUX)
#endif
		;
		// Enable PLL.
#if F_CPU == 16000000
		PLLCSR = _BV(PLLE) | _BV(PINDIV);
#else
		PLLCSR = _BV(PLLE);
#endif
		// Check PLL lock. This should use an interrupt, but there is none available for the event.
		// If this busy wait is really unacceptable, an idle function can be defined to handle any events.
		while (!(PLLCSR & _BV(PLOCK))) {
#ifdef CALL_usb_pll_wait
			usb_pll_wait();
#endif
		}
	} // }}}

	static inline void setup_ep0() { // {{{
		UENUM = 0;
		UECONX = _BV(RSTDT) | _BV(EPEN);
		UECFG0X = (CONTROL << EPTYPE0) | (OUT << EPDIR);
		setup_size = pgm_read_byte(&usb_device.bMaxPacketSize0);
		uint8_t double_bank = (USB_DOUBLE_BANK(0) >> 0) & 1;
		uint8_t size_code = (setup_size <= 8 ? 0 : setup_size <= 16 ? 1 : setup_size <= 32 ? 2 : 3);
		UECFG1X = (size_code << EPSIZE0) | (double_bank << EPBK0) | _BV(ALLOC);
		UERST &= ~(1 << 0);
	} // }}}

	static inline bool find_interfaces(Interface const **ret, Configuration const *configuration, uint8_t num_interfaces, bool default_interfaces) { // {{{
		uint8_t length = pgm_read_byte(&configuration->bLength);
		bool found[num_interfaces] = {false,};
		uint16_t total_length = pgm_read_word(&configuration->wTotalLength);
		uint8_t const *end = &reinterpret_cast <uint8_t const *>(configuration)[total_length];
		uint8_t const *ptr = &reinterpret_cast <uint8_t const *>(configuration)[length];
		while (ptr < end) {
			// ptr is now pointing to the next descriptor.
			Usb::Interface const *interface = reinterpret_cast <Usb::Interface const *>(ptr);
			length = pgm_read_byte(&interface->bLength);
			uint8_t descriptor_type = pgm_read_byte(&interface->bDescriptorType);
			if (descriptor_type == Usb::TYPE_INTERFACE) {
				// This is what we're looking for; fill in the return data.
				uint8_t interface_id = pgm_read_byte(&interface->bInterfaceNumber);
#if USB_MAX_INTERFACES > 1
				uint8_t alternate_setting = pgm_read_byte(&interface->bAlternateSetting);
				if (alternate_setting == (default_interfaces ? 0 : current_interface[interface_id])) {
					ret[interface_id] = interface;
					found[interface_id] = true;
				}
#else
				// With only one interface option, ignore the alternate setting.
				(void)&default_interfaces;
				ret[interface_id] = interface;
				found[interface_id] = true;
#endif
			}
			// Continue to the next descriptor.
			length = pgm_read_byte(&interface->bLength);
			ptr = reinterpret_cast <uint8_t const *>(reinterpret_cast <uint8_t const *>(interface) + length);
		}
		if (ptr > end)
			return false;
		for (uint8_t i = 0; i < num_interfaces; ++i) {
			if (!found[i])
				return false;
		}
		return true;
	} // }}}
/// @endcond

	// Endpoint functions. Should be called through WITH_ENDPOINT or with similar protection. {{{
	/// Convenience function for endpoint operations.
	/**
	 * This macro disables interrupts, sets the current endpoint to ep,
	 * runs code, restores the current endpoint and restores the interrupt
	 * state (enabled or disabled).
	 *
	 * It should be used when handling endpoints from places where
	 * interrupts may be enabled.
	 */
#define WITH_ENDPOINT(ep, code) \
	do { \
		uint8_t sreg = SREG; \
		cli(); \
		uint8_t uenum = UENUM; \
		UENUM = ep; \
		code \
		UENUM = uenum; \
		SREG = sreg; \
	} while (false)

	/// Enable interrupt for IN on active endpoint.
	static inline void enable_txini() { (Usb::in_interrupt ? Usb::saved_ueienx[UENUM] : UEIENX) |= _BV(TXINE); }
	/// Enable interrupt for OUT on active endpoint.
	static inline void enable_rxouti() { (Usb::in_interrupt ? Usb::saved_ueienx[UENUM] : UEIENX) |= _BV(RXOUTE); }
	/// Enable interrupt for SETUP on active endpoint.
	static inline void enable_rxstpi() { (Usb::in_interrupt ? Usb::saved_ueienx[UENUM] : UEIENX) |= _BV(RXSTPE); }
	/// Disable interrupt for IN on active endpoint.
	static inline void disable_txini() { (Usb::in_interrupt ? Usb::saved_ueienx[UENUM] : UEIENX) &= ~_BV(TXINE); }
	/// Disable interrupt for OUT on active endpoint.
	static inline void disable_rxouti() { (Usb::in_interrupt ? Usb::saved_ueienx[UENUM] : UEIENX) &= ~_BV(RXOUTE); }
	/// Disable interrupt for SETUP on active endpoint.
	static inline void disable_rxstpi() { (Usb::in_interrupt ? Usb::saved_ueienx[UENUM] : UEIENX) &= ~_BV(RXSTPE); }
	// }}}

/// @cond
#ifndef USB_RX1_INTERRUPT_PACKETS
#define USB_RX1_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_RX2_INTERRUPT_PACKETS
#define USB_RX2_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_RX3_INTERRUPT_PACKETS
#define USB_RX3_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_RX4_INTERRUPT_PACKETS
#define USB_RX4_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_RX5_INTERRUPT_PACKETS
#define USB_RX5_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_RX6_INTERRUPT_PACKETS
#define USB_RX6_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_TX1_INTERRUPT_PACKETS
#define USB_TX1_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_TX2_INTERRUPT_PACKETS
#define USB_TX2_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_TX3_INTERRUPT_PACKETS
#define USB_TX3_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_TX4_INTERRUPT_PACKETS
#define USB_TX4_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_TX5_INTERRUPT_PACKETS
#define USB_TX5_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifndef USB_TX6_INTERRUPT_PACKETS
#define USB_TX6_INTERRUPT_PACKETS USB_DEFAULT_PACKETS
#endif

#ifdef USB_RX1_BULK
	STREAM_BUFFER_WITH_CBS(rx1, (USB_RX1_BULK), usb_recv1, WITH_ENDPOINT(1, enable_rxouti();););
#elif defined(USB_RX1_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(rx1, (USB_RX1_INTERRUPT), (USB_RX1_INTERRUPT_PACKETS), usb_recv1();, WITH_ENDPOINT(1, enable_rxouti();););
#endif
#ifdef USB_RX2_BULK
	STREAM_BUFFER_WITH_CBS(rx2, (USB_RX2_BULK), usb_recv2, WITH_ENDPOINT(2, enable_rxouti();););
#elif defined(USB_RX2_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(rx2, (USB_RX2_INTERRUPT), (USB_RX2_INTERRUPT_PACKETS), usb_recv2();, WITH_ENDPOINT(2, enable_rxouti();););
#endif
#ifdef USB_RX3_BULK
	STREAM_BUFFER_WITH_CBS(rx3, (USB_RX3_BULK), usb_recv3, WITH_ENDPOINT(3, enable_rxouti();););
#elif defined(USB_RX3_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(rx3, (USB_RX3_INTERRUPT), (USB_RX3_INTERRUPT_PACKETS), usb_recv3();, WITH_ENDPOINT(3, enable_rxouti();););
#endif
#ifdef USB_RX4_BULK
	STREAM_BUFFER_WITH_CBS(rx4, (USB_RX4_BULK), usb_recv4, WITH_ENDPOINT(4, enable_rxouti();););
#elif defined(USB_RX4_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(rx4, (USB_RX4_INTERRUPT), (USB_RX4_INTERRUPT_PACKETS), usb_recv4();, WITH_ENDPOINT(4, enable_rxouti();););
#endif
#ifdef USB_RX5_BULK
	STREAM_BUFFER_WITH_CBS(rx5, (USB_RX5_BULK), usb_recv5, WITH_ENDPOINT(5, enable_rxouti();););
#elif defined(USB_RX5_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(rx5, (USB_RX5_INTERRUPT), (USB_RX5_INTERRUPT_PACKETS), usb_recv5();, WITH_ENDPOINT(5, enable_rxouti();););
#endif
#ifdef USB_RX6_BULK
	STREAM_BUFFER_WITH_CBS(rx6, (USB_RX6_BULK), usb_recv6, WITH_ENDPOINT(6, enable_rxouti();););
#elif defined(USB_RX6_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(rx6, (USB_RX6_INTERRUPT), (USB_RX6_INTERRUPT_PACKETS), usb_recv6();, WITH_ENDPOINT(6, enable_rxouti();););
#endif

#ifdef USB_TX1_BULK
	STREAM_BUFFER_WITH_CBS(tx1, (USB_TX1_BULK), WITH_ENDPOINT(1, enable_txini(););_AVR_NOP,);
#elif defined(USB_TX1_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(tx1, (USB_TX1_INTERRUPT), (USB_TX1_INTERRUPT_PACKETS), WITH_ENDPOINT(1, enable_txini(););,);
#endif
#ifdef USB_TX2_BULK
	STREAM_BUFFER_WITH_CBS(tx2, (USB_TX2_BULK), WITH_ENDPOINT(2, enable_txini(););_AVR_NOP,);
#elif defined(USB_TX2_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(tx2, (USB_TX2_INTERRUPT), (USB_TX2_INTERRUPT_PACKETS), WITH_ENDPOINT(2, enable_txini(););,);
#endif
#ifdef USB_TX3_BULK
	STREAM_BUFFER_WITH_CBS(tx3, (USB_TX3_BULK), WITH_ENDPOINT(3, enable_txini(););_AVR_NOP,);
#elif defined(USB_TX3_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(tx3, (USB_TX3_INTERRUPT), (USB_TX3_INTERRUPT_PACKETS), WITH_ENDPOINT(3, enable_txini(););,);
#endif
#ifdef USB_TX4_BULK
	STREAM_BUFFER_WITH_CBS(tx4, (USB_TX4_BULK), WITH_ENDPOINT(4, enable_txini(););_AVR_NOP,);
#elif defined(USB_TX4_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(tx4, (USB_TX4_INTERRUPT), (USB_TX4_INTERRUPT_PACKETS), WITH_ENDPOINT(4, enable_txini(););,);
#endif
#ifdef USB_TX5_BULK
	STREAM_BUFFER_WITH_CBS(tx5, (USB_TX5_BULK), WITH_ENDPOINT(5, enable_txini(););_AVR_NOP,);
#elif defined(USB_TX5_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(tx5, (USB_TX5_INTERRUPT), (USB_TX5_INTERRUPT_PACKETS), WITH_ENDPOINT(5, enable_txini(););,);
#endif
#ifdef USB_TX6_BULK
	STREAM_BUFFER_WITH_CBS(tx6, (USB_TX6_BULK), WITH_ENDPOINT(6, enable_txini(););_AVR_NOP,);
#elif defined(USB_TX6_INTERRUPT)
	PACKET_BUFFER_WITH_CBS(tx6, (USB_TX6_INTERRUPT), (USB_TX6_INTERRUPT_PACKETS), WITH_ENDPOINT(6, enable_txini(););,);
#endif

	static inline bool configure_device(bool default_interfaces) { // {{{
		// Internal function, called by SET_CONFIGURATION and SET_INTERFACE. Interrupts are disabled when this is called.
		// Return true if it worked, false if settings were wrong.
		Configuration const *configuration = reinterpret_cast <Configuration const *>(pgm_read_word(&usb_configuration[current_configuration - 1]));
		uint8_t num_interfaces = pgm_read_byte(&configuration->bNumInterfaces);
		Interface const *interfaces[num_interfaces];
		if (!find_interfaces(interfaces, configuration, num_interfaces, default_interfaces))
			return false;
		// What needs to be set up:
		// - Endpoints: address, direction, type, memory allocation.
		// First clear all endpoint data and free all memory.
		for (uint8_t e = 1; e < 7; ++e) {
			UENUM = e;
			UECFG1X = 0;
			UECONX = 0;	// reset EPEN.
		}
		// Reset all FIFOs
		UERST = 0x7e;
		// Now set up endpoints that are defined for all interfaces.
		uint8_t e = 1;
		for (uint8_t i = 0; i < num_interfaces; ++i) {
			uint8_t num_endpoints = pgm_read_byte(&interfaces[i]->bNumEndpoints);
			uint8_t length = pgm_read_byte(&interfaces[i]->bLength);
			Endpoint const *endpoint = reinterpret_cast <Endpoint const *>(&reinterpret_cast <uint8_t const *>(interfaces[i])[length]);
			for (uint8_t first_endpoint = e; e < first_endpoint + num_endpoints; ++e) {
				while (pgm_read_byte(&endpoint->bDescriptorType) != Usb::TYPE_ENDPOINT) {
					length = pgm_read_byte(&interfaces[i]->bLength);
					endpoint = reinterpret_cast <Endpoint const *>(&reinterpret_cast <uint8_t const *>(endpoint)[length]);
				}
				UENUM = e;
				uint16_t size = pgm_read_word(&endpoint->wMaxPacketSize);
				uint8_t addr = pgm_read_byte(&endpoint->bEndpointAddress);
				uint8_t attr = pgm_read_byte(&endpoint->bmAttributes);
				uint8_t type = attr & 3;
				uint8_t dir = (addr >> 7) & 1;
				uint8_t double_bank = (USB_DOUBLE_BANK(current_configuration) >> e) & 1;
				uint8_t size_code = (size <= 8 ? 0 : size <= 16 ? 1 : size <= 32 ? 2 : size <= 64 ? 3 : size <= 128 ? 4 : 5);
				UECONX = _BV(RSTDT) | _BV(EPEN);
				UECFG0X = (type << EPTYPE0) | (dir << EPDIR);
				UECFG1X = (size_code << EPSIZE0) | (double_bank << EPBK0) | _BV(ALLOC);
				udbg("setup ep# state #", e, UESTA0X);
				uint8_t ep_length = pgm_read_byte(&endpoint->bLength);
				endpoint = reinterpret_cast <Endpoint const *>(&reinterpret_cast <uint8_t const *>(endpoint)[ep_length]);
			}
		}
		UERST = 0;
#if defined(USB_RX1_BULK) || defined(USB_RX1_INTERRUPT)
		UENUM = 1;
		enable_rxouti();
		rx1_reset();
#endif
#if defined(USB_RX2_BULK) || defined(USB_RX2_INTERRUPT)
		UENUM = 2;
		enable_rxouti();
		rx2_reset();
#endif
#if defined(USB_RX3_BULK) || defined(USB_RX3_INTERRUPT)
		UENUM = 3;
		enable_rxouti();
		rx3_reset();
#endif
#if defined(USB_RX4_BULK) || defined(USB_RX4_INTERRUPT)
		UENUM = 4;
		enable_rxouti();
		rx4_reset();
#endif
#if defined(USB_RX5_BULK) || defined(USB_RX5_INTERRUPT)
		UENUM = 5;
		enable_rxouti();
		rx5_reset();
#endif
#if defined(USB_RX6_BULK) || defined(USB_RX6_INTERRUPT)
		UENUM = 6;
		enable_rxouti();
		rx6_reset();
#endif
#if defined(USB_TX1_BULK) || defined(USB_TX1_INTERRUPT)
		tx1_reset();
#endif
#if defined(USB_TX2_BULK) || defined(USB_TX2_INTERRUPT)
		tx2_reset();
#endif
#if defined(USB_TX3_BULK) || defined(USB_TX3_INTERRUPT)
		tx3_reset();
#endif
#if defined(USB_TX4_BULK) || defined(USB_TX4_INTERRUPT)
		tx4_reset();
#endif
#if defined(USB_TX5_BULK) || defined(USB_TX5_INTERRUPT)
		tx5_reset();
#endif
#if defined(USB_TX6_BULK) || defined(USB_TX6_INTERRUPT)
		tx6_reset();
#endif
#ifdef CALL_usb_configured
		usb_configured();
#endif
		UENUM = 0; // Calling code expects current endpoint to be 0.
		return true;
	} // }}}

	static inline void detect() { // {{{
		if (!(USBSTA & _BV(VBUS)))
			return;
		// VBUS is now active.
		// Attach USB device.
#ifdef USB_LOW_SPEED
		UDCON = _BV(LSM);
#else
		UDCON = 0;
#endif
		// Reset all endpoint fifos.
		UERST = 0x7f;
		// Enable "end of reset" interrupt.
		UDIEN = _BV(EORSTE);
	 } // }}}
	// }}}
/// @endcond

	/// Enable the USB system.
	/**
	 * This is normally the only USB function that needs to be called from
	 * the main function. All the rest is declared in variables and macros.
	 */
	static inline void enable() { // {{{
		// Disable all interrupts.
		UDIEN = 0;
		Usb::in_interrupt = false;
		for (uint8_t e = 0; e < 7; ++e)
			UEIENX = 0;
		// Power on USB pad regulator.
		UHWCON = _BV(UVREGE);
		USBCON = _BV(USBE) | _BV(OTGPADE) | _BV(FRZCLK);
		// Configure and enable PLL, wait for PLL lock.
		enable_PLL();
		// Enable USB interface.
		// (Configure USB interface.)
		// Wait for USB VBUS information connection.
		USBCON = _BV(USBE) | _BV(OTGPADE) | _BV(VBUSTE);
		detect(); // Force first "interrupt".
	} // }}}

	/// Disable the USB system.
	static inline void disable() { // {{{
		// Detach USB interface.
		UDCON = _BV(DETACH);
		// Disable USB interface.
		USBCON = 0;
		// Disable PLL.
		PLLCSR = 0;
		// Disable USB pad regulator.
		UHWCON = 0;
	} // }}}

	/// Initiate a remote wakeup.
	/**
	 * This is only allowed when the device is suspended and the host has
	 * configured this feature.
	 *
	 * Returns true if a wakeup event was sent to the host.
	 */
	static inline bool wakeup() { // {{{
		if (!(UDINT & _BV(SUSPI))) {
			// Cannot wake up unless suspended.
			return false;
		}
		if (!remote_wakeup_active)
			return false;
		UDCON |= _BV(RMWKUP);
		return true;
	} // }}}

#ifdef CALL_usb_hid_set_report
	PACKET_BUFFER_WITH_CBS(hid_set_report, USB_HID_SET_REPORT_SIZE, USB_HID_SET_REPORT_PACKETS, usb_hid_set_report();,)
#endif
} // namespace Usb

#ifdef _AVR_PRE_SETUP_USB
_AVR_PRE_SETUP_USB
#endif

/// @cond
ISR(USB_GEN_vect) { // {{{
	udbg("Usb General #", UDINT);
	uint8_t udint = UDINT;
	if (udint & _BV(UPRSMI)) {
		udbg("Upstream resume interrupt");
		UDINT &= ~_BV(UPRSMI);
		Usb::enable_PLL();
		// Unfreeze USB clock.
		USBCON &= ~_BV(FRZCLK);
		// Clear resume information.
		UDINT &= ~_BV(UPRSMI);
#ifdef CALL_usb_resuming
		if (UDIEN & _BV(UPRSME))
			usb_resuming();
#endif
	}
	if (udint & _BV(EORSMI)) {
		udbg("End of resume interrupt");
		UDINT &= ~_BV(EORSMI);
#ifdef CALL_usb_resumed
		if (UDIEN & _BV(EORSME))
			usb_resumed();
#endif
	}
	if (udint & _BV(WAKEUPI)) {
		udbg("wakeup interrupt");
		UDINT &= ~_BV(WAKEUPI);
#ifdef CALL_usb_wakeup
		if (UDIEN & _BV(WAKEUPE))
			usb_wakeup();
#endif
	}
	if (udint & _BV(EORSTI)) {
		udbg("End of reset interrupt");
		UDINT &= ~_BV(EORSTI);
		Usb::current_configuration = 0;
		Usb::progmem_in_pending = 0;
		Usb::remote_wakeup_active = false;
		// Enable default endpoint.
		Usb::setup_ep0();
		// Enable setup interrupt for default endpoint.
		UEIENX |= _BV(RXSTPE) | _BV(RXOUTE) | _BV(NAKOUTE) | _BV(NAKINE);
#ifdef CALL_usb_reset
		if (UDIEN & _BV(EORSTE))
			usb_reset();
#endif
	}
	if (udint & _BV(SOFI)) {
		udbg("start of frame interrupt");
		UDINT &= ~_BV(SOFI);
#ifdef CALL_usb_sof
		if (UDIEN & _BV(SOFE))
			usb_sof();
#endif
	}
	if (udint & _BV(SUSPI)) {
		udbg("suspend interrupt");
		// (Clear suspend bit.)
		//UDINT &= ~_BV(SUSPI);
		// Freeze USB clock.
		USBCON |= _BV(FRZCLK);
		// Disable PLL.
		PLLCSR = 0;
		// (Enable interrupts and sleep.)
#ifdef CALL_usb_suspended
		if (UDIEN & _BV(SUSPE))
			usb_suspended();
#endif
	}
	if (USBINT & _BV(VBUSTI)) {
		udbg("VBUS transition interrupt");
		USBINT &= ~_BV(VBUSTI);
		Usb::detect();
	}
} // }}}

ISR(USB_COM_vect) { // {{{
	udbg("Communication Int #", UEINT);
	// Save current EP to restore at the end.
	uint8_t saved_ep = UENUM;
	// Save and disable interrupt enable settings; enable non-USB interrupts.
	uint8_t saved_udien = UDIEN;
	bool saved_vbuste = USBCON & _BV(VBUSTE);
	uint8_t ueint = UEINT;
	for (uint8_t ep = 0; ep < 7; ++ep) {
		UENUM = ep;
		Usb::saved_ueienx[ep] = UEIENX;
		UEIENX = 0;
	}
	UDIEN = 0;
	USBCON &= ~_BV(VBUSTE);
	Usb::in_interrupt = true;
	sei();
	for (uint8_t ep = 0; ep < 7; ++ep) {
		if (!(ueint & (1 << ep)))
			continue;
		// This endpoint has an interrupt.
		// Possible sources:
		// UEINTX.TXINI = 1: ready to accept IN data.
		// UEINTX.RXOUTI = 1: received OUT data.
		// UEINTX.RXSTPI = 1: received SETUP data.

		// UEINTX.STALLEDI = 1: stalled packet
		// UESTA0X.OVERFI = 1: iso overflow
		// UESTA0X.UNDERFI = 1: iso underflow
		// UEINTX.NAKINI = 1: NAK IN sent
		// UEINTX.NAKOUTI = 1: NAK OUT sent
		UENUM = ep;
		udbg("EP # UEINTX # UEIENX #", ep, UEINTX, Usb::saved_ueienx[ep]);
		if (UEINTX & _BV(RXSTPI)) { // {{{
			// UEINTX.RXSTPI = 1: received SETUP data.
			if (ep != 0) {
				udbg("non-default setup");
#ifdef CALL_usb_setup
				usb_setup();
				UENUM = ep;
#endif
			}
			else {
				// SETUP on default control endpoint: handle standard requests.
				Usb::setup.request_type = UEDATX;
				Usb::setup.request = UEDATX;
				Usb::setup.value.b[0] = UEDATX;
				Usb::setup.value.b[1] = UEDATX;
				Usb::setup.index.b[0] = UEDATX;
				Usb::setup.index.b[1] = UEDATX;
				Usb::setup.length.b[0] = UEDATX;
				Usb::setup.length.b[1] = UEDATX;
				udbg("Default setup type # request # value * index * length *", Usb::setup.request_type, Usb::setup.request, Usb::setup.value.w, Usb::setup.index.w, Usb::setup.length.w);
				//if ((Usb::setup.request_type & 0x80) || Usb::setup.length.w == 0)
				USB_CLEAR_SETUP();
				if ((Usb::setup.request_type & 0x60) == 0) {
					uint8_t target = Usb::setup.request_type & 0x1f;
					switch (Usb::setup.request) {
						// Requests with an IN status phase (no data, or OUT data, after SETUP packet).
						// Response must be: OUT ack (if OUT is present), reply zero length IN packet.
					case Usb::SET_ADDRESS: // {{{
					{
						uint8_t const addr = Usb::setup.value.b[0] & 0x7f;
						udbg("Set address #", addr);
						UDADDR = addr << UADD0;
						USB_CLEAR_OUT_INT(TXINI);
						Usb::saved_ueienx[ep] |= _BV(TXINE);
						break;
					}
					// }}}
					case Usb::SET_CONFIGURATION: // {{{
					{
						udbg("Set conf #", Usb::setup.value.b[0]);
						if (Usb::setup.value.b[0] == 0) {
							Usb::current_configuration = 0;
							USB_CLEAR_OUT_INT(TXINI);
							// Disable all endpoints except the default.
							for (uint8_t e = 1; e < 7; ++e) {
								UENUM = e;
								UECFG1X = 0;
								UECONX = 0;	// reset EPEN.
							}
#ifdef CALL_usb_deconfigured
							usb_deconfigured();
#endif
							UENUM = ep;
							break;
						}
						uint8_t num_configurations = pgm_read_byte(&usb_device.bNumConfigurations);
						if (Usb::setup.value.b[0] > num_configurations) {
							USB_STALL;
							continue;
						}
						uint8_t old_configuration = Usb::current_configuration;
						Usb::current_configuration = Usb::setup.value.b[0];
						if (!Usb::configure_device(true)) {
							udbg("Set conf failed");
							Usb::current_configuration = old_configuration;
							USB_STALL;
							continue;
						}
#if USB_MAX_INTERFACES > 1
						Usb::Configuration const *configuration = reinterpret_cast <Usb::Configuration const *>(pgm_read_word(&usb_configuration[Usb::current_configuration]));
						uint8_t num_interfaces = pgm_read_byte(&configuration->bNumInterfaces);
						for (uint8_t i = 0; i < num_interfaces; ++i)
							Usb::current_interface[i] = 0;
#endif
						USB_CLEAR_OUT_INT(TXINI);
						break;
					}
					// }}}
					case Usb::SET_INTERFACE: // {{{
					{
						udbg("Set interface # = #", Usb::setup.index.b[0], Usb::setup.value.b[0]);
						Usb::Configuration const *configuration = reinterpret_cast <Usb::Configuration const *>(pgm_read_word(&usb_configuration[Usb::current_configuration]));
						uint8_t num_interfaces = pgm_read_byte(&configuration->bNumInterfaces);
						if (Usb::setup.index.b[0] >= num_interfaces) {
							USB_STALL;
							continue;
						}
#if USB_MAX_INTERFACES > 1
						uint8_t old_setting = Usb::current_interface[Usb::setup.index.b[0]];
						Usb::current_interface[Usb::setup.index.b[0]] = Usb::setup.value.b[0];
#endif
						if (!Usb::configure_device(false)) {
#if USB_MAX_INTERFACES > 1
							Usb::current_interface[Usb::setup.index.b[0]] = old_setting;
#endif
							USB_STALL;
							continue;
						}
						USB_CLEAR_OUT_INT(TXINI);
						break;
					}
					// }}}
					case Usb::CLEAR_FEATURE: // {{{
						udbg("Clear feature");
						switch (target) {
						case Usb::TARGET_DEVICE:
							switch (Usb::setup.value.w) {
							case Usb::DEVICE_REMOTE_WAKEUP:
								Usb::remote_wakeup_active = false;
								break;
							default:
								USB_STALL;
								continue;
							}
							break;
						case Usb::TARGET_INTERFACE:
							USB_STALL;
							continue;
						case Usb::TARGET_ENDPOINT:
						{
							if ((Usb::setup.index.w & ~0x80) >= 7) {
								USB_STALL;
								continue;
							}
							UENUM = Usb::setup.index.b[0] & 0x7f;
							if (!UECONX & _BV(EPEN)) {
								UENUM = ep;
								USB_STALL;
								continue;
							}
							UECONX = _BV(STALLRQC) | _BV(RSTDT) | _BV(EPEN);
							UENUM = ep;
							break;
						}
						default:
							USB_STALL;
							continue;
						}
						USB_CLEAR_OUT_INT(TXINI);
						break;
					// }}}
					case Usb::SET_FEATURE: // {{{
						udbg("Set feature");
						switch (target) {
						case Usb::TARGET_DEVICE:
							switch (Usb::setup.value.w) {
							case Usb::DEVICE_REMOTE_WAKEUP:
							{
								Usb::Configuration const *configuration = reinterpret_cast <Usb::Configuration const *>(pgm_read_word(&usb_configuration[Usb::current_configuration - 1]));
								uint8_t attr = pgm_read_byte(&configuration->bmAttributes);
								if (attr & 0x20)
									Usb::remote_wakeup_active = true;
								else {
									USB_STALL;
									continue;
								}
								break;
							}
							case Usb::TEST_MODE:
								// Only needs to be supported for "host controllers, hubs and high-speed capable functions" (usb_2.0.pdf, 7.1.20).
								// That's not us, so fall through to reply with error.
							default:
								USB_STALL;
								continue;
							}
							break;
						case Usb::TARGET_INTERFACE:
							USB_STALL;
							continue;
						case Usb::TARGET_ENDPOINT:
						{
							if (Usb::setup.index.w >= 7 || Usb::setup.value.w != Usb::ENDPOINT_HALT) {
								USB_STALL;
								continue;
							}
							UENUM = Usb::setup.index.b[0];
							if (!UECONX & _BV(EPEN)) {
								UENUM = ep;
								USB_STALL;
								continue;
							}
							USB_STALL; // Set stall for target endpoint.
							UENUM = ep;
							break;
						}
						default:
							USB_STALL;
							continue;
						}
						USB_CLEAR_OUT_INT(TXINI);
						break;
					// }}}
					case Usb::SET_DESCRIPTOR: // NI {{{
						udbg("Set descriptor");
						USB_STALL;
						break;
					// }}}

						// Requests with an OUT status phase (IN data).
						// Response must be: IN data (through Usb::progmem_in_pending), OUT ack (may be ignored).
					case Usb::GET_STATUS: // {{{
						udbg("Get status");
						switch (target) {
						case Usb::TARGET_DEVICE:
						{
							bool self_powered;
							if (Usb::current_configuration == 0)
								self_powered = false;
							else {
								Usb::Configuration const *configuration = reinterpret_cast <Usb::Configuration const *>(pgm_read_word(&usb_configuration[Usb::current_configuration - 1]));
								uint8_t attr = pgm_read_byte(&configuration->bmAttributes);
								self_powered = attr & 0x40;
							}
							UEDATX = (self_powered ? 1 : 0) | (Usb::remote_wakeup_active ? 2 : 0);
							UEDATX = 0;
							break;
						}
						case Usb::TARGET_INTERFACE:
						{
							if (Usb::current_configuration == 0) {
								USB_STALL;
								continue;
							}
							Usb::Configuration const *configuration = reinterpret_cast <Usb::Configuration const *>(pgm_read_word(&usb_configuration[Usb::current_configuration - 1]));
							if (Usb::setup.index.b[0] >= pgm_read_byte(&configuration->bNumInterfaces)) {
								USB_STALL;
								continue;
							}
							UEDATX = 0;
							UEDATX = 0;
							break;
						}
						case Usb::TARGET_ENDPOINT:
						{
							if (Usb::setup.index.w >= 7) {
								USB_STALL;
								continue;
							}
							UENUM = Usb::setup.index.b[0];
							uint8_t con = UECONX;
							UENUM = ep;
							if (!con & _BV(EPEN)) {
								USB_STALL;
								continue;
							}
							UEDATX = (con & _BV(STALLRQ) ? 1 : 0);
							UEDATX = 0;
							break;
						}
						default:
							USB_STALL;
							continue;
						}
						Usb::progmem_in_pending = 1;
						Usb::saved_ueienx[ep] |= _BV(TXINE);
						break;
					// }}}
					case Usb::GET_DESCRIPTOR: // {{{
					{
						uint8_t descriptor_type = Usb::setup.value.b[1];
						uint8_t index = Usb::setup.value.b[0];
						udbg("Get descriptor type # index #", descriptor_type, index);
						switch (descriptor_type) {
						case Usb::TYPE_DEVICE: // {{{
						{
							udbg("Device");
							Usb::progmem_in_pointer = reinterpret_cast <uint8_t const *>(&usb_device);
							uint8_t device_length = pgm_read_byte(&usb_device.bLength);
							Usb::progmem_in_pending = device_length + 1;
							if (Usb::progmem_in_pending > Usb::setup.length.w + 1)
								Usb::progmem_in_pending = Usb::setup.length.w + 1;
							Usb::saved_ueienx[ep] |= _BV(TXINE);
							break;
						}
						// }}}
						case Usb::TYPE_CONFIGURATION: // {{{
						{
							udbg("Configuration");
							uint8_t num_configurations = pgm_read_byte(&usb_device.bNumConfigurations);
							if (index >= num_configurations) {
								USB_STALL;
								continue;
							}
							Usb::progmem_in_pointer = reinterpret_cast <uint8_t const *>(pgm_read_ptr(&usb_configuration[index]));
							uint8_t configuration_length = pgm_read_word(&reinterpret_cast <Usb::Configuration const *>(Usb::progmem_in_pointer)->wTotalLength);
							Usb::progmem_in_pending = configuration_length + 1;
							if (Usb::progmem_in_pending > Usb::setup.length.w + 1)
								Usb::progmem_in_pending = Usb::setup.length.w + 1;
							Usb::saved_ueienx[ep] |= _BV(TXINE);
							break;
						}
						// }}}
						case Usb::TYPE_STRING: // {{{
						{
							udbg("String");
#if USB_NUM_STRINGS == 0
							// If there are no strings, return STALL.
							USB_STALL;
							continue;
#else
							// If the requested string does not exist, return STALL.
							if (Usb::setup.value.w >= USB_NUM_STRINGS + 1) {
								USB_STALL;
								continue;
							}

							// usb_string is an array of arrays of pointers. In particular:
							// Usb::String const *const usb_string[USB_NUM_LANGS][USB_NUM_STRINGS + 1] PROGMEM;
							uint16_t lang_ptr = pgm_read_word(&usb_string[0][0]);
							uint8_t lang_idx;
							for (lang_idx = 0; lang_idx < USB_NUM_LANGS; ++lang_idx) {
								if (pgm_read_word(&reinterpret_cast <uint16_t const *>(lang_ptr)[1 + lang_idx]) == Usb::setup.index.w)
									break;
							}
							if (lang_idx >= USB_NUM_LANGS) {
								USB_STALL;
								continue;
							}
							Usb::progmem_in_pointer = reinterpret_cast <uint8_t *>(pgm_read_ptr(&usb_string[lang_idx][Usb::setup.value.b[0]]));
							Usb::progmem_in_pending = pgm_read_byte(&reinterpret_cast <Usb::String const *>(Usb::progmem_in_pointer)->bLength) + 1;
							if (Usb::progmem_in_pending > Usb::setup.length.b[0] + 1)
								Usb::progmem_in_pending = Usb::setup.length.b[0] + 1;
							Usb::saved_ueienx[ep] |= _BV(TXINE);
							break;
#endif
						}
						// }}}
#ifdef USB_ENABLE_HID
						case Usb::HID_MAIN: // {{{
						{
							udbg("HID main");
							uint8_t num = Usb::setup.value.b[0];
							if (num > 0) {
								USB_STALL;
								continue;
							}
							// HID descriptor, which was also returned as part of the configuration. Why should it be available through here?
							Usb::progmem_in_pointer = usb_hid_descriptor(Usb::current_configuration, Usb::setup.index.w);
							Usb::progmem_in_pending = usb_hid_descriptor_size(Usb::current_configuration, Usb::setup.index.w) + 1;
							if (Usb::progmem_in_pending > Usb::setup.length.w + 1)
								Usb::progmem_in_pending = Usb::setup.length.w + 1;
							Usb::saved_ueienx[ep] |= _BV(TXINE);
							break;
						}
						// }}}
						case Usb::HID_REPORT: // Report descriptor. {{{
							udbg("HID report");
							if (Usb::setup.value.b[0] > 0) {
								USB_STALL;
								continue;
							}
							Usb::progmem_in_pointer = usb_hid_report_descriptor(Usb::current_configuration, Usb::setup.index.w);
							Usb::progmem_in_pending = usb_hid_report_descriptor_size(Usb::current_configuration, Usb::setup.index.w) + 1;
							if (Usb::progmem_in_pending > Usb::setup.length.w + 1)
								Usb::progmem_in_pending = Usb::setup.length.w + 1;
							Usb::saved_ueienx[ep] |= _BV(TXINE);
							break;
						// }}}
#ifdef CALL_usb_hid_physical
						case Usb::HID_PHYSICAL: // Physical descriptor. {{{
							udbg("HID physical");
							usb_hid_physical(Usb::setup.value.b[0], Usb::current_configuration, Usb::setup.index.w);
						// }}}
#endif
#endif
						default: // {{{
							udbg("unknown descriptor");
#ifdef CALL_usb_get_descriptor
							usb_get_descriptor();
							UENUM = ep;
#else
							USB_STALL;
#endif
							break;
						// }}}
						}
						break;
					}
					// }}}
					case Usb::GET_CONFIGURATION: // {{{
						udbg("Get configuration");
						UEDATX = Usb::current_configuration;
						Usb::progmem_in_pending = 1;
						Usb::saved_ueienx[ep] |= _BV(TXINE);
						break;
					// }}}
					case Usb::GET_INTERFACE: // {{{
						udbg("Get interface");
#if USB_MAX_INTERFACES > 1
						UEDATX = Usb::current_interface[Usb::setup.index.b[0]];
#else
						UEDATX = 0;
#endif
						UEDATX = 0;
						Usb::progmem_in_pending = 1;
						Usb::saved_ueienx[ep] |= _BV(TXINE);
						break;
					// }}}
					case Usb::SYNCH_FRAME: // NI {{{
						udbg("synch frame");
						USB_STALL;
						continue;
					// }}}
					default: // {{{
						udbg("unknown standard ep0 request");
#ifdef CALL_usb_standard_setup0
						usb_standard_setup0();
						UENUM = ep;
#else
						USB_STALL;
#endif
						break;
					} // }}}
				}
				else {
					// Not a standard request.
					udbg("non-standard ep0 request");
#ifdef USB_ENABLE_HID
					if ((Usb::setup.request_type & 0x60) == 0x20) {
						switch (Usb::setup.request) {
						case Usb::GET_REPORT: // {{{
							udbg("get report");
#ifdef CALL_usb_hid_get_report
							usb_hid_get_report(Usb::current_configuration, Usb::setup.index.b[0]);
#endif
							USB_CLEAR_OUT_INT(TXINI);
							break;
						// }}}
						case Usb::GET_IDLE: // {{{
							udbg("get idle");
#ifdef CALL_usb_hid_get_idle
							usb_hid_get_idle(Usb::current_configuration, Usb::setup.index.b[0]);
#else
							UEDATX = 0;
							UEDATX = 0;
#endif
							USB_CLEAR_OUT_INT(TXINI);
							break;
						// }}}
						case Usb::GET_PROTOCOL: // {{{
							udbg("get protocol");
#ifdef CALL_usb_hid_get_protocol
							UEDATX = usb_hid_get_protocol(Usb::current_configuration, Usb::setup.index.b[0]);
#else
							UEDATX = 1;
#endif
							USB_CLEAR_OUT_INT(TXINI);
							break;
						// }}}
#ifdef CALL_usb_hid_set_report
						case Usb::SET_REPORT: // {{{
						{
							udbg("set report setup received, waiting for # OUT #", UENUM, UEINTX);
							Usb::saved_ueienx[ep] |= _BV(RXOUTE) | _BV(NAKINE);
							break;
						}
						// }}}
#endif
						case Usb::SET_IDLE: // {{{
							udbg("set idle");
#ifdef CALL_usb_hid_set_idle
							usb_hid_set_idle(Usb::current_configuration, Usb::setup.index.b[0], Usb::setup.value.b[1], Usb::setup.value.b[0]);
#endif
							USB_CLEAR_OUT_INT(TXINI);
							break;
						// }}}
						case Usb::SET_PROTOCOL: // {{{
							udbg("set protocol");
#ifdef CALL_usb_hid_set_protocol
							usb_hid_set_protocol(Usb::current_configuration, Usb::setup.index.b[0], Usb::setup.value.b[0]);
#endif
							USB_CLEAR_OUT_INT(TXINI);
							break;
						// }}}
						default: // {{{
							udbg("unknown HID request");
							USB_STALL;
							continue;
						// }}}
						}
					}
#endif
#ifdef CALL_usb_setup0
					else {
						udbg("non-standard, non-class request");
						usb_setup0();
						UENUM = ep;
					}
#endif
				}
			}
		} // }}}
		if (UEINTX & _BV(RXOUTI)) { // {{{
			// UEINTX.RXOUTI = 1: received OUT data.
			udbg("OUT on ep #", ep);
			if (ep == 0) {
				if ((Usb::setup.request_type & 0x60) == 0) {
					switch (Usb::setup.request) {
					case Usb::SET_DESCRIPTOR:
						// Not implemented.
						USB_STALL;
						continue;
					default:
						// If this even was the cause of this interrupt, that was a problem.
						// But the flag may be set while the interrupt is disabled, so don't report an error.
						break;
					}
				}
				else if ((Usb::setup.request_type & 0x60) == 0x20) {
					switch (Usb::setup.request) {
#ifdef CALL_usb_hid_set_report
					case Usb::SET_REPORT:
					{
						udbg("set report data");
						uint8_t count = UEBCLX;
						for (uint8_t i = 0; i < count; ++i)
							Usb::hid_set_report_write(UEDATX);
						USB_CLEAR_OUT_INT(RXOUTI);
						break;
					}
#endif
					default:
						USB_STALL;
						continue;
					}
				}
				else {
					udbg("Custom OUT0");
#ifdef CALL_usb_out0
					usb_out0();
					UENUM = ep;
#else
					USB_STALL;
#endif
				}
			}
			else {
				udbg("OUT #", ep);
				if (Usb::saved_ueienx[ep] & _BV(RXOUTE)) {
					switch(ep) {
#if defined(USB_RX1_BULK) || defined(USB_RX1_INTERRUPT)
					case 1:
#ifdef USB_RX1_BULK
						_AVR_USB_RX_BULK_OUT(1);
#else
						_AVR_USB_RX_INTERRUPT_OUT(1, USB_EP1_SIZE);
#endif
						break;
#endif
#if defined(USB_RX2_BULK) || defined(USB_RX2_INTERRUPT)
					case 2:
#ifdef USB_RX2_BULK
						_AVR_USB_RX_BULK_OUT(2);
#else
						_AVR_USB_RX_INTERRUPT_OUT(2, USB_EP2_SIZE);
#endif
						break;
#endif
#if defined(USB_RX3_BULK) || defined(USB_RX3_INTERRUPT)
					case 3:
#ifdef USB_RX3_BULK
						_AVR_USB_RX_BULK_OUT(3);
#else
						_AVR_USB_RX_INTERRUPT_OUT(3, USB_EP3_SIZE);
#endif
						break;
#endif
#if defined(USB_RX4_BULK) || defined(USB_RX4_INTERRUPT)
					case 4:
#ifdef USB_RX4_BULK
						_AVR_USB_RX_BULK_OUT(4);
#else
						_AVR_USB_RX_INTERRUPT_OUT(4, USB_EP4_SIZE);
#endif
						break;
#endif
#if defined(USB_RX5_BULK) || defined(USB_RX5_INTERRUPT)
					case 5:
#ifdef USB_RX5_BULK
						_AVR_USB_RX_BULK_OUT(5);
#else
						_AVR_USB_RX_INTERRUPT_OUT(5, USB_EP5_SIZE);
#endif
						break;
#endif
#if defined(USB_RX6_BULK) || defined(USB_RX6_INTERRUPT)
					case 6:
#ifdef USB_RX6_BULK
						_AVR_USB_RX_BULK_OUT(6);
#else
						_AVR_USB_RX_INTERRUPT_OUT(6, USB_EP6_SIZE);
#endif
						break;
#endif
					default:
						// Do this only for endpoints that have no buffer defined.
#ifdef CALL_usb_out
						usb_out();
						UENUM = ep;
#else
						USB_STALL;
#endif
						break;
					}
				}
			}
		} // }}}
		if (UEINTX & _BV(TXINI)) { // {{{
			// UEINTX.TXINI = 1: ready to accept IN data.
			udbg("IN on ep #", ep);
			if (ep == 0) {
				uint16_t sent = 0;
				if (Usb::progmem_in_pending > 0) {
					while (sent < Usb::setup_size) {
						Usb::progmem_in_pending -= 1;
						if (Usb::progmem_in_pending == 0) {
							// We are allowed to write another byte, but won't.
							// That means this is a short packet, marking the end of the transmission.
							break;
						}
						uint8_t data = pgm_read_byte(Usb::progmem_in_pointer++);
						UEDATX = data;
						//udbg_byte(data);
						//udbg_char('/');
						sent += 1;
					}
					udbg("send remaining: #, UEIENX: #", Usb::progmem_in_pending, Usb::saved_ueienx[ep]);
					USB_CLEAR_IN_INT(TXINI);
					if (Usb::progmem_in_pending == 0) {
						Usb::saved_ueienx[ep] &= ~_BV(TXINE);	// No more data: disable interrupt.
						udbg("send complete");
					}
				}
				else {
					if ((Usb::setup.request_type & 0x60) == 0) {
						switch (Usb::setup.request) {
						case Usb::SET_ADDRESS:
							udbg("Finalizing SET_ADDRESS");
							UDADDR |= _BV(ADDEN);
							Usb::saved_ueienx[ep] &= ~_BV(TXINE);	// Disable interrupt.
							break;
						default:
							// If this even was the cause of this interrupt, that was a problem.
							// But the flag may be set while the interrupt is disabled, so don't report an error.
							break;
						}
					}
					else {
						udbg("Custom IN0");
						if (Usb::saved_ueienx[ep] & _BV(TXINE)) {
#ifdef CALL_usb_in0
							usb_in0();
							UENUM = ep;
#else
							USB_STALL;
#endif
						}
					}
				}
			}
			else {
				udbg("IN #", ep);
				if (Usb::saved_ueienx[ep] & _BV(TXINE)) {
					switch (ep) {
#if defined(USB_TX1_BULK) || defined(USB_TX1_INTERRUPT)
					case 1:
#ifdef USB_TX1_BULK
						_AVR_USB_TX_BULK_IN(1);
#else
						_AVR_USB_TX_INTERRUPT_IN(1);
#endif
						break;
#endif
#if defined(USB_TX2_BULK) || defined(USB_TX2_INTERRUPT)
					case 2:
#ifdef USB_TX2_BULK
						_AVR_USB_TX_BULK_IN(2);
#else
						_AVR_USB_TX_INTERRUPT_IN(2);
#endif
						break;
#endif
#if defined(USB_TX3_BULK) || defined(USB_TX3_INTERRUPT)
					case 3:
#ifdef USB_TX3_BULK
						_AVR_USB_TX_BULK_IN(3);
#else
						_AVR_USB_TX_INTERRUPT_IN(3);
#endif
						break;
#endif
#if defined(USB_TX4_BULK) || defined(USB_TX4_INTERRUPT)
					case 4:
#ifdef USB_TX4_BULK
						_AVR_USB_TX_BULK_IN(4);
#else
						_AVR_USB_TX_INTERRUPT_IN(4);
#endif
						break;
#endif
#if defined(USB_TX5_BULK) || defined(USB_TX5_INTERRUPT)
					case 5:
#ifdef USB_TX5_BULK
						_AVR_USB_TX_BULK_IN(5);
#else
						_AVR_USB_TX_INTERRUPT_IN(5);
#endif
						break;
#endif
#if defined(USB_TX6_BULK) || defined(USB_TX6_INTERRUPT)
					case 6:
#ifdef USB_TX6_BULK
						_AVR_USB_TX_BULK_IN(6);
#else
						_AVR_USB_TX_INTERRUPT_IN(6);
#endif
						break;
#endif
					default:
						// Do this only for endpoints that have no buffer defined.
#ifdef CALL_usb_in
						usb_in();
						UENUM = ep;
#else
						USB_STALL;
#endif
						break;
					}
				}
			}
		} // }}}
		if (UEINTX & _BV(STALLEDI)) { // {{{
			// UEINTX.STALLEDI = 1: stalled packet was sent to host.
			udbg("STALLED on ep #", ep);
#ifdef CALL_usb_stalled
			USB_CLEAR_OUT_INT(STALLEDI);
			if (Usb::saved_ueienx[ep] & _BV(STALLEDE)) {
				usb_stalled();
				UENUM = ep;
			}
#else
			Usb::saved_ueienx[ep] &= ~_BV(STALLEDE);
#endif
		} // }}}
		if (UEINTX & _BV(NAKINI)) { // {{{
			// UEINTX.NAKINI = 1: NAK IN was sent (iow: IN was requested)
			udbg("NAKIN on ep #", ep);
#ifdef CALL_usb_hid_set_report
			if (ep == 0 && (Usb::setup.request_type & 0x60) == 0x20 && Usb::setup.request == Usb::SET_REPORT) {
				uint8_t count = UEBCLX;
				for (uint8_t i = 0; i < count; ++i)
					Usb::hid_set_report_write(UEDATX);
				Usb::saved_ueienx[ep] &= ~(_BV(RXOUTE) | _BV(NAKINI));	// Disable interrupts.
				// Reset the endpoint. This is required, because the hardware does not do what the datasheet says.
				// The device does not expect to send an IN ZLP in the status phase if there was OUT data.
				// Perhaps it would if there was an OUT ZLP, but Linux does not send that, not does the USB standard require it.
				// Resetting the endpoint allows it to send IN again, so that works around the issue.
				UERST = 1;
				UERST = 0;
				USB_CLEAR_OUT_INT(TXINI);	// Reply ZLP IN packet.
				Usb::hid_set_report_end();
				continue;
			}
#endif
#ifdef CALL_usb_nak_in
			USB_CLEAR_IN_INT(NAKINI);
			if (Usb::saved_ueienx[ep] & _BV(NAKINE)) {
				usb_nak_in();
				UENUM = ep;
			}
#else
			Usb::saved_ueienx[ep] &= ~_BV(NAKINE);
#endif
		} // }}}
		if (UEINTX & _BV(NAKOUTI)) { // {{{
			// UEINTX.NAKOUTI = 1: NAK OUT sent
			udbg("NAKOUT on ep #", ep);
#ifdef CALL_usb_nak_out
			USB_CLEAR_OUT_INT(NAKOUTI);
			if (Usb::saved_ueienx[ep] & _BV(NAKOUTE)) {
				usb_nak_out();
				UENUM = ep;
			}
#else
			Usb::saved_ueienx[ep] &= ~_BV(NAKOUTE);
#endif
		} // }}}
		if (UESTA0X & _BV(OVERFI)) { // {{{
			// UESTA0X.OVERFI = 1: iso overflow
			udbg("OVERF on ep #", ep);
			UESTA0X &= ~_BV(OVERFI);
#ifdef CALL_usb_overflow
			if (Usb::saved_ueienx[ep] & _BV(FLERRE)) {
				usb_overflow();
				UENUM = ep;
			}
#else
#ifndef CALL_usb_underflow
			Usb::saved_ueienx[ep] &= ~_BV(FLERRE);
#endif
#endif
		} // }}}
		if (UESTA0X & _BV(UNDERFI)) { // {{{
			// UESTA0X.UNDERFI = 1: iso underflow
			udbg("UNDERF on ep #", ep);
			UESTA0X &= ~_BV(UNDERFI);
#ifdef CALL_usb_underflow
			if (Usb::saved_ueienx[ep] & _BV(FLERRE)) {
				usb_underflow();
				UENUM = ep;
			}
#else
#ifndef CALL_usb_overflow
			Usb::saved_ueienx[ep] &= ~_BV(FLERRE);
#endif
#endif
		} // }}}
	}
	udbg("End of interrupt ep #, UEIENX[0]: #, UEINTX: #", UENUM, Usb::saved_ueienx[0], UEINTX);
	// Disable interrupts so all of them will be enabled an we will exit, to avoid blowing up the stack.
	cli();
	for (uint8_t ep = 0; ep < 7; ++ep) {
		UENUM = ep;
		UEIENX = Usb::saved_ueienx[ep];
	}
	if (saved_vbuste)
		USBCON |= _BV(VBUSTE);
	UDIEN = saved_udien;
	UENUM = saved_ep;
	Usb::in_interrupt = false;
} // }}}
/// @endcond

#ifdef AVR_TEST_USB // {{{

#if AVR_TEST == 0

// minimal CDC ACM implementation. {{{

// Create device descriptor (with interface association support).
USB_SIMPLE_IA_DEVICE();

// Create configuration descriptor (including interfaces, endpoints, and CDC structs).
USB_SIMPLE_CDC_CONFIGURATION(test_configuration);

// Create a list of this 1 configuration to be all configurations.
USB_SIMPLE_SINGLE_CONFIGURATION(test_configuration);

// }}}

namespace Usb {
	static const uint8_t testcode = 'U';

	static inline void start() {
		enable();
	}

	static bool test(uint8_t part, uint8_t cmd, uint8_t len) { // Testing function; no tests are implemented currently. {{{
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('\n');
			return true;
		}
		switch (cmd) {
		default:
			return false;
		}
		(void)&len;
		// TODO: Implement USB tests.
		// This can only be done if the test management interface can be forced to Usart1.
		// Until then: most features are tested by running the test management interface.
		return true;
	} // }}}
}

#else 

namespace Usb {
	static const uint8_t testcode = 0;
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		(void)&part;
		(void)&cmd;
		(void)&len;
		return false;
	}
}
#endif
#endif // defined(AVR_TEST_USB) }}}

#endif // defined(USB_ENABLE)

#endif // include guard.

// vim: set foldmethod=marker :
