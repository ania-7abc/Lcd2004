#include <Arduino.h>

#include <lcd2004.h>

// 8 bit mode
// RS, E, D0, D1, D2, D3, D4, D5, D6, D7, Width, Height
Lcd2004 lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 2);
// 4 bit mode
// RS, E, D4, D5, D6, D7, Width, Height
// Lcd2004 lcd(2, 3, 4, 5, 6, 7, 16, 2);

void setup()
{
    // Fonts: LCD_FONT_5X8, LCD_FONT_5X10
    lcd.init(LCD_FONT_5X8);

    // Cleaning (Occurs inside init)
    // lcd.clear();

    // Return home (Occurs inside init)
    // lcd.returnHome();

    // Your own symbols
    uint8_t symbol[8] = {
        0b00000, // _____
        0b01010, // _x_x_
        0b01010, // _x_x_
        0b01010, // _x_x_
        0b00000, // _____
        0b10001, // x___x
        0b01110, // _xxx_
        0b00000, // _____
    };
    // The first number is the id, from 0 to 7 inclusive, but for convenience it is better from 1 to 7 inclusive
    lcd.saveCustomChar(1, symbol);

    // Cursor state change, the cursor is off by default.
    // 1: Turn on the cursor? 2: Turn on the cursor blinking?
    lcd.setCursor(true, true);

    // Changing the screen state, the default display is on.
    lcd.setOn(true);
}
int counter = 0;

void loop()
{
    // Reset
    lcd.reset();

    // Clearing, the position is set to 0, 0
    lcd.clear();

    // Position setting (x, y), after init is 0, 0
    lcd.setPosition(0, 1);

    // All methods of the Print class are available
    lcd.printf("Hello, World! %d", counter);

    // Instead of lcd.setPosition, it is better to use returnHome
    lcd.returnHome();

    // Output of previously added characters, in this way you can output everything except the 0 character
    lcd.print("\1 <- custom char");

    // Output of the character with the code 0
    // lcd.write(0);

    counter++;
    delay(1000);
}