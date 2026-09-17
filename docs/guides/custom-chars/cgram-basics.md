# CGRAM basics

The HD44780 has 8 slots of user memory for custom characters. Each slot holds a 5x8 pixel grid that you define once and then print like any other character

## The slot layout

Each slot is 8 bytes. Each byte is one row, top to bottom. The low five bits of each byte are the pixels in that row, with bit 4 as the leftmost column and bit 0 as the rightmost

```
Row 0:       bit4 bit3 bit2 bit1 bit0
Row 1:       bit4 bit3 bit2 bit1 bit0
...
Row 7 or 9:  bit4 bit3 bit2 bit1 bit0
```

Bits 5, 6 and 7 are ignored

## Defining a glyph

Pass an 8-byte array (or 10-byte array in 5x10 font) to `saveCustomChar`. The first argument is the slot number, 0 through 7

```cpp
uint8_t heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
};

lcd.saveCustomChar(0, heart);
```

Reading the array top to bottom:

- Row 0 is blank
- Rows 1 and 2 form the top lobes
- Rows 3 to 5 fill the body
- Row 6 narrows to the point
- Row 7 is the base

Printing slot 0 emits the heart

```cpp
lcd.write(0);
```

The same call works for any registered slot. `write` takes the slot number as a byte, not the character `'0'`

## A second glyph

Eight slots total, so pick them carefully. A common set fills a progress bar, a battery icon, or arrows

```cpp
uint8_t arrow[8] = {
    0b00100,
    0b01110,
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
};

lcd.saveCustomChar(1, arrow);
lcd.write(1);
```

## Overwriting a slot

`saveCustomChar` writes the new bitmap straight over the slot. Any character already on screen using that slot updates on the next panel refresh, which is immediate

```cpp
lcd.saveCustomChar(0, smiley);
```

There is no flicker, since the panel redraws from CGRAM every frame

## Where the slot data goes

The base `Lcd2004` sends the eight bytes straight to the display's CGRAM and forgets them. If you want the driver to remember a glyph, use `Lcd2004xs` instead. See [Virtual slots](virtual-slots-xs.md)

The base class version also restores the cursor to the previous position after the CGRAM write

```cpp
void saveCustomChar(const uint8_t code, uint8_t symbol[8]);
```

Internally it sends `SET_CGRAM_ADDRESS | (code % 8) << 3`, then eight data bytes, then a `SET_DDRAM_ADDRESS` back to where the cursor was

## Registering at startup

CGRAM is volatile. Every power cycle wipes the slots, so custom chars must be written from `setup` or the first time they are needed

```cpp
void setup() {
    lcd.init();

    lcd.saveCustomChar(0, heart);
    lcd.saveCustomChar(1, arrow);

    lcd.print("Lives: ");
    lcd.write(0);
}
```

## When 8 slots are not enough

Most projects need more than eight. `Lcd2004xs` raises the limit by keeping a table of virtual glyphs and rotating them into the eight hardware slots on demand

```cpp
#include <Lcd2004xs.h>

Lcd2004xs lcd(0x27, 20, 4);
```

See [Virtual slots](virtual-slots-xs.md) for the details

## See also

- [Virtual slots](virtual-slots-xs.md)
- [Print integration](../core/print-integration.md)
