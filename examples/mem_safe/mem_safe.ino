#include <Arduino.h>

#define LCD_D_REMOVE_COLS_AND_ROWS_VARS
#define LCD_D_REMOVE_CUR_X_AND_Y_VARS
// ...
#include <Lcd2004.h>

Lcd2004 lcd(0x27);

void setup() {
    lcd.init(2);
    lcd.print("Saving RAM");
}
