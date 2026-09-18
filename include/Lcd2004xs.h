#pragma once

#include <Arduino.h>
#include <Lcd2004.h>

#ifdef LCD_LOW_MEM
#error "Can't use LCD_LOW_MEM with Lcd2004xs"
#endif

#ifndef LCD_VIRTUAL_SLOTS
#define LCD_VIRTUAL_SLOTS 16
#endif

#define LCD_HW_SLOTS 8
#define LCD_NO_HW_SLOT 255

class Lcd2004xs : public Lcd2004
{
protected:
#ifndef LCD_PROGMEM_CHARS
    uint8_t bitmap_[LCD_VIRTUAL_SLOTS][5] = {};
#endif
    uint8_t registered_[LCD_VIRTUAL_SLOTS / 8] = {};

    uint8_t hwCode_[LCD_HW_SLOTS] = {
        LCD_NO_HW_SLOT, LCD_NO_HW_SLOT, LCD_NO_HW_SLOT, LCD_NO_HW_SLOT,
        LCD_NO_HW_SLOT, LCD_NO_HW_SLOT, LCD_NO_HW_SLOT, LCD_NO_HW_SLOT
    };
    uint8_t hwAge_[LCD_HW_SLOTS] = {};
    uint8_t clock_ = 0;

    uint8_t findHwSlot(const uint8_t code) const
    {
        for (uint8_t i = 0; i < LCD_HW_SLOTS; i++)
            if (hwCode_[i] == static_cast<int16_t>(code))
                return i;
        return LCD_NO_HW_SLOT;
    }

    uint8_t pickSlot() const
    {
        for (uint8_t i = 0; i < LCD_HW_SLOTS; i++)
            if (hwCode_[i] == LCD_NO_HW_SLOT)
                return i;

        uint8_t victim = 0;
        for (uint8_t i = 1; i < LCD_HW_SLOTS; i++)
            if (hwAge_[i] < hwAge_[victim])
                victim = i;
        return victim;
    }

    void touch(const uint8_t slot) { hwAge_[slot] = ++clock_; }

public:
    using Lcd2004::Lcd2004;

    void saveCustomChar(const uint8_t code, const uint8_t symbol[5])
    {
#ifndef LCD_PROGMEM_CHARS
        memcpy(bitmap_[code], symbol, 5);
#endif
        bitSet(registered_[code / 8], code % 8);
    }

    void freeCustomChar(const uint8_t code)
    {
        bitClear(registered_[code / 8], code % 8);

        const auto slot = findHwSlot(code);
        if (slot != LCD_NO_HW_SLOT)
            hwCode_[slot] = LCD_NO_HW_SLOT;
    }

    uint8_t loadVirtualChar(uint8_t code)
    {
        const auto slot = findHwSlot(code);
        if (slot != LCD_NO_HW_SLOT)
        {
            touch(slot);
            return slot;
        }

        const auto newSlot = pickSlot();
        hwCode_[newSlot] = code;
        touch(newSlot);
#ifdef LCD_PROGMEM_CHARS
        uint8_t bitmap_[5][1];
        memcpy_P(bitmap_, LCD_PROGMEM_CHARS + code * 5, 5);
        code = 0;
#endif
        Lcd2004::saveCustomChar(newSlot, bitmap_[code]);
        return newSlot;
    }

    bool isRegistered(const uint8_t code) const { return bitRead(registered_[code / 8], code % 8); }
    bool isLoaded(const uint8_t code) const { return findHwSlot(code) != LCD_NO_HW_SLOT; }
};
