#pragma once

#include <Arduino.h>

#define LCD_RU_START 0x0410
#define LCD_RU_END 0x044F
#define LCD_RU_GLYPHS_COUNT 43

#ifdef LCD_PROGMEM_CHARS
#undef LCD_PROGMEM_CHARS
#define LCD_PROGMEM_CHARS LcdRuGlyphs
#endif

const uint8_t LcdRuGlyphs[LCD_RU_GLYPHS_COUNT * 5] PROGMEM = {
    0x7f, 0x49, 0x49, 0x49, 0x79, 0x7f, 0x01, 0x01, 0x01, 0x01, 0x60, 0x3f, 0x21, 0x3f, 0x60, // Б, Г, Д
    0x63, 0x14, 0x7f, 0x14, 0x63, 0x7f, 0x10, 0x08, 0x04, 0x7f, 0x7c, 0x21, 0x12, 0x09, 0x7c, // Ж, И, Й
    0x40, 0x3f, 0x01, 0x01, 0x7f, 0x7f, 0x01, 0x01, 0x01, 0x7f, 0x47, 0x28, 0x10, 0x08, 0x07, // Л, П, У
    0x0f, 0x09, 0x7f, 0x09, 0x0f, 0x3f, 0x20, 0x20, 0x3f, 0x60, 0x0f, 0x08, 0x08, 0x08, 0x7f, // Ф, Ц, Ч
    0x7f, 0x40, 0x7e, 0x40, 0x7f, 0x7f, 0x40, 0x7e, 0x40, 0xff, 0x01, 0x7f, 0x48, 0x48, 0x78, // Ш, Щ, Ъ
    0x7f, 0x48, 0x48, 0x78, 0x7f, 0x7f, 0x48, 0x48, 0x78, 0x00, 0x41, 0x49, 0x49, 0x49, 0x3e, // Ы, Ь, Э
    0x7f, 0x08, 0x7f, 0x41, 0x7f, 0x4f, 0x29, 0x19, 0x09, 0x7f, 0x38, 0x54, 0x54, 0x34, 0x00, // Ю, Я, б
    0x7c, 0x54, 0x54, 0x28, 0x00, 0x7c, 0x04, 0x04, 0x04, 0x00, 0x60, 0x3c, 0x24, 0x3c, 0x60, // в, г, д
    0x6c, 0x10, 0x7c, 0x10, 0x6c, 0x00, 0x54, 0x54, 0x54, 0x28, 0x7c, 0x20, 0x10, 0x08, 0x7c, // ж, з, и
    0x78, 0x22, 0x14, 0x0a, 0x78, 0x40, 0x3c, 0x04, 0x04, 0x7c, 0x7c, 0x10, 0x10, 0x10, 0x7c, // й, л, н
    0x7c, 0x04, 0x04, 0x04, 0x7c, 0x04, 0x04, 0x7c, 0x04, 0x04, 0x38, 0x28, 0x7c, 0x28, 0x38, // п, т, ф
    0x7c, 0x40, 0x40, 0x7c, 0xc0, 0x1c, 0x10, 0x10, 0x10, 0x7c, 0x7c, 0x40, 0x78, 0x40, 0x7c, // ц, ч, ш
    0x7c, 0x40, 0x78, 0x40, 0xfc, 0x04, 0x7c, 0x50, 0x70, 0x00, 0x7c, 0x50, 0x70, 0x00, 0x7c, // щ, ъ, ы
    0x7c, 0x50, 0x70, 0x00, 0x00, 0x54, 0x54, 0x54, 0x38, 0x00, 0x7c, 0x10, 0x7c, 0x44, 0x7c, // ь, э, ю
    0x00, 0x5c, 0x34, 0x14, 0x7c // я
};

// Indexes in the alphabet
// - Numbers (0..9)
// - ? (10), Space (11)
// - ENGLISH (12..37)
// - english (38..63)
// - Glyphs (64..106)
const uint8_t LcdRuIndexes[] PROGMEM = {
    12, 64, 13, 65, 66, 16, 67, 3, 68, 69, // А, Б, В, Г, Д, Е, Ж, З, И, Й
    22, 70, 24, 19, 26, 71, 27, 14, 31, 72, // К, Л, М, Н, О, П, Р, С, Т, У
    73, 35, 74, 75, 76, 77, 78, 79, 80, 81, // Ф, Х, Ц, Ч, Ш, Щ, Ъ, Ы, Ь, Э
    82, 83, 38, 84, 85, 86, 87, 42, 88, 89, // Ю, Я, а, б, в, г, д, е, ж, з
    90, 91, 48, 92, 50, 93, 52, 94, 53, 40, // и, й, к, л, м, н, о, п, р, с
    95, 62, 96, 61, 97, 98, 99, 100, 101, 102, // т, у, ф, х, ц, ч, ш, щ, ъ, ы
    103, 104, 105, 106 // ь, э, ю, я
};

#if LCD_VIRTUAL_SLOTS < LCD_RU_GLYPHS_COUNT
#undef LCD_VIRTUAL_SLOTS
#define LCD_VIRTUAL_SLOTS LCD_RU_GLYPHS_COUNT
#endif
#include <Lcd2004xs.h>

class Lcd2004ru : public Lcd2004xs
{
    // Utf-8 parser //
    bool utf8;
    uint32_t codepoint;
    uint8_t left = 0;

    bool parseUtf8(const uint8_t byte)
    {
        // @formatter:off
        if (left)
        {
            if ((byte & 0xC0) != 0x80) { codepoint = 0xFFFD; left = 0; utf8 = false; return true; } // Invalid
            codepoint = codepoint << 6 | (byte & 0x3F);
            return --left == 0;
        }

        if ((byte & 0x80) == 0)    { codepoint = byte;                  utf8 = false; return true;  } // ASCII
        if ((byte & 0xE0) == 0xC0) { codepoint = byte & 0x1F; left = 1; utf8 = true;  return false; } // 2-byte sequence
        if ((byte & 0xF0) == 0xE0) { codepoint = byte & 0x0F; left = 2; utf8 = true;  return false; } // 3-byte sequence
        if ((byte & 0xF8) == 0xF0) { codepoint = byte & 0x07; left = 3; utf8 = true;  return false; } // 4-byte sequence

        codepoint = 0xFFFD; utf8 = false; return true; // Invalid
        // @formatter:on
    }

protected:
    bool preWrite(uint8_t& byte) override
    {
        if (!parseUtf8(byte))
            return false;
        if (codepoint == 0xFFFD)
        {
            byte = '?';
            return true;
        }
        if (codepoint < LCD_RU_START || codepoint > LCD_RU_END)
            return true;

        const uint8_t index = pgm_read_byte(LcdRuIndexes + codepoint - LCD_RU_START);
        if (index <= 9)
            byte = index + 0x30;
        else if (index == 10)
            byte = '?';
        else if (index == 11)
            byte = ' ';
        else if (index <= 37)
            byte = index - 12 + 0x41;
        else if (index <= 63)
            byte = index - 38 + 0x61;
        else
            byte = loadVirtualChar(index - 64);
        return true;
    }

public:
    using Lcd2004xs::Lcd2004xs;

    void init()
    {
        Lcd2004xs::init();

        for (int i = 0; i < LCD_RU_GLYPHS_COUNT; i++)
        {
            uint8_t symbol[5];
#ifndef LCD_PROGMEM_CHARS
            for (uint8_t b = 0; b < 5; b++)
                symbol[b] = pgm_read_byte(LcdRuGlyphs + i * 5 + b);
#endif
            saveCustomChar(i, symbol);
        }
    }
};
