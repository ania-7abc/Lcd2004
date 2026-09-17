# I2C quickstart

The PCF8574 backpack turns sixteen display pins into two I2C wires. This is the usual way to wire an HD44780 on a small board

## Wiring

| Backpack | Arduino Uno |
|----------|-------------|
| GND      | GND         |
| VCC      | 5 V         |
| SDA      | A4          |
| SCL      | A5          |

On a Leonardo, Micro or other ATmega32U4 board the pins are D2 (SDA) and D3 (SCL). On an ESP32 or ESP8266 they are configurable and `Wire.begin(sda, scl)` sets them before `lcd.init()`

Contrast comes from the small pot on the backpack. Turn it until the pixels appear

## Code

Pass the backpack's I2C address instead of pins. The library detects the I2C constructor and switches modes automatically

```cpp
#include <Lcd2004.h>

Lcd2004 lcd(0x27, 20, 4);

void setup() {
    Wire.begin();
    lcd.init();
    lcd.println("Hello over I2C");
}

void loop() {}
```

The first argument is the address, then columns, then rows. Everything else behaves the same as the parallel version

## Finding the address

Common PCF8574 addresses are `0x27` and `0x3F`. Some backpacks use a different one, and sometimes a jumper on the board shifts it

Scan the bus with the standard Arduino I2C scanner. If the display never responds, verify the scanner sees the device first

```cpp
#include <Wire.h>

void setup() {
    Wire.begin();
    Serial.begin(115200);

    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.print("Found 0x");
            Serial.println(addr, HEX);
        }
    }
}

void loop() {}
```

## Backlight on I2C

The backpack drives the backlight through a PCF8574 pin. `setBacklight` still works with I2C, no changes to your code

```cpp
lcd.setBacklight(false); // Off
lcd.setBacklight(true);  // On
```

## Speed

The library runs the bus at the default 100 kHz. Raising `Wire.setClock(400000)` before `lcd.init()` works on most backpacks and roughly quadruples throughput for `flush()` and GFX updates

## Next

- [Display control](../guides/core/display-control.md)
- [Screen buffer](../configuration/LCD_USE_BUFFER.md) if you redraw often
