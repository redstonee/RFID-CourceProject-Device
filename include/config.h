#pragma once
#include <Arduino.h>

constexpr uint8_t PIN_RGB = 13;

constexpr uint8_t PIN_SDA = 26;
constexpr uint8_t PIN_SCL = 27;

constexpr uint8_t PIN_DAC = 25;

constexpr char WIFI_SSID[] = "Boy Next Door";
constexpr char WIFI_PSK[] = "Fantasies";

enum class SystemState
{
    waitWiFi,
    idle,
    authing,
    authFail,
    authPass,
    nfcError
};