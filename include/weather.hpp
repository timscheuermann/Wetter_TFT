#ifndef WEATHER_HPP
#define WEATHER_HPP
#endif

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

class weather
{
    String endpoint;
    String key;

    double temp[40];
    double feels_like[40];
    double tempMin[40];
    double tempMax[40];
    int pressure[40];
    int sea_level[40];
    int grnd_level[40];
    int humidity[40];
    String description[40];
    String date[40];
    double windspeed[40];
    int id[40];
    void setTemp(double inTemp, int day) { temp[day] = inTemp; }
    void setTempMin(double inTempMin, int day) { tempMin[day] = inTempMin; }
    void setTempMax(double inTempMax, int day) { tempMax[day] = inTempMax; }

    String receivefromAPI()
    {
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(1000);
            Serial.println("Connecting to WiFi..");
        }
        Serial.println("Connected to the WiFi network");
        if ((WiFi.status() == WL_CONNECTED)) //Check the current connection status
        {
            HTTPClient http;
            String resp;
            http.begin(endpoint + key); //Specify the URL
            int httpCode = http.GET();  //Make the request
            if (httpCode > 0)           //Check for the returning code
            {
                resp = http.getString();
                Serial.println(httpCode);
                Serial.println(resp);
            }
            else
            {
                Serial.println("Error on HTTP request");
                return "NULL";
            }
            http.end(); //Free the resources
            return resp;
        }
    }

public:
    void begin(const size_t capacity, const String inendpoint, const String inkey)
    {
        endpoint = inendpoint;
        key = inkey;
        String response = receivefromAPI();
        if (response == "NULL")
        {
            Serial.println("weather begin failed");
            return;
        }

        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, response);
        JsonArray list = doc["list"];
        JsonObject list_i[40];
        JsonObject list_i_main[40];
        JsonObject list_i_weather[40];
        for (int i = 0; i <= 39; i++)
        {
            list_i[i] = list[i];
            list_i_main[i] = list_i[i]["main"];
            list_i_weather[i] = list_i[i]["weather"][0];
            temp[i] = list_i_main[i]["temp"];
            tempMin[i] = list_i_main[i]["temp_min"];
            tempMax[i] = list_i_main[i]["temp_max"];
            description[i] = list_i_weather[i]["description"].as<String>();
            id[i] = list_i_weather[i]["id"];
        }
    }

    double getTemp(int day) { return temp[day]; }
    double getTempMin(int day) { return tempMin[day]; }
    double getTempMax(int day) { return tempMax[day]; }
    String getDescription(int day) { return description[day]; }
    int getID(int day) { return id[day]; }
};
