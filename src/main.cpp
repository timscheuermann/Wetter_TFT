#include <Arduino.h>
#include "weather.hpp"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <time.h>
#include "login.hpp";

//Please uncomment following variables and give them your data
/*const char* ssid = "wlan_ssid";
const char* password = "wlan_password";

const String endpoint = "http://api.openweathermap.org/data/2.5/forecast?id=?????????&APPID=";
const String key = "openweathemap_key";
*/


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

const size_t capacity = 40 * JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(40) + 86 * JSON_OBJECT_SIZE(1) + 41 * JSON_OBJECT_SIZE(2) + 40 * JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 74 * JSON_OBJECT_SIZE(9) + 6 * JSON_OBJECT_SIZE(10) + 2440;

TFT_eSPI tft = TFT_eSPI();
unsigned long targetTime = 0;


void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  weather w;
  w.begin(capacity, endpoint, key);


  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  tft.init();
  tft.setRotation(2);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  //tft.drawString(w.getDescription(20),0,0,2);
  tft.drawString("Heute:  " + String(w.getTemp(0)-273.15)  +  "C" ,0,0,2);
  tft.drawString(" Min:   " +  String(w.getTempMin(0)-273.15) + "C",0,30,2);
  tft.drawString(" Max:   " +  String(w.getTempMax(0)-273.15) + "C",0,60,2);
  tft.drawString(String(w.getDescription(0)),0,90,2);

  tft.drawString("Morgen: " + String(w.getTemp(3)-273.15)  +  "C" ,0,120,2);
  tft.drawString(" Min:   " +  String(w.getTempMin(3)-273.15) + "C",0,150,2);
  tft.drawString(" Max:   " +  String(w.getTempMax(3)-273.15) + "C",0,180,2);
  tft.drawString(String(w.getDescription(3)),0,210,2);

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
  }
  /*tft.drawString(String(timeinfo.tm_mday),0,240,2);
  tft.drawString(String(timeinfo.tm_mon + 1),0,270,2);
  tft.drawString(String(1900 + timeinfo.tm_year),0,300,2);*/
  
  tft.drawString("Uhrzeit: " + String(timeinfo.tm_hour + 1) + ":" + String(timeinfo.tm_min +1) + ":" + String(1+timeinfo.tm_sec),0,300,2);
  delay(1000);
  
}
void loop()
{
  
}

/*
Serial.print("Temperatur20: ");
  Serial.println(w.getTemp(20) - 273.15);
  Serial.print("Description: ");
  Serial.println(w.getDescription(20));
  Serial.println(w.getID(20));*/