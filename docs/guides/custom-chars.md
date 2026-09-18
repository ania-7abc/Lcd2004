# Custom characters

Custom characters let you draw 5×8 pixel glyphs and use them as ordinary LCD characters.

## Introduction

The LCD controller provides 8 custom character slots. Each slot is selected by a character code from `0` to `7`.

`Lcd2004` provides `saveCustomChar()` for creating these characters:

```cpp
lcd.saveCustomChar(1, symbol);
```

The first argument selects the custom character slot. The second argument is an array of exactly 5 bytes.

!!! tip
    Prefer slots `1` through `7` for custom characters. Slot `0` works too, but using `1..7` makes strings containing custom
    characters easier to read, for example:

    ```cpp
    lcd.println("Hello, world! \1");
    ```

    Here `\1` represents character code `1`.

## Character format

`saveCustomChar()` expects 5 columns, not 8 rows.

Each byte in the array describes one column, from left to right. The first byte (`symbol[0]`) is the leftmost column,
and the fifth byte (`symbol[4]`) is the rightmost column.

The 8 bits of each byte describe the pixels from top to bottom. Bit 0 (LSB) is the top pixel, bit 7 (MSB) is the bottom
pixel.

For example, to draw a diamond:

```cpp
const uint8_t diamond[5] = {
    0b00000100, // column 0 (left)
    0b00001110, // column 1
    0b00011111, // column 2 (center)
    0b00001110, // column 3
    0b00000100  // column 4 (right)
};
```

This produces the following 8×5 pixel pattern (rows top to bottom):

<svg xmlns="http://www.w3.org/2000/svg" width="50" height="80" viewBox="0 0 5 8" shape-rendering="crispEdges">
  <rect width="5" height="8" fill="#00f"/>
  <path fill="#fff" d="M2 0h1v1h-1M1 1h3v1h-3M0 2h5v1h-5M1 3h3v1h-3M2 4h1v1h-1"/>
</svg>

The exact bit-to-pixel conversion is handled by `saveCustomChar()`. The method reads the five input bytes column by
column and sends eight pixel rows to the LCD controller.

```cpp
lcd.saveCustomChar(1, diamond);
```

After saving the character, its slot can be written using character code `1`:

```cpp
lcd.print("\1");
```

It can also be embedded into a larger string:

```cpp
lcd.println("Hello, world! \1");
```

## Defining a character

A complete example for a right-pointing arrow:

```cpp
const uint8_t arrow[5] = {
    0b00001000, // column 0
    0b00001000, // column 1
    0b00101010, // column 2
    0b00011100, // column 3
    0b00001000  // column 4
};

lcd.saveCustomChar(1, arrow);

lcd.setPosition(0, 0);
lcd.print("Next \1");
```

This arrow looks like:

<svg xmlns="http://www.w3.org/2000/svg" width="50" height="80" viewBox="0 0 5 8" shape-rendering="crispEdges">
  <rect width="5" height="8" fill="#00f"/>
  <path fill="#fff" d="M2 1h1v1h-1M3 2h1v1h-1M0 3h5v1h-5M3 4h1v1h-1M2 5h1v1h-1"/>
</svg>

Use a separate 5-byte array for each character you want to store.

```cpp
const uint8_t smile[5] = {
    0b00001000, // column 0
    0b00010010, // column 1
    0b00010000, // column 2
    0b00010010, // column 3
    0b00001000  // column 4
};

lcd.saveCustomChar(2, smile);
```

The slot number determines which character code is produced:

```cpp
lcd.saveCustomChar(1, arrow);
lcd.saveCustomChar(2, smile);

lcd.print("\1 \2");
```

## Slot limits

There are 8 custom character slots, numbered `0` through `7`.

The library uses `code % 8` when selecting the slot, so values outside this range wrap around:

```cpp
lcd.saveCustomChar(8, symbol);
```

uses the same slot as code `0`.

For predictable code, use values from `0` to `7`.

## See also

* [Virtual slots](virtual-slots.md)
* [Basic usage](basic-usage.md)
* [Graphics](graphics.md)
