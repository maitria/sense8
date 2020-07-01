

SparkFun_RHT03.o: SparkFun_RHT03.h SparkFun_RHT03.cpp
	avr-c++ -mmcu=atmega32u4 -O2 -DF_CPU=16000000L -IArduinoCore-avr/cores/arduino -I. -c SparkFun_RHT03.cpp
