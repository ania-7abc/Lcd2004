# Display control

Three functions control what the panel shows: display on/off, cursor visibility, and backlight. Each one is a single call and takes effect immediately

These functions are removed when `LCD_LOW_MEM` is defined. See [LCD_LOW_MEM](../../configuration/LCD_LOW_MEM.md) if your build strips them

## Display on and off

`setOn` toggles the whole display. When off, the panel keeps its contents in DDRAM but shows nothing, and the cursor disappears regardless of its own setting

```cpp
lcd.setOn(false); // Blank the display
lcd.setOn(true);  // Show it again
```

The state survives until changed. After `init()` the display is on

Use it to blink the screen or to save power on a battery build. A typical HD44780 draws a few milliamps either way, so the saving is small compared to the backlight

## Cursor and blink

`setCursor` controls the underline cursor and the block that blinks at the current position. Two independent flags

```cpp
lcd.setCursor(true, false);  // Underline only
lcd.setCursor(true, true);   // Underline plus blinking block
lcd.setCursor(false);        // Hide both
```

Calling `setCursor(true)` without a second argument hides the blink. The default is `false`

The cursor sits at the last write position. `setPosition` moves it, `print` advances it

On a 16x2 with the display off, the cursor stays hidden even if `setCursor` was called with `true`. Turn the display back on to see it

## Backlight

`setBacklight` drives the backlight pin passed in the constructor. If the constructor had no `BLA` argument, the call does nothing

```cpp
lcd.setBacklight(false); // Off
lcd.setBacklight(true);  // On
```

On parallel wiring, the pin must be able to source the backlight current. Many HD44780 modules pull 60 to 120 mA through the backlight LED, which is more than a GPIO can supply. Drive it through a transistor and control the transistor base from the pin

On I2C backpacks, the PCF8574 pin drives an on-board transistor, so the current is handled for you

## Reading back state

The driver stores display, cursor and blink flags in member variables so repeated calls with the same value do not send redundant commands. There is no public getter, but `setOn` and `setCursor` are cheap to call in a loop

```cpp
lcd.setOn(millis() / 500 % 2); // Blink the whole screen once a second
```

## Interaction with init

`init()` turns the display on, hides the cursor and enables the backlight. It does not reset the cursor flags to a known state if you changed them before calling `init`, so call `setCursor(false)` after `init` if you want a clean start

## See also

- [Text navigation](text-navigation.md)
- [Print integration](print-integration.md)
