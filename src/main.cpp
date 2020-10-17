/*
  433 MHz RF Module Transmitter BME280
*/

#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)

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
    hum = bme.readHumidity();  // Get Humidity value
    temp= bme.readTemperature();  // Get Temperature value
    press= bme.readPressure(); //Get Pressoure
    alt= (bme.readAltitude(SEALEVELPRESSURE_HPA)); //Get Pressure
    str_humid = "humidity " + String(hum);// Convert Humidity to string
    Serial.println(str_humid);
    str_temp = "temperature " + String(temp); // Convert Temperature to string
    Serial.println(str_temp);
    str_press ="pressoure " + String (press / 100.0F); //Convert  Pressoure
    Serial.println(str_press);
    str_alt = "altitude " + String (alt); //Convert Altitude
    Serial.println(str_alt);
    str_out = str_humid + "," + str_temp + "," + str_press; // Combine Humidity, Temperature, Pressure and Altitude
    const char *msg = str_out.c_str(); // Compose output character
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    delay(20000);  // Delay so sensor can stabalize
}
