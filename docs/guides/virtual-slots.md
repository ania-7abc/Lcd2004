# Virtual Slots

## Introduction

The Lcd2004xs class extends Lcd2004 with virtual slots. The HD44780 controller has only 8 hardware slots for custom
characters in CGRAM. Virtual slots let you define many more custom characters than that, and swap them into hardware
slots as needed. This is useful for graphics, Cyrillic text, or any project that needs more than a handful of custom
icons

## How it works

You register custom characters by calling `saveCustomChar`. They are stored in a virtual bitmap array in RAM. When you
need to display a character, you call `loadVirtualChar`. That method returns a hardware slot number (0 to 7) that
currently holds the character. If the character is not loaded yet, `loadVirtualChar` picks a free slot or replaces the
least recently used one. You then pass that hardware slot number to write() or use it in a string

## Methods

- `saveCustomChar(code, symbol)` registers a character. code is from 0 to the number of virtual slots. symbol
  is an array of 5 bytes
- `freeCustomChar(code)` unregisters the character and frees its hardware slot if it was loaded
- `loadVirtualChar(code)` returns the hardware slot for the character. Loads it if needed
- `isRegistered(code)` returns true if the character is registered
- `isLoaded(code)` returns true if the character is currently in a hardware slot

## Example

```cpp
Lcd2004xs lcd(RS, E, D4, D5, D6, D7, 20, 4);
lcd.init();

uint8_t smiley[5] = {
    0b00100000,
    0b01001110,
    0b01000000,
    0b01001110,
    0b00100000,
};
lcd.saveCustomChar(0, smiley);
lcd.write(lcd.loadVirtualChar(0));
```

You will see:

<svg xmlns="http://www.w3.org/2000/svg" width="50" height="80" viewBox="0 0 5 8" shape-rendering="crispEdges">
  <rect width="5" height="8" fill="#00f"/>
  <path fill="#fff" d="M1 1h1v3h-1zM3 1h1v3h-1zM0 5h1v1h-1zM4 5h1v1h-1zM1 6h3v1h-3z"/>
</svg>

## See also

- [Configuration Lcd2004xs](../configuration/Lcd2004xs.md)
- [Custom chars](custom-chars.md)
- [Graphics](graphics.md)
- [Guides](index.md)
