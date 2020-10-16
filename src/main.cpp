/*
  433 MHz RF Module Transmitter BME280
*/

#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1027.54)
//(1013.25)

Adafruit_BME280 bme; // I2C
float hum;    // Stores humidity value in percent
float temp;   // Stores temperature value in Celcius
float press;  // Stores pressor value
float alt;    // Store Altitude value
// Define output strings

String str_humid;
String str_temp;
String str_press;
String str_out;
String str_alt;
// Create Amplitude Shift Keying Object
RH_ASK rf_driver(2000,12,14,0); // GPIO ping 12 and 13 not usung 13

void setup()
{
    Serial.begin(9600);
    Serial.println(F("BME280 Starting"));
    bool status;
    // Initialize ASK Object
    rf_driver.init();
    // Start BME Sensor
    status = bme.begin(0x76);
   if (!status)
   {
     Serial.println("Could not find a valid BME280 sensor, check wiring!");
     while (1);
   }
}

void loop()
{
  delay(20000);  // Delay so sensor can stabalize
    hum = bme.readHumidity();  // Get Humidity value
    temp= bme.readTemperature();  // Get Temperature value
    press= bme.readPressure(); //Get Pressoure
    alt= (bme.readAltitude(SEALEVELPRESSURE_HPA));
    // Convert Humidity to string
    str_humid = String(hum);
    Serial.println("str_humid = " + str_humid);
    // Convert Temperature to string
    str_temp = String(temp);
    Serial.println("str_temp = " + str_temp);
    //Convert  Pressessor
    str_press = String (press / 100.0F);
    Serial.println("str_press = " + str_press);
    //Convert Altitude
    str_alt = String (alt);
    Serial.println("str_alt = " + str_alt);
    // Combine Humidity and Temperature and Pressure
    str_out = str_humid + "," + str_temp + "," + str_press;
    // Compose output character
    const char *msg = str_out.c_str();
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
}
