# Virtual slots

The HD44780 has 8 hardware CGRAM slots. `Lcd2004xs` adds a table of up to 255 virtual slots and rotates glyphs between them and the hardware on demand

## The idea

You register a glyph once. The library keeps its 8 bytes in RAM and gives you a virtual code. When you print that code, the driver checks whether the glyph is already in one of the 8 hardware slots. If it is, the print goes straight out. If it is not, the least recently used hardware slot is overwritten with your glyph, and the print goes out

The result: as many distinct glyphs as RAM allows, with the same `write` call you use for hardware slots

## Constructor

Same as `Lcd2004`, but you include a different header

```cpp
#include <Lcd2004xs.h>

Lcd2004xs lcd(0x27, 20, 4);
```

The class inherits the full `Lcd2004` API, so nothing else changes

## Registering a glyph

`saveCustomChar` stores the bitmap and marks the code as registered. No CGRAM write happens at this point

```cpp
uint8_t heart[8] = {
    0b00000, 0b01010, 0b11111, 0b11111,
    0b11111, 0b01110, 0b00100, 0b00000,
};

lcd.saveCustomChar(10, heart);
```

Any code from 0 to `LCD_VIRTUAL_SLOTS - 1` works. The default table holds 16 entries, so codes 0 through 15 are valid out of the box

## Printing

`write` accepts the hardware slot code. To print a virtual character, use:

```cpp
lcd.write(lcd.loadVirtualChar(code));
```

If the glyph is not in a hardware slot yet, the driver performs the swap before the write. The swap costs one CGRAM update, roughly 40 microseconds per byte plus command overhead

## Capacity

The default `LCD_VIRTUAL_SLOTS` is 16. Raise it before including the header

```cpp
#define LCD_VIRTUAL_SLOTS 64
#include <Lcd2004xs.h>
```

Each slot costs 8 bytes of `bitmap_` plus one byte of `registered_`, so 64 slots cost 576 bytes of RAM. On an ATmega328 that is most of the budget. On an ESP32 or STM32 it is nothing

The maximum capacity is 255 (not 256)

## The LRU cache

Eight hardware slots are tracked in `hwCode_` and `hwAge_`. Each successful lookup bumps the age counter of that slot

When a glyph needs a slot and none is free, the entry with the smallest age is evicted. The next time that glyph is printed, it is re-uploaded

For an animation that cycles through twenty glyphs in a tight loop, most swaps are evictions, so the cache thrashes. For typical text with occasional symbols, the cache rarely swaps after warm-up

## Querying state

Three methods report what the driver knows

```cpp
lcd.isRegistered(10); // Is a bitmap stored for code 10?
lcd.isLoaded(10);     // Is the bitmap currently in a hardware slot?
```

`isLoaded` answers a different question from `isRegistered`. A glyph can be registered but evicted

## Removing a glyph

`freeCustomChar` clears the registration and drops any hardware slot that held it

```cpp
lcd.freeCustomChar(10);
```

The hardware slot becomes available for the next eviction. The bitmap in RAM is not zeroed, but a later `write(10)` will send garbage since `isRegistered(10)` is now false. Re-register before printing again

## See also

- [CGRAM basics](cgram-basics.md)
- [LCD_VIRTUAL_SLOTS](../../configuration/LCD_VIRTUAL_SLOTS.md)
- [Cyrillic support](../russian/cyrillic-support.md)
```
