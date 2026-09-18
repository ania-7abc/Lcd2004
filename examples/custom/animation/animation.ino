#include <Wire.h>
#include <Lcd2004.h>

// I2C address, columns, rows
Lcd2004 lcd(0x27, 16, 2);

// Number of animation frames for the spinner
constexpr int animation_length = 4;

// Each frame is a 5-byte custom character bitmap
const uint8_t spinner[animation_length][5] = {
    {0x00, 0x00, 0x14, 0x00, 0x00},
    {0x00, 0x10, 0x00, 0x04, 0x00},
    {0x00, 0x08, 0x00, 0x08, 0x00},
    {0x00, 0x04, 0x00, 0x10, 0x00},
};

void setup()
{
    Wire.begin(); // initialize I2C bus
    lcd.init(); // initialize the LCD

    // Register each spinner frame as a custom character (indices 0..3)
    for (int i = 0; i < animation_length; i++)
        lcd.saveCustomChar(i, spinner[i]);

    lcd.print(' '); // print a placeholder char to be overwritten each loop
}

void loop()
{
    static uint8_t i = 0; // current frame index (persists between calls)

    lcd.print('\b'); // move cursor back one position to overwrite
    lcd.write(i); // draw current animation frame
    delay(200); // frame delay (~5 FPS)

    i++; // advance to next frame
    i %= animation_length; // wrap around to the first frame
}
