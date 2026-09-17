#include <Lcd2004.h>

Lcd2004 lcd(12, 11, 5, 4, 3, 2, 20, 4);

// A filled square with a hollow center
uint8_t square[8] = {
    0b11111,
    0b11111,
    0b11011,
    0b11011,
    0b11011,
    0b11011,
    0b11111,
    0b11111,
};

void setup() {
    lcd.init();

    lcd.saveCustomChar(0, square);

    lcd.setPosition(0, 0);
    lcd.print("Glyph: ");
    lcd.write(uint8_t(0));
}

void loop() {}
