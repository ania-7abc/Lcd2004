#include <Lcd2004.h>
#include "bestf/bestf.h"

#ifndef lcd_ctor
#define lcd_ctor() Lcd2004(12, 11, 6, 5, 4, 3, 16, 2)
#endif

auto lcd = lcd_ctor();

START
{
    lcd.init();
}

NO_STOP

TEST(hello_world)
{
    lcd.clear();
    lcd.println("Hello, world!");
    PRINT("Do you see \"Hello, world!\"?");
    CONFIRM;
    END;
}

TEST(clear)
{
    lcd.clear();
    lcd.println("0123456789012345");
    PRINT("Is the screen not empty?");
    CONFIRM;
    lcd.clear();
    PRINT("Is the screen empty?");
    CONFIRM;
    END;
}

TEST(return_home)
{
    lcd.clear();
    lcd.setPosition(5, 1);
    lcd.print("ABC");
    lcd.returnHome();
    lcd.print("HOME");
    PRINT("Do you see \"HOME\" at top-left and \"ABC\" on the bottom row?");
    CONFIRM;
    END;
}

TEST(set_position)
{
    lcd.clear();
    lcd.setPosition(0, 0);
    lcd.print("TOP");
    lcd.setPosition(0, 1);
    lcd.print("BOTTOM");
    lcd.setPosition(8, 0);
    lcd.print("MID");
    PRINT("Do you see \"TOP\" top-left, \"BOTTOM\" bottom-left, "
        "and \"MID\" at the top-middle?");
    CONFIRM;
    END;
}

TEST(newline)
{
    lcd.clear();
    lcd.print("Line1\nLine2");
    PRINT("Do you see \"Line1\" on the first line and \"Line2\" on the second?");
    CONFIRM;
    END;
}

TEST(carriage_return)
{
    lcd.clear();
    lcd.print("abc\rX");
    PRINT("Do you see \"Xbc\"?");
    CONFIRM;
    END;
}

TEST(backspace)
{
    lcd.clear();
    lcd.print("abc\bX");
    PRINT("Do you see \"abX\"?");
    CONFIRM;
    END;
}

TEST(wrap_around)
{
    lcd.clear();
    lcd.print("0123456789ABCDEFGH");
    PRINT("Do you see \"0123456789ABCDEF\" on the first line and \"GH\" on the second?");
    CONFIRM;
    END;
}

TEST(display_control)
{
    lcd.clear();
    lcd.print("Display on/off");
    lcd.setOn(false);
    PRINT("Is the display off?");
    CONFIRM;
    lcd.setOn(true);
    PRINT("Is the display on again?");
    CONFIRM;
    END;
}

TEST(cursor_control)
{
    lcd.clear();
    lcd.print("Cursor test");
    lcd.setCursor(true, false);
    PRINT("Do you see an underline cursor?");
    CONFIRM;
    lcd.setCursor(true, true);
    PRINT("Do you see a blinking cursor?");
    CONFIRM;
    lcd.setCursor(false, false);
    PRINT("Is the cursor hidden?");
    CONFIRM;
    END;
}

TEST(custom_char)
{
    lcd.clear();
    uint8_t heart[8] = {
        0b00000,
        0b01010,
        0b11111,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b00000
    };
    lcd.saveCustomChar(0, heart);
    lcd.print("Custom: ");
    lcd.write(0);
    PRINT("Do you see a heart custom character after \"Custom: \"?");
    CONFIRM;
    END;
}

TESTS_LIST(hello_world, clear, return_home, set_position, newline,
           carriage_return, backspace, wrap_around, display_control,
           cursor_control, custom_char)
