#include <Lcd2004.h>

Lcd2004 lcd(12, 11, 5, 4, 3, 2, 20, 4);

// A filled square with a hollow center
constexpr uint8_t Square[] = {0xFF, 0xFF, 0xC3, 0xFF, 0xFF};

void setup()
{
    lcd.init();

    lcd.saveCustomChar(1, Square);
    lcd.println("Glyph: \1");
}

void loop()
{
}
