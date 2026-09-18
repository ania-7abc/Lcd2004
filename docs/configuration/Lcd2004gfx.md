# Lcd2004gfx configuration

This page describes the build flags and behavior of the `Lcd2004gfx` class and what they mean for your sketch

`Lcd2004gfx` extends `Lcd2004xs` and `GyverGFX`. It turns the character LCD into a 1-bit per pixel graphics display.
Each character cell is divided into either 2x2 or 2x3 pixels. The class generates custom characters for every possible
pixel combination and swaps them into hardware slots during `update()`

GyverGFX must be installed. If the library is missing, the compiler stops with an error

## Build flags

### `LCD_GFX_LARGE`

Default: not defined

Selects the pixel resolution per character cell

When not defined:

- each character cell is 2x2 pixels
- `LCD_GFX_PPS_Y` is 2
- `LCD_VIRTUAL_SLOTS` is 15

When defined:

- each character cell is 2x3 pixels
- `LCD_GFX_PPS_Y` is 3
- `LCD_VIRTUAL_SLOTS` is 63

User impact:

- larger mode changes vertical resolution from 2 to 3 pixels per row
- larger mode needs 63 virtual slots instead of 15
- `bitmap_` grows from 75 bytes (15 * 5) to 315 bytes (63 * 5)
- `registered_` grows from 2 bytes to 8 bytes
- the pixel buffer grows from 40 to 60 bytes with the default `LCD_MAX_RESOLUTION`
- smaller mode uses less RAM and flash but has lower vertical resolution

### `LCD_MAX_RESOLUTION`

Default: `(20 * 4)` = 80

Defines the maximum number of character cells on the display. This is used to compute the size of the pixel buffer

The buffer size is calculated as:

```
LCD_GFX_BUFFER_SIZE = LCD_MAX_RESOLUTION / 8 * (LCD_GFX_PPS_X * LCD_GFX_PPS_Y)
```

User impact:

- set this to `cols * rows` for your display
- for a 16x2 display, set `LCD_MAX_RESOLUTION` to 32
- for a 20x4 display, the default 80 is correct
- if the value is too small, the buffer overflows when drawing
- if the value is too large, RAM is wasted
- the default buffer size is 40 bytes for 20x4 in 2x2 mode
- the default buffer size is 60 bytes for 20x4 in 2x3 mode

### `LCD_GFX_PPS_X`

Default: 2

Pixels per character cell horizontally. Do not change

### `LCD_GFX_PPS_Y`

Default: 2 or 3 depending on `LCD_GFX_LARGE`

Pixels per character cell vertically. Do not change directly

### `LCD_VIRTUAL_SLOTS`

Default: 15 or 63 depending on `LCD_GFX_LARGE`

Number of virtual character slots. `Lcd2004gfx` sets this value before including `Lcd2004xs.h`. `Lcd2004xs` uses this
value. Do not change

## Example configurations

Default 2x2 graphics on a 20x4 display:

```cpp
#include <Lcd2004gfx.h>
Lcd2004gfx lcd(12, 11, 5, 4, 3, 2, 20, 4);
void setup() {
    lcd.init();
    lcd.line(0, 0, 39, 15);
    lcd.update();
}
```

2x3 graphics with buffered updates:

```cpp
#define LCD_GFX_LARGE
#define LCD_USE_BUFFER
#include <Lcd2004gfx.h>
Lcd2004gfx lcd(12, 11, 5, 4, 3, 2, 20, 4);
void setup() {
    lcd.init();
    lcd.rect(0, 0, 39, 23);
    lcd.update();
}
```

16x2 display with reduced buffer:

```cpp
#define LCD_MAX_RESOLUTION (16 * 2)
#include <Lcd2004gfx.h>
Lcd2004gfx lcd(12, 11, 5, 4, 3, 2, 16, 2);
void setup() {
    lcd.init();
    lcd.circle(8, 4, 3);
    lcd.update();
}
```
