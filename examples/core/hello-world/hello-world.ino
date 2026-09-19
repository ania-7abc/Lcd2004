#include <Lcd2004.h>

// I2C address, cols, rows
Lcd2004 lcd(0x27, 16, 2);

void setup()
{
    lcd.init();
    lcd.println("Hello, world!");
}

void loop()
{
}
