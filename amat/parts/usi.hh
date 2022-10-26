#ifndef _AVR_USI_HH
#define _AVR_USI_HH

/**
 * @file
 * Universial Serial Interface
 *
 * @author Bas Wijnen <wijnen@debian.org>
 */

/// Universal Serial Interface
/**
 * Usi is currently not implemented, so there is also no documentation yet.
 */
namespace Usi {
	// TODO: Implement Usi functions.
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
