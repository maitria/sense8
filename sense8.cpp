#include "sense8.h"
#include <math.h>

float SensorData::fahrenheit()
{
    return temperature * 9/5 + 32;
}

String SensorData::displayTemperature()
{
    String displayTemperature(lrint(fahrenheit()));
    displayTemperature += " \xF8" "F";
    return displayTemperature;
}
