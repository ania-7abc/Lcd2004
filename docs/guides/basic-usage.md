# Basic Usage

This guide covers the basic capabilities of the Lcd2004 class: creating an object, initialization, printing text, cursor control, and display settings. It does not cover I2C or custom characters

## Creating an object

To use the display, you need to create an instance of Lcd2004. You specify the pins connected to the display, the number of columns and rows, and optionally the backlight pin. For 8-bit mode, you provide all data pins D0 through D7. For 4-bit mode, you only provide D4 through D7. The constructor also takes the RS and E pins

Example for 4-bit mode:
```cpp
Lcd2004 lcd(12, 11, 6, 5, 4, 3, 16, 2, 7);
```

Here 12 and 11 are the control pins, 6-3 are data pins, 16 and 2 are columns and rows, 7 is the backlight pin (BLA)

!!! Note
    The backlight pin is optional. If you don't want to control the backlight, don't pass BLA pin

## Initialization

After creating the object, call init() to initialize the display. You can optionally pass the font type. The default is 5x8

```cpp
lcd.init();
```

This sets up the display, clears it, and turns on the backlight if a backlight pin was provided

!!! Note
    There are two fonts: `LCD_FONT_5X8` and `LCD_FONT_5X10`

## Printing text

The Lcd2004 class inherits from `Print`, so you can use `print()`, `println()`, and `write()`. For example:

```cpp
lcd.print("Hello");
lcd.println("  World");
lcd.write('!');
```

`print()` and `println()` handle strings, numbers, and other types. `write()` outputs a single byte

The cursor advances automatically after each character. When it reaches the end of a line, it moves to the beginning of the next line

## Cursor positioning

You can set the cursor position with `setPosition(x, y)`

```cpp
lcd.setPosition(0, 0); // top-left corner
lcd.print("Top");
lcd.setPosition(5, 1); // column 5, row 1
lcd.print("Middle");
```

The position is modulo the display size, so you can use values outside the range and they will wrap

## Clearing the display

To clear the screen and return the cursor to the home position, use clear():

```cpp
lcd.clear();
```

This also resets the cursor position to (0,0)

## Display control

You can turn the display on or off with setOn(bool). By default the display is on

```cpp
lcd.setOn(false); // turn off
lcd.setOn(true);  // turn on
```

You can also control the cursor with setCursor(bool on, bool blink = false). The first argument turns the cursor on or off, the second enables blinking

```cpp
lcd.setCursor(true);       // cursor on, no blink
lcd.setCursor(true, true); // cursor on, blinking
lcd.setCursor(false);      // cursor off
```

## Backlight

If you provided a backlight pin, you can control the backlight with setBacklight(bool)

```cpp
lcd.setBacklight(true);
lcd.setBacklight(false);
```

If BLA isn't passed for the backlight output, these calls will have no effect

## Other methods

`returnHome()` moves the cursor to position (0,0) without clearing the display

```cpp
lcd.returnHome();
```

## See also

- [Configuration Lcd2004](../configuration/Lcd2004.md)
- [Guides](index.md)