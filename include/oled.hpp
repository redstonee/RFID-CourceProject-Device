#pragma once

#include <Arduino.h>
#include "config.h"
#include <U8g2lib.h>

extern SystemState sState;
extern String username;

namespace Screen
{
    bool showingSystemStatus = false;
    U8G2_SH1106_128X64_NONAME_F_HW_I2C *u8g2;
    void init()
    {
        u8g2 = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, PIN_SCL, PIN_SDA);
        u8g2->begin();
        u8g2->setFont(u8g2_font_8x13_tf);
        u8g2->clearBuffer();
    }

    inline void clear()
    {
        u8g2->clearBuffer();
    }

    inline void showStr(uint8_t x, uint8_t y, String s)
    {
        u8g2->drawStr(x, y, s.c_str());
    }
    void showSystemStatus()
    {
        u8g2->clearBuffer();
        switch (sState)
        {
        case SystemState::idle:
            u8g2->setCursor(10, 32);
            u8g2->print("Idle");
            break;

        case SystemState::waitWiFi:
            u8g2->setCursor(10, 32);
            u8g2->print("Waiting 4 WiFi");
            break;

        case SystemState::authing:
            u8g2->setCursor(10, 32);
            u8g2->print("Authenticating");
            break;
            
        case SystemState::authFail:
            u8g2->setCursor(10, 32);
            u8g2->print("Auth Fail");
            break;

        case SystemState::authPass:
            u8g2->setCursor(10, 20);
            u8g2->print(username);
            u8g2->setCursor(4, 40);
            u8g2->print("Sign-in Success");
            break;

        case SystemState::nfcError:
            u8g2->setCursor(10, 32);
            u8g2->print("NFC Error");
            break;

        default:
            break;
        }
    }

    void screenTask(void *pvParameters)
    {
        while (1)
        {
            if (showingSystemStatus)
                showSystemStatus();

            u8g2->sendBuffer();
            delay(100);
        }
    }
}