#include <Lcd2004gfx.h>

Lcd2004gfx lcd(12, 11, 5, 4, 3, 2, 20, 4);

void setup() {
    lcd.init();
    lcd.clear();

    lcd.line(0, 0, 39, 7);
    lcd.line(0, 7, 39, 0);

    lcd.rect(5, 2, 15, 5);
    lcd.circle(30, 4, 3);

    lcd.update();
}

void loop() {}
