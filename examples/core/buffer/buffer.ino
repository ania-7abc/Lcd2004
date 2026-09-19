#define LCD_USE_BUFFER
#define LCD_NO_I2C
#include <Lcd2004.h>

Lcd2004 lcd(12, 11, 20, 9, 8, 7, 6, 5, 4, 3, 16, 2);

void setup()
{
    lcd.init();

    // Fill buffer, display stays empty
    lcd.println("Buffer demo");
    lcd.println("Line 2");
    delay(1500);

    // Send whole buffer to display
    lcd.flush();
    delay(1500);

    // clear() affects buffer only
    lcd.clear();
    delay(1500); // display still shows old text

    lcd.flush(); // now display clears
    delay(1500);

    // Another buffer frame
    lcd.setPosition(0, 0);
    lcd.println("Hello");
    lcd.print("World");
    lcd.flush();
}

void loop()
{
}
