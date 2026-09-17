# LCD_MAX_RESOLUTION

Sizes the pixel buffer inside `Lcd2004gfx`

## Define

```cpp
#define LCD_MAX_RESOLUTION (16*2)
#include <Lcd2004gfx.h>
```

## Default

`20*4`, or 80 characters

## What it does

`Lcd2004gfx` keeps a pixel buffer sized from this macro

```cpp
#define LCD_GFX_BUFFER_SIZE (LCD_MAX_RESOLUTION / 8 * (LCD_GFX_PPS_X * LCD_GFX_PPS_Y))
```

`LCD_GFX_PPS_X` is fixed at 2. `LCD_GFX_PPS_Y` is 2 normally and 3 in `LCD_GFX_LARGE` mode

The buffer is a `uint8_t` array of `LCD_GFX_BUFFER_SIZE` bytes

## Buffer sizes

| Resolution | PPS | Bytes |
|------------|-----|-------|
| 80 (20x4)  | 2x2 | 40    |
| 80 (20x4)  | 2x3 | 60    |
| 32 (16x2)  | 2x2 | 16    |
| 32 (16x2)  | 2x3 | 24    |

## When to change it

The default 80 characters covers every common HD44780 module up to 20x4. Lower it only if you are short on RAM and know the exact display size

If you set it too low, `Lcd2004gfx` writes past the end of the buffer when the display is larger than the macro allows. The compiler does not catch this, since the index is computed at runtime

If you set it too high, you waste RAM. There is no other cost

## Interaction with the real display

The macro is an upper bound, not the actual size. At runtime `Lcd2004gfx::init` calls `size(cols_ * LCD_GFX_PPS_X, rows_ * LCD_GFX_PPS_Y)`, where `cols_` and `rows_` come from the constructor. The buffer must be large enough for those values

## Example

```cpp
#define LCD_MAX_RESOLUTION (16*2)
#include <Lcd2004gfx.h>

Lcd2004gfx lcd(12, 11, 5, 4, 3, 2, 16, 2);

void setup() {
    lcd.init();
    lcd.line(0, 0, 31, 3);
    lcd.update();
}

void loop() {}
```

`size` computes 32x4 pixels, which fits in the 16-byte buffer
