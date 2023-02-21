#Testing Amat

There are two parts of amat to test: that the software follows the API and
compiles properly, and that it will do the requested things on the hardware.

The test system tries to compile versions for all supported combinations of
features. However, because hardware has a limited number of write cycles for
its flash memory, these are not all written to hardware. Instead, the hardware
test consists of two devices that are connected through all the interfaces
(usart, spi, twi) which will run all the tests using a single program for each.
