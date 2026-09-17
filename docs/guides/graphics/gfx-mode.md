# GFX mode

`Lcd2004gfx` turns a character LCD into a pixel canvas. It builds on `Lcd2004xs` and uses virtual glyph slots to represent each character cell as a small bitmap

The class derives from `GyverGFX`, so the entire drawing API from that library is available: lines, rectangles, circles, text with fonts, and so on. `Lcd2004gfx` only implements the pixel primitive and the flush mechanism

## Dependency

Install [GyverGFX](https://github.com/GyverLibs/GyverGFX) before including the header. The header itself checks for the library and throws a compile error if it is missing

```
Add GyverGFX library to your project to use Lcd2004gfx
```

In PlatformIO, add it to `lib_deps`

```ini
lib_deps =
    ania-7abc/Lcd2004
    gyverlibs/GyverGFX
```

## Resolution

Each character cell is divided into a fixed grid. The default is 2 columns by 2 rows

```
Display 20x4
Pixels  40x8
```

`LCD_GFX_LARGE` changes the vertical division to 3 rows per cell, giving 40x12 on the same 20x4 display

| Macro           |  PPS X  |  PPS Y  |  Virtual slots used  |  Resolution on 20x4  |
|-----------------|:-------:|:-------:|:--------------------:|:--------------------:|
| default         |    2    |    2    |          16          |         40x8         |
| `LCD_GFX_LARGE` |    2    |    3    |          64          |        40x12         |

The horizontal division is always 2 in the current code. The `LCD_GFX_PPS_X` macro is defined as 2 and not meant to be overridden

## Buffer size

The framebuffer size is computed at compile time from `LCD_MAX_RESOLUTION`

```cpp
#define LCD_MAX_RESOLUTION (20 * 4)
#define LCD_GFX_BUFFER_SIZE (LCD_MAX_RESOLUTION / 8 * (LCD_GFX_PPS_X * LCD_GFX_PPS_Y))
```

For the default 20x4 setup, that is 80 characters, 10 bytes per bit row, times 4 pixel rows: 40 bytes for the standard mode and 60 bytes for `LCD_GFX_LARGE`

`LCD_MAX_RESOLUTION` is a character count, not a pixel count. If your display is smaller than the maximum, the buffer is still sized for the maximum. Set `LCD_MAX_RESOLUTION` to match your panel to save RAM

## Basic usage

```cpp
#include <Lcd2004gfx.h>

Lcd2004gfx lcd(0x27, 20, 4);

void setup() {
    lcd.init();
}

void loop() {
    lcd.clear();
    lcd.dot(10, 4);
    lcd.update();
    delay(100);
}
```

`init` does the display init, computes the size from the panel dimensions, and pre-registers the sixteen virtual glyphs that represent every 2x2 pixel pattern

## Memory

For a 20x4 display in standard mode:

| Component            | Bytes        |
|----------------------|--------------|
| Framebuffer          | 40           |
| Virtual slot bitmaps | 16 * 8 = 128 |
| Registered flags     | 16           |
| Hardware slot tables | 16           |
| GyverGFX base        | ~20          |
| **Total**            | **~220**     |

Plus the base `Lcd2004` allocations. On an ATmega328 with a full build that leaves enough room for a modest sketch, but a 16x2 panel is more comfortable

## When not to use it

GFX mode is heavy for what it does. A character LCD was not designed for pixels, and the 2x2 cell resolution is coarse. Reach for it when you want a simple chart, a bar graph, or a progress bar without rolling your own glyphs

For text-heavy projects, plain `Lcd2004` or `Lcd2004ru` uses far less RAM and produces sharper output

## See also

- [GFX update](gfx-update.md)
- [LCD_GFX_LARGE](../../configuration/LCD_GFX_LARGE.md)
- [LCD_MAX_RESOLUTION](../../configuration/LCD_MAX_RESOLUTION.md)
