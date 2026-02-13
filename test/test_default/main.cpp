#include "../bestf.h"
#include <lcd2004.h>

#define TEST_START                                       \
    Lcd2004 lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 4); \
    lcd.init();                                          \
    PRINT("Attention to the screen");                    \
    WAIT(3);

#define TEST_END \
    CONFIRM;     \
    END;

NO_OTHERS

TEST(clear)
{
    TEST_START

    lcd.print("Nothing");
    WAIT(1);
    lcd.clear();

    TEST_END
}

TEST(return_home)
{
    TEST_START

    srand(digitalRead(2));
    lcd.setPosition(rand() % 12, rand() % 2);
    lcd.print("Rand");
    WAIT(1);
    lcd.returnHome();
    lcd.print("T");

    TEST_END
}

TEST(setOn)
{
    TEST_START

    lcd.print("Text");
    WAIT(1);
    lcd.setOn(false);
    WAIT(1);
    lcd.setOn(true);

    TEST_END
}

TEST(setCursor)
{
    TEST_START

    lcd.setCursor(true, true);
    WAIT(1);
    lcd.setCursor(false, false);

    TEST_END
}

TEST(setPosition)
{
    TEST_START

    for (int y = 0; y < 2; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            lcd.setPosition(x, y);
            lcd.print(x);
        }
    }

    TEST_END
}

TEST(saveCustomChar)
{
    TEST_START

    uint8_t smile[8] = {
        0b00000,
        0b01010,
        0b01010,
        0b01010,
        0b00000,
        0b10001,
        0b01110,
        0b00000,
    };
    lcd.saveCustomChar(1, smile);
    lcd.print("\1 <- smile");

    TEST_END
}

TEST(write) {
    TEST_START

    lcd.write('1');
    WAIT(1);
    lcd.write('2');

    TEST_END
}

TESTS_LIST(write, clear, return_home, setOn, setCursor, setPosition, saveCustomChar)
