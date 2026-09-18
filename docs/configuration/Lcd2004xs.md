# Lcd2004xs configuration

This page describes the build flags that change the behavior of the `Lcd2004xs` class and what they mean for your
sketch

`Lcd2004xs` extends `Lcd2004` with a virtual character system. The HD44780 controller has only 8 hardware character
slots. `Lcd2004xs` lets you register more characters and swaps them into hardware slots on demand. This is the base
class for `Lcd2004ru` and `Lcd2004gfx`

## Build flags

### `LCD_LOW_MEM`

Default: not defined

Cannot be used with `Lcd2004xs`. If defined, the compiler stops with:

```
Can't use LCD_LOW_MEM with Lcd2004xs
```

User impact:

- you cannot reduce RAM by removing optional features when using `Lcd2004xs`
- the full `Lcd2004` feature set is always available

### `LCD_VIRTUAL_SLOTS`

Default: 16

Defines the number of virtual character slots. Each slot holds one custom character definition

When increased:

- `bitmap_` array grows by 5 bytes per additional slot
- `registered_` array grows by 1 bit per additional slot
- more custom characters can be registered
- the LRU (least recently used) swapping logic still maps them to 8 hardware slots

User impact:

- 16 slots use 80 bytes of RAM for `bitmap_` and 2 bytes for `registered_`
- 32 slots use 160 bytes of RAM for `bitmap_` and 4 bytes for `registered_`

### `LCD_HW_SLOTS`

Default: 8

Number of hardware character slots in the HD44780 controller. Do not change

### `LCD_NO_HW_SLOT`

Default: 255

Constant used to mark a free hardware slot in `hwCode_`. Do not change

### `LCD_PROGMEM_CHARS`

Default: not defined

When defined, the bitmap data for virtual characters is read from PROGMEM instead of stored in the `bitmap_` array.

When defined:

- `bitmap_` array is not allocated in the class
- `loadVirtualChar` reads character data from a PROGMEM array named `LCD_PROGMEM_CHARS`
- the PROGMEM array must contain 5 bytes per character, laid out contiguously
- `Lcd2004ru` defines this to point at its `LcdRuGlyphs` array

User impact:

- saves RAM at the cost of flash memory
- you must provide your own PROGMEM array if you use this flag directly with `Lcd2004xs`
- `saveCustomChar` only marks the character as registered, it does not copy the bitmap into RAM

## Interaction with Lcd2004 flags

`Lcd2004xs` inherits from `Lcd2004`, so all `Lcd2004` build flags also apply:

- `LCD_NO_I2C` removes I2C support
- `LCD_USE_BUFFER` adds a software buffer
- `LCD_LOW_MEM` is not allowed

Be careful with `LCD_USE_BUFFER`. The buffer stores character codes. `loadVirtualChar` returns hardware slot numbers
that change over time. If you buffer those numbers and then flush later, the hardware slots may have been reassigned to
different characters. Using both together is not recommended unless you understand the implications.

## Example configurations

Cyrillic support with automatic PROGMEM:

```cpp
#include <Lcd2004ru.h>
Lcd2004ru lcd(12, 11, 5, 4, 3, 2, 20, 4);
void setup() {
    lcd.init();
    lcd.print("Привет");
}
```

Graphics mode with 15 virtual slots:

```cpp
#include <Lcd2004gfx.h>
Lcd2004gfx lcd(12, 11, 5, 4, 3, 2, 20, 4);
void setup() {
    lcd.init();
    lcd.line(0, 0, 39, 15);
    lcd.update();
}
```

Custom virtual slot count for plain `Lcd2004xs`:

```cpp
#define LCD_VIRTUAL_SLOTS 32
#include <Lcd2004xs.h>
Lcd2004xs lcd(12, 11, 5, 4, 3, 2, 20, 4);
```
