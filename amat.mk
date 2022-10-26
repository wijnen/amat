# This is a Makefile for building programs for avr microcontrollers using avr.hh.
# Include it from your source Makefile.

# In your source, you may define the following variables:

##################### Start of variable list #####################

# If SOURCE is defined, it overrides TARGETS.
# It must be all sources for the project to make (usually just a single *.cc file).
# The name of the result is the basename of the first source file.
ifdef SOURCE
TARGETS := $(basename $(firstword ${SOURCE}))
$(subst -,_,${TARGETS})_SOURCES = ${SOURCE}
endif

# The name of the programs to make when called with no arguments.
TARGETS ?= firmware


# List of the program's sources.
# Files named *.cc are compiled and linked.
# Other files are treated as dependencies for all files for this target.
# This is commented out, because you should always have this explicitly defined.
# Each target has its own sources, this example shows a list of sources for my_avr-firmware.hex
# The name in an ID like this uses '_' where the target has '-', as shown here.
#my_avr_firmware_SOURCES ?= \
#	firmware.cc \
#	firmware.hh

# Suffix to add to the build target name.
# If defined, an _ and this value is added to all generated names, including the build result.
#SUFFIX = "rc1"


# Build flags.
CPPFLAGS ?= -Wall -Wextra -ggdb3 -Wformat -Werror=format-security -D_FORTIFY_SOURCE=2 -Wshadow -fno-strict-aliasing -Werror --param=ssp-buffer-size=4 -std=c++11 ${EXTRA_CPPFLAGS}
CXXFLAGS ?= -Os ${EXTRA_CXXFLAGS}
LDFLAGS ?= ${EXTRA_LDFLAGS}
LIBS ?= ${EXTRA_LIBS}


# Target mcu and upload settings.
# Supported values for mcu are filenames (without .hh) in mcu/.
# Note that MCU and F_CPU must be global variables; they cannot be different per target.
# If differing values are required, make needs to be called once for each combination.
MCU ?= atmega328p
F_CPU ?= 16000000
PROTOCOL ?= arduino
BAUD ?= 57600
TOUCH1200 ?=	# Set this to non-empty for Leonardo.

# Port for uploading to the device.
# You probably don't need to override this default.
# If you do, it is a good idea to use wildcards like in the default value.
PORT ?= $(strip $(firstword $(wildcard /dev/ttyUSB* /dev/ttyACM*)))

###################### End of variable list ######################

# Prepare suffix value including _.
ifdef SUFFIX
FULL_SUFFIX := _${SUFFIX}
else
FULL_SUFFIX :=
endif

# Determine the path to the sources.
AVR_HH_PATH := $(dir $(realpath $(lastword ${MAKEFILE_LIST})))amat

BASE_MAKEFILES := ${MAKEFILE_LIST}

# Disable implicit rules.
%.o: %.cc

# Construct list of TARGETS that can be used as IDs.
TARGET_IDS := $(subst -,_,${TARGETS})

# Define the default goal, if it wasn't defined in the calling Makefile.
default: $(addsuffix _${MCU}_${F_CPU}${FULL_SUFFIX}.hex,${TARGETS})

ifneq (${MAKECMDGOALS}, clean)
# Include dependency Makefiles.
include $(addsuffix _${MCU}_${F_CPU}${FULL_SUFFIX}.d,$(addprefix build/,${TARGETS}))
endif


# Required values for the flags.
CPPFLAGS += -I${AVR_HH_PATH} -DF_CPU=${F_CPU} -DMCU="${MCU}" -DAVR_MCU_HEADER=\"mcu/${MCU}.hh\"
CXXFLAGS += -mmcu=${MCU}
LDFLAGS += -mmcu=${MCU}

# Object files to build.
build/%.d: target = $(notdir $(basename $@))
build/%.d: deps = $(strip $(filter-out %.cc,${$(subst -,_,$(notdir $(basename $@)))_SOURCES}))
build/%.d: sources = $(filter %.cc,${$(subst -,_,$(subst _${MCU}_${F_CPU}${FULL_SUFFIX},,$(notdir $(basename $@))))_SOURCES})
build/%.d: objs = $(patsubst %.cc,build/%_${MCU}_${F_CPU}${FULL_SUFFIX}.o,${sources})
build/%.d:
	mkdir -p "$(dir $@)"
	echo "build/${target}.elf: ${objs} ${deps}" > "$@"
	echo "${subst -,_,${target}}_OBJS = ${objs}" >> "$@"
	echo "build/${target}.d: ${objs} ${BASE_MAKEFILES}" >> "$@"
	for source in ${sources} ; do \
		avr-g++ ${CPPFLAGS} ${CXXFLAGS} -M "$$source" -MQ "build/$${source%.*}_${MCU}_${F_CPU}${FULL_SUFFIX}.o" -o- >> "$@" ; \
		if test "${deps}" != "" ; then \
			echo "build/$${source%.cc}_${MCU}_${F_CPU}${FULL_SUFFIX}.o: ${deps}" >> "$@" ; \
		fi ; \
	done

%.hex: TARGET_ID = $(subst -,_,$(basename $@))

build/%_${MCU}_${F_CPU}${FULL_SUFFIX}.o: %.cc ${BASE_MAKEFILES}
	mkdir -p "$(dir $@)"
	avr-g++ ${CPPFLAGS} ${CXXFLAGS} -c "$<" -o "$@"

%.elf: ${BASE_MAKEFILES}
	mkdir -p "$(dir $@)"
	avr-g++ ${LDFLAGS} ${$(subst -,_,$(patsubst %.elf,%_OBJS,$(notdir $@)))} -o "$@" ${LIBS}
	avr-size --format=avr --mcu=${MCU} "$@"

%.hex: build/%.elf ${BASE_MAKEFILES}
	avr-objcopy -O ihex -R .eeprom "$<" "$@"

upload: $(firstword ${TARGETS})_${MCU}_${F_CPU}${FULL_SUFFIX}.hex ${BASE_MAKEFILES}
	if [ "${TOUCH1200}" != "" ] ; then stty -F ${PORT} raw 1200 ; echo '' > ${PORT} ; sleep 4 ; fi
	avrdude -v -D -P ${PORT} -c "${PROTOCOL}" -b "${BAUD}" -p "${MCU}" -U "flash:w:$<:i"

clean:
	rm -rf build *.hex

.PHONY: default upload clean
