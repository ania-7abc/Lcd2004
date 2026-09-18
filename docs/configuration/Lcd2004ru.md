# Lcd2004ru configuration

This page describes the build flags and behavior of the `Lcd2004ru` class and what they mean for your sketch

`Lcd2004ru` extends `Lcd2004xs` with Cyrillic support. It parses UTF-8 input and maps Cyrillic codepoints to ASCII or
virtual characters. This lets you print Russian text directly without manual conversion

## Build flags

### `LCD_RU_START`

Default: `0x0410`

Defines the first Cyrillic codepoint in the supported range. Do not change

### `LCD_RU_END`

Default: `0x044F`

Defines the last Cyrillic codepoint in the supported range. Do not change

### `LCD_RU_GLYPHS_COUNT`

Default: `43`

Number of glyphs in the `LcdRuGlyphs` array. Do not change

### `LCD_PROGMEM_CHARS`

Default: not defined

When defined, `Lcd2004ru` redefines it to `LcdRuGlyphs`. This forces the use of PROGMEM for glyph storage in
`Lcd2004xs`, saving RAM. You cannot use your own PROGMEM array with `Lcd2004ru`. If you need custom glyphs, use
`Lcd2004xs` directly.

User impact: defining `LCD_PROGMEM_CHARS` saves 215 bytes of RAM (43 glyphs * 5 bytes) but you lose the ability to
supply your own character data. Without it, the glyphs are copied into RAM by `init()`

### `LCD_VIRTUAL_SLOTS`

Default: 16 (from `Lcd2004xs`)

`Lcd2004ru` overrides this to at least 43. If you set it lower, it is increased to 43. If you set it higher, your value
is kept. Lcd2004ru is taking up the first 43 available slots.

User impact: you cannot have fewer than 43 virtual slots. This uses at least 215 bytes of RAM for `bitmap_` if
`LCD_PROGMEM_CHARS` is not defined, plus 6 bytes for `registered_` bits (43 bits rounded up to 6 bytes)

## Example configurations

Basic Cyrillic:

```cpp
#include <Lcd2004ru.h>
Lcd2004ru lcd(12, 11, 5, 4, 3, 2, 16, 2);
void setup() {
    lcd.init();
    lcd.print("Привет мир");
}
```

Save RAM with PROGMEM:

```cpp
#define LCD_PROGMEM_CHARS
#include <Lcd2004ru.h>
Lcd2004ru lcd(12, 11, 5, 4, 3, 2, 16, 2);
```

I2C Cyrillic:

```cpp
#include <Lcd2004ru.h>
Lcd2004ru lcd(0x27, 20, 4);
void setup() {
    lcd.init();
    lcd.print("Привет");
}
```
