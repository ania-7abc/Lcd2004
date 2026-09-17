# LCD_LOW_MEM

Strips features that are convenient but not always needed

## Define

```cpp
#define LCD_LOW_MEM
#include <Lcd2004.h>
```

## What it removes

- The backlight control state and the `setBacklight` method
- The `setOn` method that turns the display on and off
- The `setCursor` method that toggles cursor and blink
- The internal `display_`, `cursor_` and `cursor_blink_` fields

## Effect on the API

Four public methods vanish: `setBacklight`, `setOn`, `setCursor` and the protected `preWrite`. Code that calls them fails to build

The display powers on with the default state programmed by `init`: display on, cursor off, blink off, backlight on if wired

## When to use

- The backlight is hardwired to +5 V and never toggled
- You never show a cursor and never blink
- You are out of flash on an ATtiny or a nearly full ATmega

## Savings

On ATmega328P, 20x4, parallel bus:

| Configuration                | Flash (B) | RAM (B) |
|------------------------------|-----------|---------|
| Default                      | 4554      | 262     |
| `LCD_LOW_MEM`                | 4142      | 254     |
| `LCD_LOW_MEM` + `LCD_NO_I2C` | 2348      | 72      |

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
