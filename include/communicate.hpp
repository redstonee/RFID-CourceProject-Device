#pragma once
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>

namespace Communication
{
    WiFiClient wc;

    const String SERVER_ADDR = "http://114.5.14.1:2333";

    bool makeRequest(String addr, JsonDocument *responseJson)
    {
        HTTPClient hc;
        hc.setTimeout(3000);
        hc.begin(wc, addr);
        auto httpCode = hc.GET();
        if (httpCode != HTTP_CODE_OK)
        {
            hc.end();
            Serial.println("Can't connect to server.");
            return false;
        }

        auto responseStr = hc.getString();
        deserializeJson(*responseJson, responseStr);
        hc.end();
        return true;
    }

    bool auth(String uidStr, String *username)
    {
        StaticJsonDocument<100> responseJson;

        auto success = makeRequest(SERVER_ADDR + "/get_username?uid=" + uidStr, &responseJson);
        if (!success)
            return false;

        if (responseJson["status"] == "SUC")
        {
            *username = responseJson["username"].as<String>();
            return true;
        }

        return false;
    }

    bool submitSignIn(String uidStr)
    {
        StaticJsonDocument<100> responseJson;

        auto success = makeRequest(SERVER_ADDR + "/sign_in?uid=" + uidStr, &responseJson);
        if (!success)
            return false;

        return (responseJson["status"] == "SUC");
    }

}