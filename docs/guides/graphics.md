# Graphics

## Introduction

Lcd2004gfx adds a graphical mode to the Lcd2004 family. It inherits from Lcd2004xs and from GyverGFX, so you can draw
pixels, lines, and shapes using the familiar GyverGFX API, then push the result to the display. The class uses a
framebuffer in RAM and maps groups of pixels to custom characters stored in the display's CGRAM

!!! note
    You need the GyverGFX library installed in your project. Without it the header will not compile

## Basic usage

Create an object like you would with Lcd2004xs, then call init(). After that you can use drawing methods such as dot(),
`line()`, `rect()`, and `circle()` that come from GyverGFX. The display is not updated automatically. You must call update()
to transfer the framebuffer to the screen

```cpp
Lcd2004gfx lcd(RS, E, D4, D5, D6, D7, 20, 4);
lcd.init();
lcd.dot(0, 0);
lcd.line(0, 0, 10, 10);
lcd.update();
```

The drawing coordinates are in pixels. The width is the number of columns multiplied by 2, and the height is the number
of rows multiplied by 2. When you enable the larger pixel mode, the height multiplier becomes 3. The width multiplier
stays 2

## Memory

The class uses a framebuffer whose size depends on the maximum resolution you plan to support. By default the maximum
resolution is 20 columns by 4 rows, which is enough for most displays. If you use a smaller display, you can reduce the
buffer size by lowering the `LCD_MAX_RESOLUTION` define before including the header. This saves RAM

There is also a define called `LCD_GFX_LARGE`. When defined, each virtual pixel becomes 2 by 3 real pixels instead of 2
by 2. This gives more vertical detail but increases the number of required custom characters

The exact memory usage depends on your display size and the maximum resolution you set. For a typical 20x4 display the
buffer takes a few dozen bytes. If you are short on RAM, lowering `LCD_MAX_RESOLUTION` to match your actual display size
is the simplest way to reduce the footprint

## See also

- [Configuration Lcd2004gfx](../configuration/Lcd2004gfx.md)
- [Virtual slots](virtual-slots.md)
- [Guides](index.md)
