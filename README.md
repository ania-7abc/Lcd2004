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

**Arduino IDE** - grab the archive from the [latest release](https://github.com/ania-7abc/Lcd2004/releases/latest) and
use *Sketch → Include Library → Add .ZIP Library*

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

The HD44780 has only **8 CGRAM slots** for user-defined characters. `Lcd2004xs` lifts that limit with a virtual slot
table:

- Up to **255 virtual slots** are addressable - valid codes are `0..254` inclusive
- A small LRU cache maps virtual codes onto the 8 hardware slots on demand
- Every glyph you register is stored once in RAM; the driver rotates them into hardware only when a character is
  actually written

`Lcd2004ru` is a thin UTF-8 layer on top of `Lcd2004xs`. Use `Lcd2004xs` directly when you want the extra slots without
the Cyrillic plumbing

## Configuration macros

All tuning is compile-time via `#define`. Define them **before** including the headers

| Macro                | Default  | Effect                                                                              |
|----------------------|----------|-------------------------------------------------------------------------------------|
| `LCD_NO_I2C`         | off      | Strips all `Wire` code. Saves ~1.8 KB flash and ~180 B RAM.                         |
| `LCD_LOW_MEM`        | off      | Removes backlight, cursor, blink, and `preWrite`/hook code. Saves ~360–410 B flash. |
| `LCD_USE_BUFFER`     | off      | Keeps a shadow copy of the screen. Enables flicker-free updates via `flush()`.      |
| `LCD_VIRTUAL_SLOTS`  | `16`     | Number of virtual glyph slots in `Lcd2004xs`. Max `255` (codes `0..254`).           |
| `LCD_MAX_RESOLUTION` | `20 * 4` | Upper bound on characters, used to size the `Lcd2004gfx` buffer.                    |
| `LCD_GFX_LARGE`      | off      | In `Lcd2004gfx`: 2×3 pixel blocks per cell instead of 2×2.                          |

## Flash and RAM footprint

Measured on ATmega328P, 4-bit 20×4, Hello-world sketch:

```cpp
#include <Arduino.h>

// #define LCD_USE_BUFFER
// #define LCD_NO_I2C
// #define LCD_LOW_MEM
#include <Lcd2004.h>

Lcd2004 lcd(9, 8, 7, 6, 5, 4, 20, 4);

void setup()
{
    lcd.init();
    lcd.print("Hello, world!");
    // lcd.flush();
}

void loop()
{
}
```

| Buffer | No I2C | `LCD_LOW_MEM` | Flash (KiB) | RAM (B) |
|:------:|:------:|:-------------:|:-----------:|:-------:|
|   ❌    |   ❌    |       ❌       |    4,58     |   256   |
|   ✅    |   ❌    |       ❌       |    4,74     |   361   |
|   ❌    |   ✅    |       ❌       |    2,73     |   74    |
|   ✅    |   ✅    |       ❌       |    2,89     |   179   |
|   ❌    |   ❌    |       ✅       |    4,10     |   251   |
|   ✅    |   ❌    |       ✅       |    4,27     |   356   |
|   ❌    |   ✅    |       ✅       |    2,34     |   69    |
|   ✅    |   ✅    |       ✅       |    2,51     |   174   |

The leanest useful build (`LCD_LOW_MEM` + `LCD_NO_I2C`) is **2348 B flash / 72 B RAM**

## License

MIT. See [LICENSE](LICENSE)

## Contributing

If this library saved you some time, a ⭐ on [GitHub](https://github.com/ania-7abc/Lcd2004) is appreciated - it helps the
project keep growing

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
