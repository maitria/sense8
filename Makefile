# you gotta set your own environment variables for your usb ports
sensor-port       =  $(BOTTOM_LEFT_USB)
hub-port	  =  $(TOP_RIGHT_USB)

libraries         =  ArduinoCore-avr/libraries/SPI\
		     ArduinoCore-avr/libraries/Wire\
		     RF24\
		     Adafruit_BusIO\
		     Adafruit-GFX-Library\
		     Adafruit_SSD1306

pro-micro         = -mmcu=atmega32u4 -DF_CPU=16000000L
usb-config        = -DUSB_VID=0x2341 -DUSB_PID=0x8037 -DUSB_MANUFACTURER='"Unknown"' -DUSB_PRODUCT='"Arduino Micro"'
make-small-code   = -Os
do-not-link       = -c
c-stuff-arduino-people-want = -g -DARDUINO=150 -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects
cpp-stuff-arduino-people-want = -g -DARDUINO=150 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error-narrowing -MMD -flto

define library_include_paths
  $(if $(wildcard $(1)/*.h), -I$(1), -I$(1)/src)
endef

define legacy_library_sources
  $(wildcard $(1)/*.cpp)\
  $(wildcard $(1)/*.c)\
  $(wildcard $(1)/utility/*.cpp)\
  $(wildcard $(1)/utility/*.c)
endef

define library_sources
  $(if $(wildcard $(1)/*.h),\
     $(call legacy_library_sources,$(1))\
    ,\
     $(shell find $(1)/src -name '*.cpp' -o -name '*.c'))
endef

include-paths     = -IArduinoCore-avr/cores/arduino\
		    $(foreach library,$(libraries), $(call library_include_paths,$(library)))\
		    -I.

sensor_sources = \
		 SparkFun_RHT03.cpp\
		 respite-sensor.cpp\
		 sense8.cpp\
		 $(wildcard ArduinoCore-avr/cores/arduino/*.cpp)\
		 $(wildcard ArduinoCore-avr/cores/arduino/*.c)\
		 $(foreach library,$(libraries),$(call library_sources,$(library)))

hub_sources =    \
		 SparkFun_RHT03.cpp\
		 respite-hub.cpp\
		 sense8.cpp\
		 $(wildcard ArduinoCore-avr/cores/arduino/*.cpp)\
		 $(wildcard ArduinoCore-avr/cores/arduino/*.c)\
		 $(foreach library,$(libraries),$(call library_sources,$(library)))

sensor_objects = $(patsubst %.c,out/%.o,$(patsubst %.cpp,out/%.o,$(sensor_sources)))

hub_objects    = $(patsubst %.c,out/%.o,$(patsubst %.cpp,out/%.o,$(hub_sources)))

.PHONY: all flash clean
all: sensor hub

sensor: out/respite-sensor.hex

flash-sensor: out/respite-sensor.hex
	avrdude -p atmega32u4 -c avr109 -P $(sensor-port) -U flash:w:$<

hub:    out/respite-hub.hex

flash-hub:    out/respite-hub.hex
	avrdude -p atmega32u4 -c avr109 -P $(hub-port) -U flash:w:$<

clean:
	rm -f $(sensor_objects) *.hex

# For the noobs. In the below stuff,
# -o is here comes the output file
# $@ is a variable that contains the target filename
# $< is a variable that's the first input file for this rule

out/%.o: %.c
	@printf "Compiling $< ...\n"
	@mkdir -p $(dir $@)
	@avr-gcc $(pro-micro) $(usb-config) $(make-small-code) $(include-paths) $(do-not-link) $(c-stuff-arduino-people-want) -o $@ $<

out/%.o: %.cpp
	@printf "Compiling $< ...\n"
	@mkdir -p $(dir $@)
	@avr-c++ $(pro-micro) $(usb-config) $(make-small-code) $(include-paths) $(do-not-link) $(cpp-stuff-arduino-people-want) -o $@ $<

out/respite-sensor.elf: $(sensor_objects)
	@printf "Linking $@ ...\n"
	@avr-c++ $(pro-micro) -o out/respite-sensor.elf $(sensor_objects)

out/respite-hub.elf: $(hub_objects)
	@printf "Linking $@ ...\n"
	@avr-c++ $(pro-micro) -o out/respite-hub.elf $(hub_objects)

out/%.hex: out/%.elf
	@printf "Writing $@ ...\n"
	@avr-objcopy -O ihex -R .eeprom $< $@

