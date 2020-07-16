#include "sense8.h"
#include <math.h>

SensorData::SensorData()
{
    strcpy(location, "???");
    temperature = 0;
    humidity = 0;
}

float SensorData::fahrenheit()
{
    return temperature * 9/5 + 32;
}

String SensorData::displayTemperature()
{
    return String(lrint(fahrenheit())) + " \xF8" "F";
}

String SensorData::displayHumidity()
{
    return String(lrint(humidity)) + "%";
}
