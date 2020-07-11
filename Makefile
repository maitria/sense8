libraries         =  SparkFun_Micro_OLED_Arduino_Library\
		     ArduinoCore-avr/libraries/SPI\
		     ArduinoCore-avr/libraries/Wire

pro-micro         = -mmcu=atmega32u4 -DF_CPU=16000000L
usb-config        = -DUSB_VID=0x2341 -DUSB_PID=0x8037 -DUSB_MANUFACTURER='"Unknown"' -DUSB_PRODUCT='"Arduino Micro"'
make-small-code   = -Os

define library_include_paths
  -I$(1)/src
endef

include-paths     = -IArduinoCore-avr/cores/arduino\
		    $(foreach library,$(libraries), $(call library_include_paths,$(library)))\
		    -I.

do-not-link       = -c
c-stuff-arduino-people-want = -g -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects
cpp-stuff-arduino-people-want = -g -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error-narrowing -MMD -flto

define library_sources
  $(shell find $(1)/src -name '*.cpp' -o -name '*.c')
endef

sensor_sources = \
		 SparkFun_RHT03.cpp\
		 respite-sensor.cpp\
		 $(wildcard ArduinoCore-avr/cores/arduino/*.cpp)\
		 $(wildcard ArduinoCore-avr/cores/arduino/*.c)\
		 $(foreach library,$(libraries),$(call library_sources,$(library)))

sensor_objects = $(patsubst %.c,out/%.o,$(patsubst %.cpp,out/%.o,$(sensor_sources)))

.PHONY: all flash clean
all: out/respite-sensor.hex

flash: out/respite-sensor.hex
	avrdude -p atmega32u4 -c avr109 -P $(TTY) -U flash:w:$<

clean:
	rm -f $(sensor_objects) *.hex

# For the noobs. In the below stuff,
# -o is here comes the output file
#  $@ is a variable that contains the target filename
#  $< is a variable that's the first input file for this rule

out/%.o: %.c
	mkdir -p $(dir $@)
	avr-gcc $(pro-micro) $(usb-config) $(make-small-code) $(include-paths) $(do-not-link) $(c-stuff-arduino-people-want) -o $@ $<

out/%.o: %.cpp
	mkdir -p $(dir $@)
	avr-c++ $(pro-micro) $(usb-config) $(make-small-code) $(include-paths) $(do-not-link) $(cpp-stuff-arduino-people-want) -o $@ $<

out/respite-sensor.elf: $(sensor_objects)
	avr-c++ $(pro-micro) -o out/respite-sensor.elf $(sensor_objects)

out/%.hex: out/%.elf
	avr-objcopy -O ihex -R .eeprom $< $@

