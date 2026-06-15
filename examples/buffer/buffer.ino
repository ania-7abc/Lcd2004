#include <Arduino.h>

#define LCD_D_USE_BUFFER // Enabling the buffer
#include <lcd2004.h>

// RS, E, D0, D1, D2, D3, D4, D5, D6, D7, Width, Height
// Lcd2004 lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 2);
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
    lcd.flush();
    delay(1000);
}