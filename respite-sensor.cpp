#include <SparkFun_RHT03.h>
#include <Wire.h>
#include <SFE_MicroOLED.h>
#include <SPI.h>
#include "RF24.h"

#define SENSOR_DATA_PIN A8
#define FAKE_RESET_PIN A1
#define DEFAULT_ADDRESS 0

RHT03 sensor;
MicroOLED display(FAKE_RESET_PIN, DEFAULT_ADDRESS);
RF24 radio(5,4);

void setupDisplay()
{
    delay(100);
    Wire.begin();
    display.begin();
    display.clear(ALL);
    display.clear(PAGE);
    display.setCursor(0,0); 
    display.println("READY");
    display.display();
}

void setupRadio()
{
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
}

void setup()
{
    setupDisplay();
    setupRadio();
    Serial.begin(9600);
    sensor.begin(SENSOR_DATA_PIN);
}

int tick = 0;

void broadcast(float temperature, float humidity, bool radio_error)
{
    Serial.println("Tick: " + String(tick));
    Serial.println("Humidity: " + String(humidity, 1) + " %");
    Serial.println("Temp (C): " + String(temperature, 1) + " deg C");
    if (radio_error)
        Serial.println("RADIO ERROR");
}

void show(float temperature, float humidity, int tick, bool radio_error)
{
    display.clear(PAGE);
    display.setCursor(0,0); 
    display.println(String(tick));
    display.println();
    display.println(String(temperature, 1) + " C");
    display.println(String(humidity, 1) + " %"); 
    if (radio_error)
        display.println("RADIO ERR");

    display.display();
}

void loop()
{
    bool radio_error = false;
    if (!radio.isPVariant())
        radio_error = true;

    tick ++;
    int updateResult = sensor.update();
    bool succeeded = updateResult == 1;

    if (succeeded)
    {
        float latestHumidity = sensor.humidity();
        float latestTempC = sensor.tempC();
        float latestTempF = sensor.tempF();
        
        broadcast(latestTempC, latestHumidity, radio_error); 
        show(latestTempC, latestHumidity, tick, radio_error);
    }
    else
    {
        Serial.println("Failed attempt at tick: " + String(tick));
    }
    
    delay(RHT_READ_INTERVAL_MS);
}
