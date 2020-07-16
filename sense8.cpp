#include "sense8.h"

float SensorData::fahrenheit()
{
    return temperature * 9/5 + 32;
}

String SensorData::displayTemperature()
{
    String displayTemperature(fahrenheit(), 0);
    displayTemperature += " \xF8" "F";
    return displayTemperature;
}
