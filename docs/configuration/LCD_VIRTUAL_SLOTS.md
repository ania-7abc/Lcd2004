# LCD_VIRTUAL_SLOTS

Controls how many glyphs `Lcd2004xs` can hold beyond the eight hardware slots

## Define

```cpp
#define LCD_VIRTUAL_SLOTS 32
#include <Lcd2004xs.h>
```

## Default

16 slots. That is enough for a small icon set: a battery, a signal bar, a couple of arrows, a degree symbol

## Range

1 to 255 (not 256). The upper bound comes from the code space: virtual characters are addressed by a single `uint8_t`, and `LCD_NO_HW_SLOT` uses the value 8, so the usable code range is effectively 0 to 254

Values above 255 are truncated by the preprocessor into a `uint8_t` and silently wrap

## Memory cost

Each slot holds an 8-byte bitmap plus a bool flag. That is 8 bytes and 1 bit per slot

| Slots | RAM for glyphs |
|-------|----------------|
| 16    | 130 B          |
| 32    | 260 B          |
| 64    | 520 B          |
| 255   | 2072 B         |

On an ATmega328P with 2 KB of RAM, 64 slots already consume a quarter of the total. 255 slots do not fit at all

## Hardware interaction

The HD44780 has 8 CGRAM positions. `Lcd2004xs` maps a virtual slot onto a hardware slot on demand and evicts the least recently used entry when all eight are busy

A glyph that has been evicted reloads from the RAM bitmap the next time it is written. The user never sees the swap

Because only eight glyphs are visible per screen at once, a display never shows more than eight distinct custom characters even if hundreds are registered

## Example

```cpp
#define LCD_VIRTUAL_SLOTS 64
#include <Lcd2004xs.h>

Lcd2004xs lcd(12, 11, 5, 4, 3, 2, 20, 4);

void setup() {
    lcd.init();

    for (uint8_t i = 0; i < 64; i++) {
        uint8_t symbol[8] = {0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F, 0};
        lcd.saveCustomChar(i, symbol);
    }
    lcd.write(lcd.loadVirtualSlot(7));
}

void loop() {}
```

All 64 glyphs stay in RAM. Only the eight on screen at any moment occupy hardware slots
