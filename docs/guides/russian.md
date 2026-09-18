# Russian

## Introduction

Lcd2004ru adds Russian language support to the Lcd2004 family. It inherits from Lcd2004xs and overrides the character output to parse UTF-8 and map Cyrillic letters to custom characters. You can print Russian strings directly without manual conversion

## How it works

The class uses 43 virtual slots to store Cyrillic glyphs. When you create an Lcd2004ru object and call init(), it loads these 43 glyphs into the virtual slot system. The library automatically adjusts the number of virtual slots to at least 43 if your configuration had fewer. It occupies these 43 slots for its own use

If you have more than 43 virtual slots defined, the library takes the last 43 slots for Cyrillic. The remaining slots at the beginning stay free for your own custom characters. This way you can mix Russian text with your own icons

The class parses UTF-8 input in preWrite. It recognizes the Cyrillic range U+0410 to U+044F. Other characters pass through unchanged. Invalid UTF-8 sequences become a question mark

## Example

```cpp
Lcd2004ru lcd(RS, E, D4, D5, D6, D7, 20, 4);
lcd.init();
lcd.print("Привет мир");
```

## See also

- [Configuration Lcd2004ru](../configuration/Lcd2004ru.md)
- [Virtual slots](virtual-slots.md)
- [Basic usage](basic-usage.md)
- [Guides](index.md)
