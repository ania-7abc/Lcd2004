#pragma once

#if __has_include(<GyverGFX.h>)
#include <GyverGFX.h>
#else
#error "Add GyverGFX library to your project to use Lcd2004gfx"
#endif

#define LCD_GFX_PPS_X 2

#ifdef LCD_GFX_LARGE
#define LCD_GFX_PPS_Y 3
#define LCD_VIRTUAL_SLOTS 63
#else
#define LCD_GFX_PPS_Y 2
#define LCD_VIRTUAL_SLOTS 15
#endif

#ifndef LCD_MAX_RESOLUTION
#define LCD_MAX_RESOLUTION (20 * 4)
#endif

#define LCD_GFX_BUFFER_SIZE (LCD_MAX_RESOLUTION / 8 * (LCD_GFX_PPS_X * LCD_GFX_PPS_Y))

#include <Lcd2004xs.h>

class Lcd2004gfx : Lcd2004xs, public GyverGFX
{
    uint8_t buffer[LCD_GFX_BUFFER_SIZE] = {};

    void bufferWrite(const uint16_t x, const uint16_t y, const bool value)
    {
        bitWrite(buffer[(x + y * _w) / 8], x % 8, value);
    }

    bool bufferRead(const uint16_t x, const uint16_t y) const
    {
        return bitRead(buffer[(x + y * _w) / 8], x % 8);
    }

public:
    void init()
    {
        Lcd2004xs::init();
        size(cols_ * LCD_GFX_PPS_X, rows_ * LCD_GFX_PPS_Y);

        for (int i = 1; i < LCD_VIRTUAL_SLOTS + 1; i++)
        {
            uint8_t symbol[8] = {};

            // lu, lc, [ld], ru, rc, [rd]

            // @formatter:off
#ifdef LCD_GFX_LARGE
            if (i & 32) { symbol[0] |= 0b11000; symbol[1] |= 0b11000; }
            if (i & 16) { symbol[3] |= 0b11000; symbol[4] |= 0b11000; }
            if (i & 8) { symbol[6] |= 0b11000; symbol[7] |= 0b11000; }
            if (i & 4) { symbol[0] |= 0b00011; symbol[1] |= 0b00011; }
            if (i & 2) { symbol[3] |= 0b00011; symbol[4] |= 0b00011; }
            if (i & 1) { symbol[6] |= 0b00011; symbol[7] |= 0b00011; }
#else
            if (i & 8) { symbol[1] |= 0b11000; symbol[2] |= 0b11000; }
            if (i & 4) { symbol[5] |= 0b11000; symbol[6] |= 0b11000; }
            if (i & 2) { symbol[1] |= 0b00011; symbol[2] |= 0b00011; }
            if (i & 1) { symbol[5] |= 0b00011; symbol[6] |= 0b00011; }
#endif
            // @formatter:on

            saveCustomChar(i - 1, symbol);
        }
    }

    void dot(const int x, const int y, const uint8_t fill = GFX_FILL) override
    {
        bufferWrite(x, y, fill);
    }

    void fill(const uint8_t fill = GFX_FILL) override
    {
        for (uint8_t& i : buffer)
            i = fill == GFX_FILL ? 0xFF : 0;
    }

    void clear() override
    {
        for (uint8_t& i : buffer)
            i = 0;
    }

    void update() override
    {
        for (uint8_t y = 0; y < rows_; y++)
        {
            setPosition(0, y);
            for (uint8_t x = 0; x < cols_; x++)
            {
#ifdef LCD_GFX_LARGE
                const auto lu = bufferRead(x * LCD_GFX_PPS_X, y * LCD_GFX_PPS_Y);
                const auto lc = bufferRead(x * LCD_GFX_PPS_X, y * LCD_GFX_PPS_Y + 1);
                const auto ld = bufferRead(x * LCD_GFX_PPS_X, y * LCD_GFX_PPS_Y + 2);
                const auto ru = bufferRead(x * LCD_GFX_PPS_X + 1, y * LCD_GFX_PPS_Y);
                const auto rc = bufferRead(x * LCD_GFX_PPS_X + 1, y * LCD_GFX_PPS_Y + 1);
                const auto rd = bufferRead(x * LCD_GFX_PPS_X + 1, y * LCD_GFX_PPS_Y + 2);
                const uint8_t code = lu * 32 + lc * 16 + ld * 8 + ru * 4 + rc * 2 + rd;
#else
                const auto lu = bufferRead(x * LCD_GFX_PPS_X, y * LCD_GFX_PPS_Y);
                const auto lc = bufferRead(x * LCD_GFX_PPS_X, y * LCD_GFX_PPS_Y + 1);
                const auto ru = bufferRead(x * LCD_GFX_PPS_X + 1, y * LCD_GFX_PPS_Y);
                const auto rc = bufferRead(x * LCD_GFX_PPS_X + 1, y * LCD_GFX_PPS_Y + 1);
                const uint8_t code = lu * 8 + lc * 4 + ru * 2 + rc;
#endif
                Lcd2004xs::write(code == 0 ? ' ' : loadVirtualChar(code - 1));
            }
        }
#ifdef LCD_USE_BUFFER
        Lcd2004xs::flush();
#endif
    }

    using Lcd2004xs::Lcd2004xs;
};
