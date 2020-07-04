pro-micro         = -mmcu=atmega32u4 -DF_CPU=16000000L
usb-config        = -DUSB_VID=0x2341 -DUSB_PID=0x8037 -DUSB_MANUFACTURER='"Unknown"' -DUSB_PRODUCT='"Arduino Micro"'
make-small-code   = -Os
include-paths     = -IArduinoCore-avr/cores/arduino -I.
do-not-link       = -c
c-stuff-arduino-people-want = -g -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects
cpp-stuff-arduino-people-want = -g -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error-narrowing -MMD -flto


sensor_sources = SparkFun_RHT03.cpp RHT03-Example-Serial.cpp $(wildcard ArduinoCore-avr/cores/arduino/*.cpp) $(wildcard ArduinoCore-avr/cores/arduino/*.c)
sensor_objects = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(sensor_sources)))

all: RHT03-Example-Serial.hex

flash: RHT03-Example-Serial.hex
	avrdude -p atmega32u4 -c avr109 -P $(TTY) -U flash:w:$<

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@

RHT03-Example-Serial.elf: $(sensor_objects)
	avr-c++ $(pro-micro) -o RHT03-Example-Serial.elf $(sensor_objects)

%.o: %.c
	avr-gcc $(pro-micro) $(usb-config) $(make-small-code) $(include-paths) $(do-not-link) $(c-stuff-arduino-people-want) -o $@ $<

%.o: %.cpp
	avr-c++ $(pro-micro) $(usb-config) $(make-small-code) $(include-paths) $(do-not-link) $(cpp-stuff-arduino-people-want) -o $@ $<

clean:
	rm -f $(sensor_objects) *.hex
