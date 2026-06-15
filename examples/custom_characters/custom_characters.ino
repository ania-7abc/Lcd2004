#include <Arduino.h>

#include <lcd2004.h>

// RS, E, D0-D7, Width, Height.
Lcd2004 lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 2);

void setup()
{
    // По умолчанию LCD_FONT_5X8.
    lcd.init();

    // Символ который мы сохраним.
    uint8_t symbol[8] = {
        0b00000, // _____
        0b01010, // _x_x_
        0b01010, // _x_x_
        0b01010, // _x_x_
        0b00000, // _____
        0b10001, // x___x
        0b01110, // _xxx_
        0b00000, // _____
    };
    // Можно сохранить с ID от 0 до 7 включительно.
    lcd.saveCustomChar(0, symbol);
    lcd.saveCustomChar(1, symbol);
}

void loop()
{
    lcd.clear();
    // Все символы кроме символа с ID равным 0 можно вывести так.
    lcd.print("\1<-symbol");
    lcd.setPosition(0, 1);
    // Для символа с ID 0 надо делать вот так:
    lcd.write(0);
    lcd.print("<-symbol");
    // По этому рекомендуется сохранять символы с ID от 1 до 7 включительно
    delay(1000);
}
