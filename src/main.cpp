#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <XT_DAC_Audio.h>

#include "config.h"
#include "led.hpp"
#include "oled.hpp"
#include "nfc.hpp"
#include "sounds.hpp"
#include "communicate.hpp"

SystemState sState = SystemState::idle;
String username;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Screen::init();
  LedCtrl::init();
  NfcReader::init();
  SoundFx::init();

  Screen::showingSystemStatus = true;

  xTaskCreate(LedCtrl::ledTask, "ledTask", 2000, NULL, 1, NULL);
  xTaskCreate(Screen::screenTask, "screenTask", 4000, NULL, 1, NULL);

  WiFi.begin(WIFI_SSID, WIFI_PSK);
  sState = SystemState::waitWiFi;
  while (!WiFi.isConnected())
  {
    delay(100);
  }
  Serial.println("WiFi Connected");
  sState = SystemState::idle;
  SoundFx::playSound(SoundFx::bootUp);
  while (true)
  {
    if (NfcReader::wakeUp())
      break;
    delay(1000);
    sState = SystemState::nfcError;
  }

  sState = SystemState::idle;
}

String byteArray2HexString(uint8_t *data, uint8_t len)
{
  String ret = "";
  for (uint8_t i = 0; i < len; i++)
  {
    if (data[i] < 0x10)
      ret += "0";
    ret += String(data[i], HEX);
  }
  ret.toUpperCase();
  return ret;
}

void loop()
{

  uint8_t uid[8];
  if (NfcReader::readUID(uid)) // Wait 4 a card
  {
    auto uidStr = byteArray2HexString(uid, 4);
    // Screen::showingSystemStatus = false;

    Serial.println("Found a card!");
    Serial.print("UID Value: ");
    Serial.println(uidStr);

    SoundFx::playSound(SoundFx::foundCard);
    sState = SystemState::authing;

    uint64_t authStartTime = millis();
    if (Communication::auth(uidStr, &username))
    {
      sState = SystemState::authPass;
      Communication::submitSignIn(uidStr);
      SoundFx::playSound(SoundFx::authPass);
    }
    else
    {
      sState = SystemState::authFail;
      SoundFx::playSound(SoundFx::authFail);
    }

    while (millis() - authStartTime < 2000) // Show the info for long enough
    {
      delay(100);
    }
    sState = SystemState::idle;

    // Screen::showingSystemStatus = true;
  }
}
