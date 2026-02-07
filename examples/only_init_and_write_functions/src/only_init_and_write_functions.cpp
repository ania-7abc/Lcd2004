#include <Arduino.h>

#define LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS
#define LCD_D_FULL_ACCESS
#include <lcd2004.h>

// Это единственый конструктор в этом режиме
// RS, E, D0, D1, D2, D3, D4, D5, D6, D7
Lcd2004 lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

void setup()
{
    // В этом режиме нужно указывать кол-во строк, но не нужно шрифт
    lcd.init(2);

    // Также в этом режиме нет ничего кроме init и write, но поскольку включен LCD_D_FULL_ACCESS есть несколько доп. функций
}

// Обычного lcd.print нет
void print(const char *str)
{
    while (true)
    {
        if (*str == '\0')
            break;
        lcd.write(*str++);
        delay(1);
    }
}

void loop()
{
    // sendByte доступен только при включеным LCD_D_FULL_ACCESS
    lcd.sendByte(LCD_CMD_CLEAR_DISPLAY); // очистка дисплея
    delay(1);
    lcd.sendByte(LCD_CMD_RETURN_HOME); // возврат домой
    delay(1);

    print("Hello, World!");

    delay(1000);
}
