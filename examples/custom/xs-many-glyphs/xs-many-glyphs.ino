#include <Arduino.h>

#define LCD_VIRTUAL_SLOTS 32
#include <Lcd2004xs.h>

Lcd2004xs lcd(0x27, 16, 2);

inline void setDiagBit(uint8_t& bits, const uint8_t row)
{
    bits |= 1 << (7 - row % 8);
}

// 32 frames = 4 stages x 8 frames:
// 0-7   single diagonal, top-left -> bottom-right
// 8-15  single diagonal, top-right -> bottom-left (reversed)
// 16-23 two crossing diagonals (X)
// 24-31 two parallel diagonals
void makeFrame(const uint8_t frame, uint8_t (&symbol)[5])
{
    const uint8_t stage = frame / 8;
    const uint8_t offset = frame % 8;

    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t bits = 0;

        switch (stage)
        {
        case 0:
            setDiagBit(bits, i + offset);
            break;
        case 1:
            setDiagBit(bits, offset + 8 - i);
            break;
        case 2:
            setDiagBit(bits, i + offset);
            setDiagBit(bits, offset + 8 - i);
            break;
        case 3:
            setDiagBit(bits, i + offset);
            setDiagBit(bits, i + offset + 4);
            break;
        }

        symbol[i] = bits;
    }
}

void setup()
{
    lcd.init();

    for (uint8_t i = 0; i < 32; i++)
    {
        uint8_t symbol[5];
        makeFrame(i, symbol);
        lcd.saveCustomChar(i, symbol);
    }

    lcd.print(' '); // placeholder char
}

void loop()
{
    for (uint8_t f = 0; f < 32; f++)
    {
        lcd.print('\b');
        lcd.write(lcd.loadVirtualChar(f));
        delay(150);
    }
}
