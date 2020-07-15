#include "sense8.h"


String SensorData::displayTemperature()
{
    float temperatureF = temperature * 9/5 + 32;
    String displayTemperature(temperatureF, 1);
    displayTemperature += " \xF8" "F";
    return displayTemperature;
}
