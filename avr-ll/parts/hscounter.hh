// High Speed Counter

#ifndef _AVR_HSCOUNTER_HH
#define _AVR_HSCOUNTER_HH

/** @file
# High speed counter 4, present in USB-capable devices.

@author Bas Wijnen <wijnen@debian.org>
*/

namespace Hscounter {
}

#ifdef AVR_TEST_HSCOUNTER // {{{

#if AVR_TEST == 0
namespace Hscounter {
	static const uint8_t testcode = 'H';
	static bool test(uint8_t part, uint8_t cmd, uint8_t len) {
		if (part != testcode)
			return false;
		if (cmd == '?' && len == 0) {
			Test::tx(testcode);
			Test::tx('\n');
			return true;
		}
		// Commands:
		switch (cmd) {
		default:
			return false;
		}
		return true;
	}
}

#else

namespace Hscounter {
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
