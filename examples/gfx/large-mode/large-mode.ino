#define LCD_GFX_LARGE
#include <Lcd2004gfx.h>

Lcd2004gfx lcd(12, 11, 5, 4, 3, 2, 20, 4);

void setup()
{
    lcd.init();
    lcd.clear();

    lcd.line(0, 0, 39, 11);
    lcd.line(0, 11, 39, 0);

    lcd.update();
}

void loop()
{
}
