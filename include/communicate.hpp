#pragma once

#include <Arduino.h>

namespace Communication
{
    bool auth(String uidStr, String *username)
    {
        delay(300);
        if (uidStr == "d4553f83")
        {
            *username = "RedStoneee";
            return true;
        }
        return false;
    }
}