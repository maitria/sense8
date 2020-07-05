#include <SparkFun_RHT03.h>
#include <Wire.h>
#include <SFE_MicroOLED.h>

#define SENSOR_DATA_PIN A8
#define FAKE_RESET_PIN A1
#define DEFAULT_ADDRESS 0

RHT03 sensor;
MicroOLED display(FAKE_RESET_PIN, DEFAULT_ADDRESS);

void setupDisplay()
{
    delay(100);
    Wire.begin();
    display.begin();
    display.clear(ALL);
}

void setup()
{
    setupDisplay();
    Serial.begin(9600);
    sensor.begin(SENSOR_DATA_PIN);
}

int tick = 0;

void communicate(float temperature, float humidity)
{
    Serial.println("Tick: " + String(tick));
    Serial.println("Humidity: " + String(humidity, 1) + " %");
    Serial.println("Temp (F): " + String(temperature, 1) + " deg F");
    //Serial.println("Temp (C): " + String(latestTempC, 1) + " deg C");
}

void show(float temperature, float humidity, int tick)
{
    display.clear(PAGE);
    display.setCursor(0,0); 
    display.println(String(tick));
    display.println();
    display.println(String(temperature, 1) + " F");
    display.println(String(humidity, 1) + " %"); 

    display.display();
}

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
        
        communicate(latestTempF, latestHumidity); 
        show(latestTempF, latestHumidity, tick);
    }
    else
    {
        Serial.println("Failed attempt at tick: " + String(tick));
    }
    
    delay(RHT_READ_INTERVAL_MS);
}
