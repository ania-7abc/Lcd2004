#pragma once

#include <Arduino.h>
#include <lcd2004.h>

#define LCD_USER_USED 1
#define LCD_FREE 0

#define CANONICAL_GLYPH_GROUPS_SIZE 21
const uint16_t canonicalGlyphGroups[CANONICAL_GLYPH_GROUPS_SIZE][2] PROGMEM = {
    {0x0041, 0x0410},
    {0x0042, 0x0412},
    {0x0043, 0x0421},
    {0x0045, 0x0415},
    {0x0048, 0x041D},
    {0x004B, 0x041A},
    {0x004D, 0x041C},
    {0x004F, 0x041E},
    {0x0050, 0x0420},
    {0x0054, 0x0422},
    {0x0058, 0x0425},
    {0x0061, 0x0430},
    {0x0063, 0x0441},
    {0x0065, 0x0435},
    {0x006B, 0x043A},
    {0x006D, 0x043C},
    {0x006F, 0x043E},
    {0x0070, 0x0440},
    {0x0078, 0x0445},
    {0x0079, 0x0443},
    {0x0033, 0x0417},
};

const uint8_t bigGlyphNums[32] PROGMEM = {
    0, 0, 0, 1, 2, 0, 3, 0, 4, 5, 0, 6, 0, 0, 0, 7, 0, 0, 0, 8, 9, 0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
const uint8_t bigGlyphs[] PROGMEM = {
    0x7f, 0x49, 0x49, 0x49, 0x79, 0x7f, 0x01, 0x01, 0x01, 0x01, 0x60, 0x3f, 0x21, 0x3f, 0x60, 0x63, 0x14, 0x7f, 0x14, 0x63, 0x7f, 0x10, 0x08, 0x04,
    0x7f, 0x7c, 0x21, 0x12, 0x09, 0x7c, 0x40, 0x3f, 0x01, 0x01, 0x7f, 0x7f, 0x01, 0x01, 0x01, 0x7f, 0x47, 0x28, 0x10, 0x08, 0x07, 0x0f, 0x09, 0x7f,
    0x09, 0x0f, 0x3f, 0x20, 0x20, 0x3f, 0x60, 0x0f, 0x08, 0x08, 0x08, 0x7f, 0x7f, 0x40, 0x7e, 0x40, 0x7f, 0x7f, 0x40, 0x7e, 0x40, 0xff, 0x01, 0x7f,
    0x48, 0x48, 0x78, 0x7f, 0x48, 0x48, 0x78, 0x7f, 0x7f, 0x48, 0x48, 0x78, 0x00, 0x41, 0x49, 0x49, 0x49, 0x3e, 0x7f, 0x08, 0x7f, 0x41, 0x7f, 0x4f,
    0x29, 0x19, 0x09, 0x7f};

const uint8_t littleGlyphsNums[32] PROGMEM = {
    0, 0, 1, 2, 3, 0, 4, 5, 6, 7, 0, 8, 0, 9, 0, 10, 0, 0, 11, 12, 13, 0, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
const uint8_t littleGlyphs[] PROGMEM = {
    0x38, 0x54, 0x54, 0x34, 0x00, 0x7c, 0x54, 0x54, 0x28, 0x00, 0x7c, 0x04, 0x04, 0x04, 0x00, 0x60, 0x3c, 0x24, 0x3c, 0x60, 0x6c, 0x10, 0x7c, 0x10,
    0x6c, 0x00, 0x54, 0x54, 0x54, 0x28, 0x7c, 0x20, 0x10, 0x08, 0x7c, 0x78, 0x22, 0x14, 0x0a, 0x78, 0x40, 0x3c, 0x04, 0x04, 0x7c, 0x7c, 0x10, 0x10,
    0x10, 0x7c, 0x7c, 0x04, 0x04, 0x04, 0x7c, 0x04, 0x04, 0x7c, 0x04, 0x04, 0x44, 0x28, 0x10, 0x08, 0x04, 0x38, 0x28, 0x7c, 0x28, 0x38, 0x7c, 0x40,
    0x40, 0x7c, 0xc0, 0x1c, 0x10, 0x10, 0x10, 0x7c, 0x7c, 0x40, 0x78, 0x40, 0x7c, 0x7c, 0x40, 0x78, 0x40, 0xfc, 0x04, 0x7c, 0x50, 0x70, 0x00, 0x7c,
    0x50, 0x70, 0x00, 0x7c, 0x7c, 0x50, 0x70, 0x00, 0x00, 0x54, 0x54, 0x54, 0x38, 0x00, 0x7c, 0x10, 0x7c, 0x44, 0x7c, 0x00, 0x5c, 0x34, 0x14, 0x7c};

class Lcd2004ru : public Lcd2004
{
private:
    // Utf-8 Parser //
    bool utf8;
    uint16_t buf;
    uint8_t remaining = 0;

    bool parseUtf8(uint8_t byte)
    {
        if ((byte & 0x80) == 0 || (byte & 0xF8) == 0xF0) // ACII & 4-byte sequence
        {
            buf = byte;
            utf8 = false;
            return true;
        }
        uint8_t mask;
        if ((byte & 0xE0) == 0xC0) // 2-byte sequence
        {
            buf = 0;
            mask = 0x1F;
            remaining = 1;
            utf8 = true;
        }
        else if ((byte & 0xF0) == 0xE0) // 3-byte sequence
        {
            buf = 0;
            mask = 0x0F;
            remaining = 2;
            utf8 = true;
        }
        else if ((byte & 0xC0) == 0x80) // Symbol continue
        {
            mask = 0x3F;
            remaining--;
        }
        buf <<= 6;
        buf |= byte & mask;
        return remaining == 0;
    }

    // Get CGRAM slot logic //
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

    // Get canonical glyph //
    uint16_t getCanonical(uint16_t code)
    {
        for (uint8_t i = 0; i < CANONICAL_GLYPH_GROUPS_SIZE; i++)
        {
            auto g0 = pgm_read_word(&canonicalGlyphGroups[i][0]);
            auto g1 = pgm_read_word(&canonicalGlyphGroups[i][1]);
            if (g0 == code || g1 == code)
                return g0;
        }
        return code;
    }

    // Get glyph //
    void getGlyph(uint16_t code, uint8_t (&symbol)[8])
    {
        uint8_t scan[5];
        if (code >= 0x0410 && code <= 0x042F)
        {
            uint8_t num = pgm_read_byte(&bigGlyphNums[code - 0x0410]) * 5;
            for (int i = 0; i < 5; i++)
                scan[i] = pgm_read_byte(&bigGlyphs[num + i]);
        }
        else if (code >= 0x0430 && code <= 0x044F)
        {
            uint8_t num = pgm_read_byte(&littleGlyphsNums[code - 0x0430]) * 5;
            for (int i = 0; i < 5; i++)
                scan[i] = pgm_read_byte(&littleGlyphs[num + i]);
        }
        else
        {
            scan[0] = code & 0xF;
            scan[1] = code >> 8;
            scan[2] = 0;
            scan[3] = 0;
            scan[4] = 0;
        }

        for (int j = 0; j < 8; j++)
        {
            uint8_t val = 0;
            for (int i = 0; i < 5; i++)
                if (scan[4 - i] & (1 << j))
                    val |= (1 << i);
            symbol[j] = val;
        }
    }

    // pre-Write //
    bool preWrite(uint8_t &byte) override
    {
        if (!parseUtf8(byte))
            return false;
        if (!utf8)
            return true;
        buf = getCanonical(buf);
        if (buf > 0x7F)
        {
            uint8_t slot = getFreeSlot(buf);
            uint8_t symbol[8] = {0};
            getGlyph(buf, symbol);
            Lcd2004::saveCustomChar(slot, symbol);
            byte = slot;
        }
        else
            byte = buf;
        return true;
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
};
