#include <Arduino.h>

#include <lcd2004.h>

// RS, E, D4, D5, D6, D7, Width, Height
Lcd2004 lcd(3, 4, 9, 10, 11, 12, 16, 2);

void setup()
{
    lcd.init();
}

void loop()
{
    lcd.clear();
    lcd.print("Hello, World!");
    delay(1000);
}
