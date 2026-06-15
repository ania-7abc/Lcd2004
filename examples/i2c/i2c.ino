#include <Arduino.h>

#include <Lcd2004.h>

Lcd2004 lcd(0x27, 16, 2);

void setup()
{
    Wire.begin();
    lcd.init();
    lcd.setPosition(0, 1);
    lcd.print("I2C works!");
}

void loop()
{
}
