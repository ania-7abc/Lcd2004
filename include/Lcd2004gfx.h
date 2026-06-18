#pragma once

#define LCD_D_USE_BUFFER
#define LCD_D_DISABLE_PRINT
#include <core/slots_cont.h>

#if __has_include(<GyverGFX.h>)
#include <GyverGFX.h>
#else
#error "Add GyverGFX library to your project to use Lcd2004gfx"
#endif

// 20 * 4 / 8 * 4 = 40
// - 20*4 = max resolution
// - /8 = bits in byte
// - *4 = 4 pixels per symbol
#define LCD_GFX_BUFFER_SIZE 40

#define LCD_GFX_PPS_X 2
#ifdef LCD_GFX_LARGE
#define LCD_GFX_PPS_Y 3
#else
#define LCD_GFX_PPS_Y 2
#endif

class Lcd2004gfx : private _Lcd2004sc, public GyverGFX
{

private:
    uint8_t buffer[LCD_GFX_BUFFER_SIZE] = {0};

    void bufferWrite(uint16_t x, uint16_t y, bool value)
    {
        bitWrite(buffer[(x + y * _w) / 8], x % 8, value);
    }

    bool bufferRead(uint16_t x, uint16_t y)
    {
        return bitRead(buffer[(x + y * _w) / 8], x % 8);
    }

#ifdef LCD_GFX_LARGE
    uint8_t generateSymbol(bool lu, bool lc, bool ld, bool ru, bool rc, bool rd)
    {
        uint8_t symbol[8] = {0};
        if (lu)
            for (uint8_t row = 0; row <= 1; row++)
                symbol[row] |= 0b11000;
        if (lc)
            for (uint8_t row = 3; row <= 4; row++)
                symbol[row] |= 0b11000;
        if (ld)
            for (uint8_t row = 6; row <= 7; row++)
                symbol[row] |= 0b11000;

        if (ru)
            for (uint8_t row = 0; row <= 1; row++)
                symbol[row] |= 0b00011;
        if (rc)
            for (uint8_t row = 3; row <= 4; row++)
                symbol[row] |= 0b00011;
        if (rd)
            for (uint8_t row = 6; row <= 7; row++)
                symbol[row] |= 0b00011;
        uint16_t code = lu * 32 + lc * 16 + ld * 8 + ru * 4 + rc * 2 + rd;
        code += 64; // bias
        code = getFreeSlot(code);
        Lcd2004::saveCustomChar(code, symbol);
        return code;
    }
#else
    uint8_t generateSymbol(bool lu, bool ld, bool ru, bool rd)
    {
        uint8_t symbol[8] = {0};
        if (lu)
            for (uint8_t row = 1; row <= 2; row++)
                symbol[row] |= 0b11000;
        if (ld)
            for (uint8_t row = 5; row <= 6; row++)
                symbol[row] |= 0b11000;
        if (ru)
            for (uint8_t row = 1; row <= 2; row++)
                symbol[row] |= 0b00011;
        if (rd)
            for (uint8_t row = 5; row <= 6; row++)
                symbol[row] |= 0b00011;
        uint16_t code = lu * 8 + ru * 4 + ld * 2 + rd;
        code += 16; // bias
        code = getFreeSlot(code);
        Lcd2004::saveCustomChar(code, symbol);
        return code;
    }
#endif

public:
    void init()
    {
        _Lcd2004sc::init();
        size(cols * LCD_GFX_PPS_X, rows * LCD_GFX_PPS_Y);
    }

    void dot(int x, int y, uint8_t fill = GFX_FILL) override
    {
        bufferWrite(x, y, fill);
    }

    void fill(uint8_t fill = GFX_FILL) override
    {
        for (uint8_t i = 0; i < LCD_GFX_BUFFER_SIZE; i++)
            buffer[i] = fill == GFX_FILL ? 0xFF : 0;
    }

    void update() override
    {
        for (uint8_t y = 0; y < rows; y++)
        {
            _Lcd2004sc::setPosition(0, y);
            for (uint8_t x = 0; x < cols; x++)
            {
#ifdef LCD_GFX_LARGE
                bool lu = bufferRead(x * LCD_GFX_PPS_X, y * LCD_GFX_PPS_Y);
                bool lc = bufferRead(x * LCD_GFX_PPS_X, y * LCD_GFX_PPS_Y + 1);
                bool ld = bufferRead(x * LCD_GFX_PPS_X, y * LCD_GFX_PPS_Y + 2);
                bool ru = bufferRead(x * LCD_GFX_PPS_X + 1, y * LCD_GFX_PPS_Y);
                bool rc = bufferRead(x * LCD_GFX_PPS_X + 1, y * LCD_GFX_PPS_Y + 1);
                bool rd = bufferRead(x * LCD_GFX_PPS_X + 1, y * LCD_GFX_PPS_Y + 2);
                uint8_t code = generateSymbol(lu, lc, ld, ru, rc, rd);
#else
                bool lu = bufferRead(x * LCD_GFX_PPS_X, y * LCD_GFX_PPS_Y);
                bool ld = bufferRead(x * LCD_GFX_PPS_X, y * LCD_GFX_PPS_Y + 1);
                bool ru = bufferRead(x * LCD_GFX_PPS_X + 1, y * LCD_GFX_PPS_Y);
                bool rd = bufferRead(x * LCD_GFX_PPS_X + 1, y * LCD_GFX_PPS_Y + 1);
                uint8_t code = generateSymbol(lu, ld, ru, rd);
#endif
                _Lcd2004sc::write(code);
            }
        }
        _Lcd2004sc::flush();
    }

    using _Lcd2004sc::_Lcd2004sc;
};
