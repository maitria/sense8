#include <SparkFun_RHT03.h>
#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "sense8.h"

#define SENSOR_DATA_PIN A8
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

RHT03 sensor;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RF24 radio(5,4);

SensorData remote_data;
SensorData local;

String rightSize(String s, int goal)
{
    if(s.length() > goal)
        return s.substring(0, goal);
    while (s.length() < goal)   
        s += " ";
    return s;
}

void setupRadio()
{
    uint8_t pipe_name[] = "ANode";
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    radio.openReadingPipe(1, pipe_name);
    radio.startListening();
}

void setupDisplay()
{
    display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS);
    display.setRotation(2);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.cp437(true);
    display.clearDisplay();
}

void setup()
{
    setupRadio();
    Serial.begin(9600);
    sensor.begin(SENSOR_DATA_PIN);
    strcpy(local.location, "Main");
    setupDisplay();
}

void show_data()
{
    String local_data =  rightSize(String(local.location), 11) + " " + local.displayTemperature() + " " + local.displayHumidity();
    String remote = rightSize(String(remote_data.location), 11) + " " + remote_data.displayTemperature() + " " + remote_data.displayHumidity();
    display.clearDisplay();
    display.setCursor(0,0);

    display.println(local_data);
    display.println(remote);

    display.display();
    delay(3000);
}

void loop()
{
    int updateResult = sensor.update();
    bool sensor_succeeded = updateResult == 1;
    if (sensor_succeeded)
    {
        local.humidity = sensor.humidity();
        local.temperature = sensor.tempC();
    }

    if (radio.available())
    {
        radio.read(&remote_data, sizeof(remote_data));
    }

    show_data();
}
