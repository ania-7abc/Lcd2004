# Quickstart

## Introduction

This page shows how to connect your display and print a simple message. You can use I2C or 4-bit parallel mode. Both
examples assume an Arduino Uno and a 16x2 display

## I2C connection

You need an I2C backpack based on PCF8574 or similar. Connect the backpack to the display, then connect the backpack to
your board

| PCF8574 | Arduino |
|---------|---------|
| VCC     | 5V      |
| GND     | GND     |
| SDA     | A4      |
| SCL     | A5      |

The I2C address is often 0x27 or 0x3F. If you are not sure, run an I2C scanner sketch to find it

## 4-bit connection

Connect the display directly to your board. Also, you need a contrast potentiometer connected to V0

| LCD | Arduino |
|-----|---------|
| GND | GND     |
| VDD | 5V      |
| RS  | 12      |
| RW  | GND     |
| E   | 11      |
| D0  | -       |
| D1  | -       |
| D2  | -       |
| D3  | -       |
| D4  | 5       |
| D5  | 4       |
| D6  | 3       |
| D7  | 2       |
| BLA | 5V      |
| BLK | GND     |

Adjust the pins to match your wiring

## Hello World with I2C

```cpp
#include <Lcd2004.h>

Lcd2004 lcd(0x27, 16, 2);

void setup() {
    lcd.init();
    lcd.print("Hello, world!");
}

void loop() {}
```

## Hello World with 4-bit

```cpp
#include <Lcd2004.h>

Lcd2004 lcd(12, 11, 5, 4, 3, 2, 16, 2);

void setup() {
    lcd.init();
    lcd.print("Hello, world!");
}

void loop() {
}
```

## See also

- [Basic usage](../guides/basic-usage.md)
- [I2C](../guides/i2c.md)
- [Getting started index](index.md)
