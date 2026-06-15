#include "bestf/bestf.h"

#define LCD_D_USE_BUFFER
#include <lcd2004.h>

#define TEST_START                                       \
    Lcd2004 lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 4); \
    lcd.init();

NO_OTHERS

TEST(flush)
{
    TEST_START
    PRINT("Attention to the screen");
    WAIT(3);
    lcd.print("Text");
    PRINT("Flush");
    lcd.flush();
    CONFIRM;
    END;
}

TEST(avalibleForWrite)
{
    TEST_START
    lcd.setPosition(10, 1);
    int x = lcd.availableForWrite();
    PRINT("Avalible for write: ");
    PRINT(x);
    ASSUME(x == (LCD_S_BUFFER_SIZE - 0x4A));
    END;
}

TEST(write)
{
    TEST_START
    PRINT("Attention to the screen");
    WAIT(3);
    lcd.print("Nothing");
    PRINT("Nothing");
    CONFIRM;
    END;
}

TESTS_LIST(flush, avalibleForWrite, write)
