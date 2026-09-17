# LCD_NO_I2C

Removes every reference to the `Wire` library and the I2C code path

## Define

```cpp
#define LCD_NO_I2C
#include <Lcd2004.h>
```

## Effect

The library stops including `<Wire.h>` and drops the I2C constructor, the I2C branch inside `setPin`, and the I2C branch inside `pulseEnable`. The `setPin` method becomes a `static` free function that only calls `digitalWrite`

## When to use

- You wired the display in 4-bit or 8-bit parallel and never touch a PCF8574
- Your board has no hardware I2C, or you already use the I2C pins for something else
- You are close to the flash limit and want the ~1.8 KB back

## Savings

Measured on ATmega328P with a full 20x4 build:

| Configuration                | Flash (B) | RAM (B) |
|------------------------------|-----------|---------|
| Default                      | 4554      | 262     |
| `LCD_NO_I2C`                 | 2712      | 80      |
| `LCD_NO_I2C` + `LCD_LOW_MEM` | 2348      | 72      |

The RAM drop comes from removing the Wire transmit buffer that `Wire.h` pulls in through its static globals

## Example

```cpp
#define LCD_LOW_MEM
#define LCD_NO_I2C
#include <Lcd2004.h>

Lcd2004 lcd(12, 11, 5, 4, 3, 2, 16, 2);

void setup() {
    lcd.init();
    lcd.print("Hello, world!");
}

void loop() {}
```
