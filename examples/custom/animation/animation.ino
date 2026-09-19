#include <Wire.h>
#include <Lcd2004.h>

// I2C address, columns, rows
Lcd2004 lcd(0x3F, 16, 2);

constexpr int animation_length = 4;
const uint8_t spinner[animation_length * 5] = {
    0x00, 0x00, 0x14, 0x00, 0x00,
    0x00, 0x10, 0x00, 0x04, 0x00,
    0x00, 0x08, 0x00, 0x08, 0x00,
    0x00, 0x04, 0x00, 0x10, 0x00,
};

void setup()
{
    Wire.begin();
    lcd.init();

    for (int i = 0; i < animation_length; i++)
        lcd.saveCustomChar(i, spinner + i * 5);

    lcd.print(' '); // print a placeholder char to be overwritten
}

void loop()
{
    for (int i = 0; i < animation_length; i++)
    {
        lcd.print('\b');
        lcd.write(i);
        delay(200);
    }
}
