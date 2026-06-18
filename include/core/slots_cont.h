#pragma once

#include <Arduino.h>
#include <Lcd2004.h>

#define LCD_USER_USED 1
#define LCD_FREE 0

class _Lcd2004sc : public Lcd2004
{
protected:
    uint16_t slotCode[8] = {LCD_FREE};
    uint8_t slotCycle = 0;

    uint8_t getFreeSlot(uint16_t code, bool ignore_self = false)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            if (slotCode[i] == code && !ignore_self)
                return i;
            if (slotCode[i] == LCD_FREE)
            {
                slotCode[i] = code;
                return i;
            }
        }

        uint8_t old = slotCycle;
        while (slotCode[slotCycle] == LCD_USER_USED)
        {
            slotCycle = (slotCycle + 1) % 8;
            if (slotCycle == old) // User is greedy and took all slots. Greedy!
                for (int i = 0; i < 8; i++)
                    slotCode[i] = LCD_FREE;
        }
        slotCode[slotCycle] = code;
        return slotCycle;
    }

public:
    using Lcd2004::Lcd2004;

    void saveCustomChar(uint8_t code, uint8_t symbol[8])
    {
        if (slotCode[code] != LCD_FREE && slotCode[code] != LCD_USER_USED)
            slotCode[getFreeSlot(slotCode[code], true)] = slotCode[code]; // move to another place
        slotCode[code] = LCD_USER_USED;
        Lcd2004::saveCustomChar(code, symbol);
    }

    void freeCustomChar(uint8_t code)
    {
        if (slotCode[code] == LCD_USER_USED)
            slotCode[code] = LCD_FREE;
    }
};
