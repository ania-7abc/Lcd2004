# Lcd2004

A lightweight HD44780 library for Arduino with a single API across parallel and I2C buses

## What you get

- 4-bit and 8-bit parallel wiring, plus PCF8574 I2C
- `Print` integration, so `print`, `println`, `write` and number formatting work out of the box
- Backlight and cursor control baked into the API
- Optional screen buffer for flicker-free redraws
- Custom glyphs beyond the hardware limit of 8 via virtual slots
- UTF-8 input and Cyrillic support without external tables
- Compile-time feature trimming for tight MCUs

## Where to go next

**Getting started**:

- [Installation](getting-started/installation.md)
- [Wiring](getting-started/wiring.md)
- [Hello, world](getting-started/hello-world.md)
- [I2C quickstart](getting-started/i2c-quickstart.md)

**Guides**:

- [Display control](guides/core/display-control.md)
- [Text navigation](guides/core/text-navigation.md)
- [Print integration](guides/core/print-integration.md)
- [CGRAM basics](guides/custom-chars/cgram-basics.md)
- [Virtual slots](guides/custom-chars/virtual-slots-xs.md)
- [GFX mode](guides/graphics/gfx-mode.md)
- [GFX update](guides/graphics/gfx-update.md)
- [Cyrillic support](guides/russian/cyrillic-support.md)
- [UTF-8 input](guides/russian/utf8-input-ru.md)

**Configuration**:

- [Overview](configuration/index.md)
- [LCD_NO_I2C](configuration/LCD_NO_I2C.md)
- [LCD_LOW_MEM](configuration/LCD_LOW_MEM.md)
- [LCD_USE_BUFFER](configuration/LCD_USE_BUFFER.md)
- [LCD_VIRTUAL_SLOTS](configuration/LCD_VIRTUAL_SLOTS.md)
- [LCD_MAX_RESOLUTION](configuration/LCD_MAX_RESOLUTION.md)
- [LCD_GFX_LARGE](configuration/LCD_GFX_LARGE.md)

## Choosing a class

| Need                       | Class        |
|----------------------------|--------------|
| Plain text, Latin, one bus | `Lcd2004`    |
| Many custom glyphs         | `Lcd2004xs`  |
| Cyrillic or UTF-8 text     | `Lcd2004ru`  |
| GyverGFX integration       | `Lcd2004gfx` |

Each class builds on the one above it. `Lcd2004ru` extends `Lcd2004xs`, which extends `Lcd2004`. `Lcd2004gfx` also extends `Lcd2004xs`
