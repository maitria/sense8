#include <SparkFun_RHT03.h>
#include <Wire.h>
#include <SFE_MicroOLED.h>
#include <SPI.h>
#include "RF24.h"
#include "respite-sensors.h"

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

void show(Message& message)
{
    display.clear(PAGE);
    display.setCursor(0,0); 
    display.println(message.location);
    display.println(message.temperature);
    display.println(message.humidity);
    display.display();
}

void loop()
{
    if (radio.available()) 
    {
        Message message;
        radio.read(&message, sizeof(message));
        show(message);
    }

    delay(RHT_READ_INTERVAL_MS);
}
