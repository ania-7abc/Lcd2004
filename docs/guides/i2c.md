# I2C

## Introduction

Connecting a display via I2C allows you to reduce the number of wires to two: SDA and SCL. For this purpose, a converter based on the PCF8574 chip or a similar one is used. The library supports this mode via a separate constructor

## Connection

To use I2C, create a Lcd2004 object with the device address and display dimensions

```cpp
Lcd2004 lcd(0x27, 16, 2);
```

The first argument is the I2C address of the adapter. The second and third are the number of columns and rows. The backlight pin and the other pins are configured automatically within the library

After creation, call `init()` as usual:

```cpp
lcd.init();
```

The library will initialize Wire itself and configure all the necessary adapter pins

## Backlight

In I2C mode, the backlight is controlled via the same adapter. The setBacklight method works in the same way as with parallel connection

```cpp
lcd.setBacklight(true);
lcd.setBacklight(false);
```

If you don’t want to control the backlight, simply don’t call these methods. By default, the backlight is enabled after initialization

## See also

- [Basic Usage](basic-usage.md)
- [Configuration Lcd2004](../configuration/Lcd2004.md)
- [Guides](index.md)
