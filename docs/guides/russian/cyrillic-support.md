# Cyrillic support

`Lcd2004ru` adds UTF-8 decoding and a Cyrillic glyph set on top of `Lcd2004xs`. You get the extra virtual slots needed for Cyrillic, plus the ability to print Russian text without external font tables

## What it handles

The class reads UTF-8 directly. Send a string literal in UTF-8 from the sketch, and the driver decodes it one code point at a time

```cpp
#include <Lcd2004ru.h>

Lcd2004ru lcd(0x27, 20, 4);

void setup() {
    lcd.init();
    lcd.println("Привет, мир!");
}
```

The source file must be saved as UTF-8. Arduino IDE defaults to that, PlatformIO also

## The glyph set

Two ranges are decoded into custom glyphs

| Range            | Contents               |
|------------------|------------------------|
| U+0410 to U+042F | А through Я, uppercase |
| U+0430 to U+044F | а through я, lowercase |

Both ranges cover the full Russian alphabet except ё and Ё, which fall outside them. Letters that have a canonical twin are mapped to the standard character set instead of using a custom glyph. The remaining 43 letters get glyph shapes stored in flash as 5-column bitmaps, converted to row format during `init()`

The driver reserves the last 43 virtual slots. `Lcd2004ru` forces `LCD_VIRTUAL_SLOTS` to at least 43, so with the default value the Cyrillic slots are 0 through 42. Uppercase non-canonical letters come first, then lowercase non-canonical letters

```cpp
#if LCD_VIRTUAL_SLOTS < 43
#undef LCD_VIRTUAL_SLOTS
#define LCD_VIRTUAL_SLOTS 43
#endif
```

If you need more slots, define `LCD_VIRTUAL_SLOTS` above 43 before including the header

## Canonical glyphs

Some Cyrillic letters look exactly like Latin or ASCII ones: А and A, В and B, С and C, З and 3, and so on. The library keeps a table of 21 pairs and maps each Cyrillic letter to its standard twin

```cpp
const uint16_t canonicalGlyphGroups[21][2] PROGMEM = {
    {0x0041, 0x0410}, // A / А
    {0x0042, 0x0412}, // B / В
    ...
};
```

When `preWrite` sees a Cyrillic code point whose twin is already in the standard character set, it substitutes the standard code point and skips glyph allocation entirely. That saves both flash and a virtual slot

The mapping is one-way. Latin text is never rewritten into Cyrillic

## Mixing scripts

Cyrillic and Latin coexist in the same string

```cpp
lcd.println("Темп: 25 *C");
```

ASCII characters go through unchanged. Cyrillic characters are decoded into the virtual table. The transition between the two is transparent

## Cost

The Cyrillic tables alone add roughly 370 bytes of flash. Combined with the UTF-8 parser and the virtual slot machinery, `Lcd2004ru` is significantly larger than plain `Lcd2004`

For a project that only needs ASCII, use `Lcd2004` or `Lcd2004xs` directly

## See also

- [UTF-8 input](utf8-input-ru.md)
- [Virtual slots](../custom-chars/virtual-slots-xs.md)
