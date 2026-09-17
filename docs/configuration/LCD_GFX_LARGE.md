# LCD_GFX_LARGE

Switches `Lcd2004gfx` from 2x2 to 2x3 pixel cells

## Define

```cpp
#define LCD_GFX_LARGE
#include <Lcd2004gfx.h>
```

## Cell size

A single character cell on an HD44780 is 5x8 pixels. The GFX mode divides that into sub-cells, each mapped to one custom glyph

With the default setting, a cell holds 2 columns by 2 rows of sub-cells. The pixel grid per character is therefore 2x2, and 16 custom glyphs cover every on/off combination

With `LCD_GFX_LARGE`, the cell holds 2 columns by 3 rows. The pixel grid per character is 2x3, and 64 custom glyphs cover every combination

## Buffer size

The pixel buffer grows by 50 percent. For a 20x4 display:

| Mode | Bytes |
|------|-------|
| 2x2  | 40    |
| 2x3  | 60    |

On an ATmega328P this is noticeable but not fatal

## Resolution

The logical resolution of the framebuffer doubles in the vertical axis

| Display | 2x2  | 2x3   |
|---------|------|-------|
| 20x4    | 40x8 | 40x12 |
| 16x2    | 32x4 | 32x6  |

`size()` is called inside `init` with `cols_ * 2` and `rows_ * LCD_GFX_PPS_Y`

## Glyph count

The virtual slot count is forced to 64 by the header

```cpp
#ifdef LCD_GFX_LARGE
#define LCD_VIRTUAL_SLOTS 64
#else
#define LCD_VIRTUAL_SLOTS 16
#endif
```

A manual `#define LCD_VIRTUAL_SLOTS` before including `Lcd2004gfx.h` is overwritten

## When to use

- You are drawing glyphs, not just pixels, and the 2x3 grid gives more room
- You have at least 60 bytes of RAM to spare

## When to leave it off

- You are on a small MCU and 16 glyphs cover your needs
- You need the fastest possible `update`, since it loads more distinct glyphs per frame
- Your buffer is already tight and 40 bytes is the limit

## Example

```cpp
#define LCD_GFX_LARGE
#include <Lcd2004gfx.h>

Lcd2004gfx lcd(12, 11, 5, 4, 3, 2, 20, 4);

void setup() {
    lcd.init();
    lcd.circle(20, 6, 5);
    lcd.update();
}

void loop() {}
```

The circle is drawn on a 40x12 grid instead of a 40x8 grid, so the arc has six vertical steps instead of four
