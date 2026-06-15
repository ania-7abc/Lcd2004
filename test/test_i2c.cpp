#include "bestf/bestf.h"
#include <lcd2004.h>

Lcd2004 lcd(0x26, 16, 2);

START
{
    lcd.init();
}

NO_STOP

TEST(hello_world)
{
    PRINT("Attention to the screen");
    WAIT(3);
    lcd.print("Hello, world");
    CONFIRM;
    END;
}

TESTS_LIST(hello_world)
