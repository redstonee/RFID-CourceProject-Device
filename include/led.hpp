#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"
#include <WiFi.h>

extern SystemState sState;

namespace LedCtrl
{
    Adafruit_NeoPixel *rgb;

    void init()
    {
        rgb = new Adafruit_NeoPixel(1, PIN_RGB);
        rgb->begin();
        rgb->setBrightness(20);
    }

    void showHSV(uint8_t h, uint8_t s = 255, uint8_t v = 255)
    {
        rgb->setPixelColor(0, Adafruit_NeoPixel::ColorHSV(h * 1000, s, v));
        rgb->show();
    }

    void blink(uint16_t h, uint8_t delayTime = 5)
    {
        for (uint8_t v = 0; v < 255; v++)
        {
            showHSV(h, 255, v);
            delay(delayTime);
        }
        for (uint8_t v = 255; v > 0; v--)
        {
            showHSV(h, 255, v);
            delay(delayTime);
        };
    }

    void ledTask(void *pvParameters)
    {
        while (1)
        {
            switch (sState)
            {
            case SystemState::idle:
                static uint8_t idleLedHue = 0;
                showHSV(idleLedHue++, 255, 50);
                if (idleLedHue >= 65)
                    idleLedHue = 0;
                delay(100);
                break;

            case SystemState::waitWiFi:
                blink(50);
                break;

            case SystemState::authing:
                rgb->clear();
                rgb->show();
                delay(100);
                break;

            case SystemState::authFail:
                blink(4, 1);
                break;

            case SystemState::authPass:
                blink(23, 1);
                break;

            case SystemState::nfcError:
                blink(0, 3);

            default:
                break;
            }
        }
    }
}