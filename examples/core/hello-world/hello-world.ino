#include <Lcd2004.h>

// RS, E, D4, D5, D6, D7, cols, rows
Lcd2004 lcd(12, 11, 5, 4, 3, 2, 20, 4);

void setup() {
    lcd.init();
    lcd.println("Hello, world!");
}

void loop() {}
