#define LCD_VIRTUAL_SLOTS 32

#include <Lcd2004xs.h>

Lcd2004xs lcd(12, 11, 5, 4, 3, 2, 20, 4);

// Eight rows of a diagonal, rotated over 32 slots
void makeDiagonal(uint8_t offset, uint8_t (&symbol)[8])
{
    for (uint8_t i = 0; i < 8; i++)
    {
        const uint8_t col = (i + offset) % 5;
        symbol[i] = 1 << (4 - col);
    }
}

void setup()
{
    lcd.init();

    for (uint8_t i = 0; i < 32; i++)
    {
        uint8_t symbol[8];
        makeDiagonal(i % 5, symbol);
        lcd.saveCustomChar(i, symbol);
    }

    lcd.print(' ');
}

void loop()
{
    static uint8_t i = 0;
    lcd.print('\b');
    lcd.write(lcd.loadVirtualChar(i));
    delay(200);
    i = (i + 1) % 32;
}
