pro-micro = -mmcu=atmega32u4 -DF_CPU=16000000L
use-optimizations = -O2
include-paths = -IArduinoCore-avr/cores/arduino -I.
do-not-link = -c

SparkFun_RHT03.o: SparkFun_RHT03.h SparkFun_RHT03.cpp
	avr-c++ $(pro-micro) $(use-optimizations) $(include-paths) $(do-not-link) SparkFun_RHT03.cpp
