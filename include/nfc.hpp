#pragma once

#include <Arduino.h>

namespace NfcReader
{
    inline void init()
    {
        Serial2.begin(115200);
    }

    void flushRx()
    {
        while (Serial2.available())
        {
            Serial2.read();
        }
    }

    inline bool wakeUp()
    {
        flushRx();
        uint8_t cmd[] = {
            0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
            
        Serial2.write(cmd, sizeof(cmd));
        uint8_t buf[15];
        uint8_t expected[] = {
            0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 
            0xFF, 0x02, 0xFE, 0xD5, 0x15, 0x16, 0x00};

        uint8_t len = Serial2.readBytes(buf, 15);

        if (len != 15)
            return false;

        for (uint8_t i = 0; i < 15; i++)
        {
            if (buf[i] != expected[i])
                return false;
        }

        return true;
    }

    bool readUID(uint8_t *uid)
    {
        flushRx();
        uint8_t cmd[] = {0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x01, 0x00, 0xE1, 0x00};
        Serial2.write(cmd, sizeof(cmd));
        uint8_t buf[25];
        uint8_t len = Serial2.readBytes(buf, 25);
        if (len == 25)
        {
            memcpy(uid, buf + 19, 4);
            return true;
        }
        return false;
    }
}