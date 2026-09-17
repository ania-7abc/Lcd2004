# Configuration

Every tuning knob in the library is a compile-time macro. Define the ones you need **before** including any Lcd2004 header

```cpp
#define LCD_NO_I2C
#define LCD_LOW_MEM
#include <Lcd2004.h>
```

Macros placed after the include have no effect. The preprocessor has already stripped the code they control

## Available macros

| Macro                | Default  | Header                  | Effect                                       |
|----------------------|----------|-------------------------|----------------------------------------------|
| `LCD_NO_I2C`         | off      | all                     | Drops all `Wire` code                        |
| `LCD_LOW_MEM`        | off      | all                     | Drops backlight, cursor, blink               |
| `LCD_USE_BUFFER`     | off      | all                     | Keeps a shadow copy of the screen            |
| `LCD_VIRTUAL_SLOTS`  | `16`     | `Lcd2004xs.h` and above | Virtual glyph slot count                     |
| `LCD_MAX_RESOLUTION` | `(20*4)` | `Lcd2004gfx.h`          | Upper bound on characters for the GFX buffer |
| `LCD_GFX_LARGE`      | off      | `Lcd2004gfx.h`          | 2x3 pixel cells instead of 2x2               |

## Combining macros

The macros are independent. The common combinations and what they cost on an ATmega328P are listed in the README footprint table

The leanest useful build is `LCD_LOW_MEM` plus `LCD_NO_I2C`, which lands at 2348 B flash and 72 B RAM

The heaviest is the default with everything enabled, at 4716 B flash and 367 B RAM for a 20x4 Hello-world sketch

## Where each macro is read

`LCD_NO_I2C` and `LCD_LOW_MEM` are checked inside `Lcd2004.h` and affect every derived class

`LCD_USE_BUFFER` is read in both `Lcd2004.h` and the classes that override `flush`, so it must be defined before including any of them

`LCD_VIRTUAL_SLOTS` is read in `Lcd2004xs.h`. `Lcd2004ru.h` raises it to 43 if the value is lower, since the Cyrillic glyph set does not fit in fewer slots

`LCD_MAX_RESOLUTION` and `LCD_GFX_LARGE` only affect `Lcd2004gfx.h` and the buffer sizing inside it

## Per-macro pages

- [LCD_NO_I2C](LCD_NO_I2C.md)
- [LCD_LOW_MEM](LCD_LOW_MEM.md)
- [LCD_USE_BUFFER](LCD_USE_BUFFER.md)
- [LCD_VIRTUAL_SLOTS](LCD_VIRTUAL_SLOTS.md)
- [LCD_MAX_RESOLUTION](LCD_MAX_RESOLUTION.md)
- [LCD_GFX_LARGE](LCD_GFX_LARGE.md)
