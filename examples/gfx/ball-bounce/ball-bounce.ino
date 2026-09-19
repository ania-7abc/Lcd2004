#define LCD_MAX_RESOLUTION (16*2)
#include <Lcd2004gfx.h>

Lcd2004gfx lcd(12, 11, 20, 9, 8, 7, 6, 5, 4, 3, 16, 2);

int16_t x = 0;
int8_t dx = 1;

void setup()
{
    lcd.init();
    lcd.clear();
    lcd.update();
}

void loop()
{
    lcd.clear();

    lcd.circle(x, 4, 2, GFX_FILL);

    lcd.update();

    x += dx;
    if (x <= 0 || x >= 39)
        dx = -dx;

    delay(80);
}
