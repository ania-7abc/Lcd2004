#include <Arduino.h>

#define LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS
#define LCD_D_FULL_ACCESS
#include <lcd2004.h>

// This is the only constructor in this mode
// RS, E, D0, D1, D2, D3, D4, D5, D6, D7
Lcd2004 lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

void setup()
{
    // In this mode, you need to specify the number of lines, but you don't need a font
    lcd.init(2);
    delay(2); // For stability in this mode, you need to wait 2 milliseconds

    // There is also nothing in this mode except init and write, but since LCD_D_FULL_ACCESS is enabled, there are several additional functions
}

// There is no regular lcd.print
void print(const char *str)
{
    while (true)
    {
        if (*str == '\0')
            break;
        lcd.write(*str++);
        delay(1);
    }
}

void loop()
{
    // sendByte is only available when LCD_D_FULL_ACCESS is enabled
    lcd.sendByte(LCD_CMD_CLEAR_DISPLAY); // clearing the display
    delay(1);
    lcd.sendByte(LCD_CMD_RETURN_HOME); // return home
    delay(1);

    print("Hello, World!");

    delay(1000);
}