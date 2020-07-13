#include <SparkFun_RHT03.h>
#include <Wire.h>
#include <SPI.h>
#include "RF24.h"
#include "respite-sensors.h"
#include "micro_oled.h"

#define SENSOR_DATA_PIN A8

RHT03 sensor;
RF24 radio(5,4);

void draw(int x, int y)
{
    clear_buffer();
    draw_pixel(x-1,y-1,1,NORM);
    draw_pixel(x+1,y+1,1,NORM);
    draw_pixel(x-1,y+1,1,NORM);
    draw_pixel(x+1,y-1,1,NORM);
    
    char text[8];
    sprintf(text,"%d, %d",x,y);
    draw_string(0, 0, text, 1, NORM, 1);
    send_buffer();

    delay(5000);
}

void setupDisplay()
{
    delay(100);
    Wire.begin();

    micro_oled_init();
    clear_screen();
    clear_buffer();

    draw(24,24);
    draw(47,47);
    draw(127,63);
    draw(127,0);
    draw(63,5);
}

void setupRadio()
{
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    uint8_t pipe_name[] = "ANode";
    radio.openWritingPipe(pipe_name);
}

void setup()
{
    setupDisplay();
    setupRadio();
    Serial.begin(9600);
    sensor.begin(SENSOR_DATA_PIN);
}

int tick = 0;

void console_sensor_error_log(String message)
{
    Serial.println(message);
}

void console_sensor_data_log(float temperature, float humidity, bool radio_error)
{
    Serial.println("Tick: " + String(tick));
    Serial.println("Humidity: " + String(humidity, 1) + " %");
    Serial.println("Temp (C): " + String(temperature, 1) + " deg C");
    if (radio_error)
        Serial.println("RADIO ERROR");
}

void broadcast(float temperature, float humidity, bool radio_error)
{
    Message message;
    strcpy(message.location, "Basement");
    message.temperature = temperature;
    message.humidity = humidity;

    radio.write(&message, sizeof(message));
}

void show(float temperature, float humidity, int tick, bool radio_error)
{
    char text[48];

    clear_buffer();

    draw_pixel(0, 0, 1, NORM);
    draw_pixel(1, 0, 1, NORM);
    draw_pixel(0, 1, 1, NORM);
    draw_pixel(1, 1, 1, NORM);
    /*
    sprintf(text, "%d", tick);
    draw_string(0, 0, text, 1, NORM, 1);

    dtostrf(temperature, 4, 1, text);
    strcat(text, " C");
    draw_string(0, 16, text, 1, NORM, 1);

    dtostrf(humidity, 4, 1, text);
    strcat(text, " %");
    draw_string(0, 32, text, 1, NORM, 1);
    */

    send_buffer();
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
        
        show(latestTempC, latestHumidity, tick, radio_error);
        broadcast(latestTempC, latestHumidity, radio_error); 
        console_sensor_data_log(latestTempC, latestHumidity, radio_error);
    }
    else
    {
        console_sensor_error_log("Sensor failed at tick: "+String(tick)); 
    }
    
    delay(RHT_READ_INTERVAL_MS);
}
