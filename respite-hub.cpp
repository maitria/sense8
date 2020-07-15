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

Message remote_data;
Message local;

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
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.cp437(true);
    display.clearDisplay();
}

void setup()
{
    setupRadio();
    Serial.begin(9600);
    sensor.begin(SENSOR_DATA_PIN);
    setupDisplay();
}

void show_data(Message& message)
{
    String temperatureF((message.temperature * 9/5) + 32, 1);
    temperatureF += " \xF8" "F";
    String humidityInfo(message.humidity, 1);
    humidityInfo += "% HUM";

    display.clearDisplay();
    display.setCursor(0,0); 
    display.println(message.location);
    display.println();
    display.println(temperatureF);
    display.println(humidityInfo);
    display.display();
    delay(3000);
}

void loop()
{
    if (radio.available()) 
    {
        radio.read(&remote_data, sizeof(remote_data));
        show_data(remote_data);
    }

    int updateResult = sensor.update();
    bool sensor_succeeded = updateResult == 1;
    if (sensor_succeeded)
    {
        local.humidity = sensor.humidity();
        local.temperature = sensor.tempC();
        strcpy(local.location, "Main");
        show_data(local);
    }
}
