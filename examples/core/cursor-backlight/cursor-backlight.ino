#define LCD_NO_I2C
#include <Lcd2004.h>

// RS, E, D4, D5, D6, D7, cols, rows, backlight
Lcd2004 lcd(12, 11, 5, 4, 3, 2, 20, 4, 10);

void setup()
{
    lcd.init();
    lcd.println("Cursor demo");
}

void loop()
{
    lcd.setCursor(false); // Cursor off
    delay(500);

    lcd.setCursor(true); // Cursor on
    delay(500);

    lcd.setCursor(true, true); // Cursor on and blink
    delay(1000);

    lcd.setBacklight(false);
    delay(500);

    lcd.setBacklight(true);
    delay(500);

    lcd.setOn(false);
    delay(500);

    lcd.setOn(true);
    delay(500);
}
