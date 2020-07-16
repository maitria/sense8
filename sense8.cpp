#include "sense8.h"
#include <math.h>

float SensorData::fahrenheit()
{
    return temperature * 9/5 + 32;
}

String SensorData::displayTemperature()
{
    return String(lrint(fahrenheit())) + " \xF8" "F";
}
