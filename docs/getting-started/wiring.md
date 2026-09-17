# Wiring

The HD44780 talks over either a 4-bit or an 8-bit parallel bus. Both work with this library

## Pins on the display

| Pin   | Name  | Purpose                                  |
|-------|-------|------------------------------------------|
| 1     | VSS   | Ground                                   |
| 2     | VDD   | +5 V                                     |
| 3     | V0    | Contrast, wire to a 10k pot wiper        |
| 4     | RS    | Register select                          |
| 5     | RW    | Read/write, tie to GND for write-only    |
| 6     | E     | Enable strobe                            |
| 7-10  | D0-D3 | Data low nibble (unused in 4-bit mode)   |
| 11-14 | D4-D7 | Data high nibble                         |
| 15    | A     | Backlight anode, +5 V through a resistor |
| 16    | K     | Backlight cathode, GND                   |

## Contrast

Pin 3 sets the pixel contrast. A 10k potentiometer between +5 V and GND, with the wiper on V0, gives the usual adjustment range. Without it the display stays blank

Some I2C backpacks have a small trim pot on board, so no external part is needed

## 4-bit parallel

Only four data lines are used. This is the common wiring and leaves the most pins free

```cpp
// RS, E, D4, D5, D6, D7, cols, rows
Lcd2004 lcd(12, 11, 5, 4, 3, 2, 20, 4);
```

| Display | Arduino |
|---------|---------|
| RS      | 12      |
| E       | 11      |
| D4      | 5       |
| D5      | 4       |
| D6      | 3       |
| D7      | 2       |
| RW      | GND     |

Any free digital pins work. The example uses the same set as the Arduino `LiquidCrystal` tutorial, so existing wiring carries over

## 8-bit parallel

All eight data lines are used. More pins, slightly faster updates, same API

```cpp
// RS, E, D0..D7, cols, rows
Lcd2004 lcd(12, 11, 5, 4, 3, 2, 9, 8, 7, 6, 20, 4);
```

The 8-bit constructor takes ten pins in this order: RS, E, D0, D1, D2, D3, D4, D5, D6, D7

## Backlight

If the display has a backlight, and you want software control, wire the anode side to a spare pin through a resistor and pass that pin as the `BLA` argument

```cpp
Lcd2004 lcd(12, 11, 5, 4, 3, 2, 20, 4, 10);
```

The last argument is the backlight pin. Without it the backlight stays always on and cannot be toggled from code

Keep the current under the pin limit. A typical HD44780 backlight draws more than a GPIO can source, so use a transistor if the segment pulls over 20 mA

## I2C

A PCF8574 backpack cuts the wiring to four lines: VCC, GND, SDA, SCL. See [I2C quickstart](i2c-quickstart.md) for details

## Next

- [Hello, world](hello-world.md)
- [I2C quickstart](i2c-quickstart.md)
