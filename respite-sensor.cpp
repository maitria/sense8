#include <SparkFun_RHT03.h>
#include <Wire.h>
#include <SFE_MicroOLED.h>

RHT03 sensor;

void setup()
{
    Serial.begin(9600); // Serial is used to print sensor readings.
    
    sensor.begin(A8);//A8 is the data pin
}

int tick = 0;

void loop()
{
    tick ++;
    int updateResult = sensor.update();
    
    bool succeeded = updateResult == 1;

    if (succeeded)
    {
        float latestHumidity = sensor.humidity();
        float latestTempC = sensor.tempC();
        float latestTempF = sensor.tempF();
        
        Serial.println("Tick: " + String(tick));
        Serial.println("Humidity: " + String(latestHumidity, 1) + " %");
        Serial.println("Temp (F): " + String(latestTempF, 1) + " deg F");
        Serial.println("Temp (C): " + String(latestTempC, 1) + " deg C");
    }
    else
    {
        Serial.println("Failed attempt at tick: " + String(tick));
    }
    
    delay(RHT_READ_INTERVAL_MS);
}
