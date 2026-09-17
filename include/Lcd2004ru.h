#pragma once

#include <Arduino.h>

#if LCD_VIRTUAL_SLOTS < 43
#undef LCD_VIRTUAL_SLOTS
#define LCD_VIRTUAL_SLOTS 43
#endif

#include <Lcd2004xs.h>

#define CANONICAL_GLYPH_GROUPS_SIZE 21
const uint16_t canonicalGlyphGroups[CANONICAL_GLYPH_GROUPS_SIZE][2] PROGMEM = {
    {0x0041, 0x0410}, {0x0042, 0x0412}, {0x0043, 0x0421}, {0x0045, 0x0415}, {0x0048, 0x041D},
    {0x004B, 0x041A}, {0x004D, 0x041C}, {0x004F, 0x041E}, {0x0050, 0x0420}, {0x0054, 0x0422},
    {0x0058, 0x0425}, {0x0061, 0x0430}, {0x0063, 0x0441}, {0x0065, 0x0435}, {0x006B, 0x043A},
    {0x006D, 0x043C}, {0x006F, 0x043E}, {0x0070, 0x0440}, {0x0078, 0x0445}, {0x0079, 0x0443},
    {0x0033, 0x0417},
};

const uint8_t uppercaseGlyphNums[32] PROGMEM = {
    0, 0, 0, 1, 2, 0, 3, 0, 4, 5, 0, 6, 0, 0, 0, 7, 0, 0, 0, 8, 9, 0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
};
const uint8_t uppercaseGlyphs[] PROGMEM = {
    0x7f, 0x49, 0x49, 0x49, 0x79, 0x7f, 0x01, 0x01, 0x01, 0x01, 0x60, 0x3f, 0x21, 0x3f, 0x60, 0x63, 0x14, 0x7f, 0x14,
    0x63, 0x7f, 0x10, 0x08, 0x04, 0x7f, 0x7c, 0x21, 0x12, 0x09, 0x7c, 0x40, 0x3f, 0x01, 0x01, 0x7f, 0x7f, 0x01, 0x01,
    0x01, 0x7f, 0x47, 0x28, 0x10, 0x08, 0x07, 0x0f, 0x09, 0x7f, 0x09, 0x0f, 0x3f, 0x20, 0x20, 0x3f, 0x60, 0x0f, 0x08,
    0x08, 0x08, 0x7f, 0x7f, 0x40, 0x7e, 0x40, 0x7f, 0x7f, 0x40, 0x7e, 0x40, 0xff, 0x01, 0x7f, 0x48, 0x48, 0x78, 0x7f,
    0x48, 0x48, 0x78, 0x7f, 0x7f, 0x48, 0x48, 0x78, 0x00, 0x41, 0x49, 0x49, 0x49, 0x3e, 0x7f, 0x08, 0x7f, 0x41, 0x7f,
    0x4f, 0x29, 0x19, 0x09, 0x7f
};

const uint8_t lowercaseGlyphsNums[32] PROGMEM = {
    0, 0, 1, 2, 3, 0, 4, 5, 6, 7, 0, 8, 0, 9, 0, 10, 0, 0, 11, 12, 13, 0, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
};
const uint8_t lowercaseGlyphs[] PROGMEM = {
    0x38, 0x54, 0x54, 0x34, 0x00, 0x7c, 0x54, 0x54, 0x28, 0x00, 0x7c, 0x04, 0x04, 0x04, 0x00, 0x60, 0x3c, 0x24, 0x3c,
    0x60, 0x6c, 0x10, 0x7c, 0x10, 0x6c, 0x00, 0x54, 0x54, 0x54, 0x28, 0x7c, 0x20, 0x10, 0x08, 0x7c, 0x78, 0x22, 0x14,
    0x0a, 0x78, 0x40, 0x3c, 0x04, 0x04, 0x7c, 0x7c, 0x10, 0x10, 0x10, 0x7c, 0x7c, 0x04, 0x04, 0x04, 0x7c, 0x04, 0x04,
    0x7c, 0x04, 0x04, 0x44, 0x28, 0x10, 0x08, 0x04, 0x38, 0x28, 0x7c, 0x28, 0x38, 0x7c, 0x40, 0x40, 0x7c, 0xc0, 0x1c,
    0x10, 0x10, 0x10, 0x7c, 0x7c, 0x40, 0x78, 0x40, 0x7c, 0x7c, 0x40, 0x78, 0x40, 0xfc, 0x04, 0x7c, 0x50, 0x70, 0x00,
    0x7c, 0x50, 0x70, 0x00, 0x7c, 0x7c, 0x50, 0x70, 0x00, 0x00, 0x54, 0x54, 0x54, 0x38, 0x00, 0x7c, 0x10, 0x7c, 0x44,
    0x7c, 0x00, 0x5c, 0x34, 0x14, 0x7c
};

class Lcd2004ru : public Lcd2004xs
{
    // Utf-8 Parser //
    bool utf8;
    uint16_t buf;
    uint8_t remaining = 0;

    bool parseUtf8(const uint8_t byte)
    {
        if ((byte & 0x80) == 0 || (byte & 0xF8) == 0xF0) // ASCII & 4-byte sequence
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
        else if ((byte & 0xC0) == 0x80) // Symbol continuation
        {
            mask = 0x3F;
            remaining--;
        }
        else // Invalid
            return true;
        buf <<= 6;
        buf |= byte & mask;
        return remaining == 0;
    }

    // Get canonical glyph //
    static uint16_t getCanonical(uint16_t code)
    {
        for (auto canonicalGlyphGroup : canonicalGlyphGroups)
        {
            auto g0 = pgm_read_word(&canonicalGlyphGroup[0]);
            auto g1 = pgm_read_word(&canonicalGlyphGroup[1]);
            if (g0 == code || g1 == code)
                return g0;
        }
        return code;
    }

    static uint8_t toVirtualCode(const uint16_t code)
    {
        if (code >= 0x0410 && code <= 0x042F)
        {
            uint8_t idx = 0;
            for (uint16_t c = 0x0410; c < code; ++c)
                if (getCanonical(c) == c)
                    ++idx;

            return LCD_VIRTUAL_SLOTS - 43 + idx;
        }

        if (code >= 0x0430 && code <= 0x044F)
        {
            uint8_t idx = 20;
            for (uint16_t c = 0x0430; c < code; ++c)
                if (getCanonical(c) == c)
                    ++idx;

            return (LCD_VIRTUAL_SLOTS - 43 + idx);
        }

        return (code & 0x3F) % LCD_VIRTUAL_SLOTS - 43;
    }

protected:
    // pre-Write //
    bool preWrite(uint8_t& byte) override
    {
        if (!parseUtf8(byte))
            return false;
        if (!utf8)
            return true;

        buf = getCanonical(buf);
        if (buf <= 0x7F)
        {
            byte = buf;
            return true;
        }

        byte = loadVirtualChar(toVirtualCode(buf));
        return true;
    }

public:
    using Lcd2004xs::Lcd2004xs;

    void init()
    {
        Lcd2004xs::init();

        for (uint16_t code = 0x0410; code <= 0x044F; ++code)
        {
            if (getCanonical(code) != code)
                continue;
            auto virtual_code = toVirtualCode(code);
            if (!isRegistered(virtual_code))
            {
                uint8_t symbol[8], scan[5];
                const uint8_t* glyphs = nullptr;
                uint8_t index = 0;

                if (code >= 0x0410 && code <= 0x042F)
                {
                    index = pgm_read_byte(&uppercaseGlyphNums[code - 0x0410]) * 5;
                    glyphs = uppercaseGlyphs;
                }
                else
                {
                    index = pgm_read_byte(&lowercaseGlyphsNums[code - 0x0430]) * 5;
                    glyphs = lowercaseGlyphs;
                }

                for (uint8_t byte = 0; byte < 5; byte++)
                    scan[byte] = pgm_read_byte(glyphs + index + byte);

                for (uint8_t j = 0; j < 8; j++)
                {
                    uint8_t val = 0;
                    for (uint8_t i = 0; i < 5; i++)
                        val |= (scan[4 - i] >> j & 1) << i;
                    symbol[j] = val;
                }

                saveCustomChar(virtual_code, symbol);
            }
        }
    }
};
