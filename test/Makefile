# Build system for testing the avr header files.

MCUS = atmega328p atmega1284p atmega168 atmega2560
FREQS = 16000000 8000000 32768
MAXTEST = 0
EXTRA_CPPFLAGS ?= -DAVR_TEST_INDEX=0

all:
	set -e ; \
	for mcu in ${MCUS} ; do \
		for freq in ${FREQS} ; do \
			for i in `seq 0 ${MAXTEST}` ; do \
				${MAKE} default MCU=$$mcu F_CPU=$$freq SUFFIX=$$i EXTRA_CPPFLAGS=-DAVR_TEST_INDEX=$$i ; \
			done ; \
		done ; \
	done

SOURCE = test.cc

# Target mcu and upload settings.
MCU ?= atmega32u4
F_CPU ?= 16000000
PROTOCOL ?= avr109
BAUD ?= 57600
#TOUCH1200 = yes
PORT = $(wildcard /dev/ttyACM*)	# Avoid connecting to /dev/ttyUSB0 that is used for debugging.

include amat.mk
