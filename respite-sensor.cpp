#include <SparkFun_RHT03.h>
#include <Wire.h>
#include <SFE_MicroOLED.h>

#define SENSOR_DATA_PIN A8
#define FAKE_RESET_PIN A1
#define DEFAULT_ADDRESS 0

RHT03 sensor;
MicroOLED display(FAKE_RESET_PIN, DEFAULT_ADDRESS);

void setup()
{
    delay(100);
    Wire.begin();
    display.begin();
    display.clear(ALL);
    display.display();
    delay(1000);
    display.clear(PAGE);
    
    display.pixel(10,10);
    display.pixel(11,10);
    display.pixel(11,11);
    display.pixel(10,11);
    display.display();

    Serial.begin(9600);
    sensor.begin(SENSOR_DATA_PIN);
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
