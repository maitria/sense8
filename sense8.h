#ifndef RESPITE_SENSORS_INCLUDED
#define RESPITE_SENSORS_INCLUDED

struct Message
{
    char location[9];
    float temperature;
    float humidity;
};

#endif
