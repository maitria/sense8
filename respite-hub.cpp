#include <SparkFun_RHT03.h>
#include <Wire.h>
#include <SFE_MicroOLED.h>
#include <SPI.h>
#include "RF24.h"

#define SERIAL_BAUD_RATE 9600

// we chose pin 8 on the pro micro
#define SENSOR_DATA_PIN A8

// there is no reset pin but the code wants one <eyeroll>
#define FAKE_RESET_PIN A1

#define USE_DEFAULT_I2C_ADDRESS 0

RHT03 sensor;
MicroOLED display(FAKE_RESET_PIN, USE_DEFAULT_I2C_ADDRESS);
RF24 radio(5,4);

void setupDisplay()
{
    delay(100);
    Wire.begin();
    display.begin();
    display.clear(ALL);
    display.clear(PAGE);
    display.setCursor(0,0); 
    display.println("HELLO");
    display.display();
}

void setupRadio()
{
    uint8_t pipe_name[] = "ANode";
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    radio.openReadingPipe(1, pipe_name); 
    radio.startListening();
}

void setup()
{
    setupDisplay();
    setupRadio();
    Serial.begin(SERIAL_BAUD_RATE);
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
    
    #define MESSAGE_SIZE 8
    char message[MESSAGE_SIZE+1] = {};
    if (radio.available()) 
    {
        radio.read(message, MESSAGE_SIZE);
        display.println(message);
        display.display();
    }

    delay(RHT_READ_INTERVAL_MS);
}
