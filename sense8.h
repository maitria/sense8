#ifndef SENSE8_INCLUDED
#define SENSE8_INCLUDED

#include <Arduino.h>

struct SensorData
{
    char location[9];
    float temperature;
    float humidity;

    String displayTemperature();

private:
    float fahrenheit();
};

#endif
