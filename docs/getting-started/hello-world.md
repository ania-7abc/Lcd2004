# Hello, world

The smallest working sketch on a 4-bit parallel display

```cpp
#include <Lcd2004.h>

// RS, E, D4, D5, D6, D7, cols, rows
Lcd2004 lcd(12, 11, 5, 4, 3, 2, 20, 4);

void setup() {
    lcd.init();
    lcd.println("Hello, world!");
}

void loop() {}
```

## What happens

`init()` wakes the display, sets the bus width, clears the screen and turns the backlight on. It runs once in `setup`

`println` comes from the `Print` base class, so number formatting and stream operators work too

```cpp
lcd.print(42);
lcd.print(' ');
lcd.print(3.14, 2);
lcd.println(" done");
```

## Positioning

`setPosition` moves the cursor to a column and row. Both are zero-based

```cpp
lcd.setPosition(0, 0);
lcd.print("Top left");

lcd.setPosition(19, 3);
lcd.print("End");
```

Coordinates wrap, so `setPosition(20, 0)` on a 20-column display lands at column 0

## Clearing and home

```cpp
lcd.clear();       // Wipe the screen, cursor to 0,0
lcd.returnHome();  // Cursor to 0,0, keep contents
```

`clear` is slow on the HD44780, roughly 1.5 ms. Avoid calling it every frame if you can redraw just the changed cells

## Newlines

`println` sends a `\n` that moves the cursor to the start of the next row. On the last row it wraps back to the top

```cpp
lcd.println("Line one");
lcd.println("Line two");
```

`\r` returns to column 0 on the current row. `\b` is a destructive backspace: it steps back, writes a space and steps back again

## I2C version

Same code, one constructor change

```cpp
Lcd2004 lcd(0x27, 20, 4);
```

`0x27` is the usual PCF8574 address. `0x3F` is the other common one. Run an I2C scanner if neither responds

## Custom size

Pass any column and row count the display supports: `16, 2` for a 16x2, `8, 1` for a single-line 8-character module

```cpp
Lcd2004 lcd(12, 11, 5, 4, 3, 2, 16, 2);
```

## Next

- [I2C quickstart](i2c-quickstart.md)
- [Display control](../guides/core/display-control.md) for cursor and backlight
- [Custom characters](../guides/custom-chars/cgram-basics.md)
