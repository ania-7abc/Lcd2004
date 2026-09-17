#include <Lcd2004.h>

// RS, E, D4, D5, D6, D7, cols, rows, backlight
Lcd2004 lcd(12, 11, 5, 4, 3, 2, 20, 4, 10);

void setup() {
    lcd.init();
    lcd.setPosition(0, 0);
    lcd.print("Cursor demo");
}

void loop() {
    lcd.setCursor(true, false);
    delay(1000);

    lcd.setCursor(true, true);
    delay(1000);

    lcd.setCursor(false, false);
    delay(1000);

    lcd.setBacklight(false);
    delay(500);

    lcd.setBacklight(true);
    delay(500);

    lcd.setOn(false);
    delay(500);

    lcd.setOn(true);
    delay(500);
}
