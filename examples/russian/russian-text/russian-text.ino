#include <Lcd2004ru.h>

Lcd2004ru lcd(12, 11, 5, 4, 3, 2, 20, 4);

void setup() {
    lcd.init();

    lcd.setPosition(0, 0);
    lcd.println("Привет, мир!");

    lcd.setPosition(0, 1);
    lcd.print("Температура: 23 C");
}

void loop() {}
