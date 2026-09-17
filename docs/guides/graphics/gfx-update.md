# GFX update

`Lcd2004gfx` keeps a framebuffer in RAM. Drawing calls modify the buffer. Nothing reaches the panel until `update` is called

This is the key difference from `Lcd2004` and `Lcd2004ru`, where writes go straight to the display. GFX mode always requires an explicit flush

## The three primitives

`GyverGFX` provides the full drawing API. `Lcd2004gfx` implements only the pixel operation that the base class calls

```cpp
void dot(const int x, const int y, const uint8_t fill = GFX_FILL) override;
```

Every line, rectangle and circle reduces to a sequence of `dot` calls. You can call `dot` directly for single pixels

```cpp
lcd.dot(10, 3);            // Set pixel
lcd.dot(10, 3, GFX_CLEAR); // Clear pixel
```

`fill` clears or sets the whole buffer

```cpp
lcd.fill();            // All pixels set
lcd.fill(GFX_CLEAR);   // All pixels cleared
```

`fill` takes effect in the framebuffer, not on the panel, until `update` runs

## The update cycle

```cpp
void update() override;
```

For each character cell in the panel, `update` reads the 2x2 (or 2x3) pixel block from the framebuffer, computes the virtual code, and writes it

```cpp
for (uint8_t y = 0; y < rows_; y++) {
    setPosition(0, y);
    for (uint8_t x = 0; x < cols_; x++) {
        // Read 2x2 pixels, compute code, write
    }
}
```

The result is one DDRAM write per cell, so a full refresh on a 20x4 panel sends 80 bytes plus the cursor moves. On a 100 kHz I2C bus that is roughly 8 ms. On parallel wiring it is under 1 ms

## Redraw strategy

Calling `update` every frame produces a full refresh each time. For an animation that only changes a small area, redrawing the whole screen is wasteful but visible jitter is the real issue

The panel updates one cell at a time. A slow bus makes partial redraws visible as a wipe across the screen

For a smooth animation, use the screen buffer feature alongside GFX mode

```cpp
#define LCD_USE_BUFFER
#include <Lcd2004gfx.h>
```

With the buffer enabled, `update` writes to the shadow buffer instead of the panel, then the driver flushes the whole buffer in one uninterrupted burst

```cpp
void update() override {
    // ... write to buffer_ ...
#ifdef LCD_USE_BUFFER
    Lcd2004xs::flush();
#endif
}
```

`flush` sends `RETURN_HOME` and then streams all 80 cells back to back with a 40 microsecond pause between bytes. The panel never shows a half-updated frame

## Cost of buffering

`LCD_USE_BUFFER` adds a full screen of shadow RAM. On a 20x4 that is 104 bytes. Combined with the GFX framebuffer, the total RAM footprint climbs past 300 bytes. On an ATmega328 that is a large chunk of the 2 KB

On a 16x2 panel the numbers drop to roughly half

## Example: animated bar

```cpp
#include <Lcd2004gfx.h>

Lcd2004gfx lcd(0x27, 20, 4);

void setup() {
    lcd.init();
}

void loop() {
    static uint8_t level = 0;

    lcd.fill(GFX_CLEAR);
    lcd.rect(0, 0, level, 7);
    lcd.update();

    if (++level > 39) level = 0;
    delay(50);
}
```

The `rect` call draws into the framebuffer. The `update` call sends it to the panel. Without `update`, the panel shows the previous frame

## Blitting to avoid full refresh

`GyverGFX` does not provide a dirty-rectangle API. If a full refresh is too slow, track changed regions yourself and call `update` only when something visible actually changed

```cpp
if (value != lastValue) {
    redrawBar(value);
    lcd.update();
    lastValue = value;
}
```

For a progress bar that changes once per second, this is enough

## Panel-size mismatch

The GFX size is set from `cols_` and `rows_` inside `init`

```cpp
size(cols_ * LCD_GFX_PPS_X, rows_ * LCD_GFX_PPS_Y);
```

If you want a larger virtual canvas than the panel, call `size` yourself after `init` and scroll the viewport by offsetting your drawing coordinates

## See also

- [GFX mode](gfx-mode.md)
- [LCD_USE_BUFFER](../../configuration/LCD_USE_BUFFER.md)
