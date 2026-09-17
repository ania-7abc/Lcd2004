# Lcd2004
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/ania-7abc/library/Lcd2004.svg)](https://registry.platformio.org/libraries/ania-7abc/Lcd2004)
[![GitHub License](https://img.shields.io/github/license/ania-7abc/Lcd2004)](https://github.com/ania-7abc/Lcd2004/blob/main/LICENSE)
[![GitHub Downloads (all assets, latest release)](https://img.shields.io/github/downloads/ania-7abc/Lcd2004/latest/total)](https://github.com/ania-7abc/Lcd2004/releases/latest)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ania-7abc/Lcd2004)

A clean, lightweight library for liquid crystal displays that is easy to set up

## Installation
**PlatformIO** - add to `platformio.ini`:
```ini
lib_deps =
    ania-7abc/Lcd2004
```

**Arduino IDE** - grab the archive from the [latest release](https://github.com/ania-7abc/Lcd2004/releases/latest) and use *Sketch → Include Library → Add .ZIP Library*

## Hello, world!
```cpp
#include <Lcd2004.h>

// 4-bit: RS, E, D4, D5, D6, D7, cols, rows
Lcd2004 lcd(12, 11, 5, 4, 3, 2, 20, 4);

void setup() {
    lcd.init();
    lcd.println("Hello, world!");
}

void loop() {}
```

I2C variant - pass the PCF8574 address instead of pins:

```cpp
Lcd2004 lcd(0x27, 20, 4);
```

## Привет, мир! (Russian "Hello, world!")
Cyrillic and UTF-8 support live in `Lcd2004ru`, which builds on `Lcd2004xs`

```cpp
#include <Lcd2004ru.h>

Lcd2004ru lcd(12, 11, 5, 4, 3, 2, 20, 4);

void setup() {
    lcd.init();
    lcd.println("Привет, мир!");
}

void loop() {}
```

I2C variant:
```cpp
Lcd2004ru lcd(0x27, 20, 4);
```

## Lcd2004xs - eXtra Slots
The HD44780 has only **8 CGRAM slots** for user-defined characters. `Lcd2004xs` lifts that limit with a virtual slot table:

- Up to **255 virtual slots** are addressable - valid codes are `0..254` inclusive
- A small LRU cache maps virtual codes onto the 8 hardware slots on demand
- Every glyph you register is stored once in RAM; the driver rotates them into hardware only when a character is actually written

`Lcd2004ru` is a thin UTF-8 layer on top of `Lcd2004xs`. Use `Lcd2004xs` directly when you want the extra slots without the Cyrillic plumbing

## Configuration macros

All tuning is compile-time via `#define`. Define them **before** including the headers

| Macro | Default | Effect |
||||
| `LCD_NO_I2C` | off | Strips all `Wire` code. Saves ~1.8 KB flash and ~180 B RAM. |
| `LCD_LOW_MEM` | off | Removes backlight, cursor, blink, and `preWrite`/hook code. Saves ~360–410 B flash. |
| `LCD_USE_BUFFER` | off | Keeps a shadow copy of the screen. Enables flicker-free updates via `flush()`. |
| `LCD_VIRTUAL_SLOTS` | `16` | Number of virtual glyph slots in `Lcd2004xs`. Max `255` (codes `0..254`). |
| `LCD_MAX_RESOLUTION` | `20 * 4` | Upper bound on characters, used to size the `Lcd2004gfx` buffer. |
| `LCD_GFX_LARGE` | off | In `Lcd2004gfx`: 2×3 pixel blocks per cell instead of 2×2. |



## Flash and RAM footprint

Measured on ATmega328P, full 20×4 configuration, Hello-world sketch:

| Configuration                          | Flash (B) | RAM (B) |
|----------------------------------------|-----------|---------|
| Full (I2C, buffer, low-mem off)        | 4716      | 367     |
| Full without buffer                    | 4554      | 262     |
| `LCD_NO_I2C`                           | 2712      | 80      |
| `LCD_NO_I2C` + `LCD_USE_BUFFER`        | 2874      | 185     |
| `LCD_LOW_MEM` + I2C                    | 4142      | 254     |
| `LCD_LOW_MEM`                          | 2348      | 72      |
| `LCD_LOW_MEM` + `LCD_USE_BUFFER` + I2C | 4292      | 359     |
| `LCD_LOW_MEM` + `LCD_USE_BUFFER`       | 2498      | 177     |

The leanest useful build (`LCD_LOW_MEM` + `LCD_NO_I2C`) is **2348 B flash / 72 B RAM**

## Versus LiquidCrystal

`LiquidCrystal` and its ecosystem (`LiquidCrystal_I2C`, `LiquidCrystal_I2C_RUS`) are older and fragmented. `Lcd2004` is one library that covers what those three do together - and more

| Feature                   | Lcd2004 | LiquidCrystal | LiquidCrystal_I2C | LiquidCrystal_I2C_RUS |
|---------------------------|:-------:|:-------------:|:-----------------:|:---------------------:|
| 4-bit / 8-bit bus         |    ✅    |       ✅       |         ❌         |           ❌           |
| I2C (PCF8574)             |    ✅    |       ❌       |         ✅         |           ✅           |
| Single API for both buses |    ✅    |       -       |         -         |           -           |
| Backlight control in API  |    ✅    |       ❌       |         ✅         |           ✅           |
| `Print` integration       |    ✅    |       ✅       |      partial      |        partial        |
| Screen buffer             |    ✅    |       ❌       |         ❌         |           ❌           |
| Custom glyphs beyond 8    |    ✅    |       ❌       |         ❌         |           ❌           |
| UTF-8 input               |    ✅    |       ❌       |         ❌         |           ❌           |
| Cyrillic                  |    ✅    |       ❌       |         ❌         |           ✅           |
| Compile-time trimming     |    ✅    |       ❌       |         ❌         |           ❌           |
| Canonical glyph dedup     |    ✅    |       ❌       |         ❌         |           ❌           |

The only column where the older libraries win is raw minimum size. In its smallest build `Lcd2004` still takes a few hundred bytes more flash than `LiquidCrystal` - that is the honest price of carrying UTF-8, virtual slots, buffer support, and a unified bus API. On an ATmega328 the difference disappears into noise; on an ATtiny you may have to choose between features and bytes

## License
MIT. See [LICENSE](LICENSE)

## Contributing
If this library saved you some time, a ⭐ on [GitHub](https://github.com/ania-7abc/Lcd2004) is appreciated - it helps the project keep growing

## Feedback
If you find a bug, create an [Issue](https://github.com/ania-7abc/Lcd2004/issues)

### Be sure to specify:
* If you write by email, Telegram: Library name (Lcd2004)
* Library version
* The MC where the bug occurs
* If available: SDK version for development
* How do I get the bug?
* Display type. Examples: 16x2, 20x4, 6x1
* Is there a bug in the examples?
* How should I address you?
