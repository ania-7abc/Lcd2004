# Text navigation

The cursor sits at a column and row inside the display's memory. `setPosition` moves it, `print` advances it, and the driver wraps or scrolls at the edges according to where you are

## Coordinates

Columns run left to right starting at 0. Rows run top to bottom starting at 0. A 20x4 display has columns 0 to 19 and rows 0 to 3

```cpp
lcd.setPosition(0, 0);  // Top left
lcd.setPosition(19, 0); // Top right
lcd.setPosition(0, 3);  // Bottom left
```

Both arguments wrap modulo the display size

```cpp
lcd.setPosition(25, 0); // Same as (5, 0) on a 20-column display
lcd.setPosition(0, 5);  // Same as (0, 1) on a 4-row display
```

The wrap is deliberate. It lets `x + step` overflow without an explicit clamp. If you would rather stop at the edge, check the bounds yourself

## What setPosition does under the hood

`setPosition` computes a DDRAM address and sends a `SET_DDRAM_ADDRESS` command. The HD44780 has a non-linear address map for two-line displays: row 0 starts at 0x00, row 1 at 0x40, row 2 at 0x14, row 3 at 0x54

```cpp
const uint8_t addr = y % 2 * 0x40 + x + cols_ * (y >= 2);
```

The formula covers four-line panels and collapses to the two-line case when `rows_` is 1 or 2. You do not need to know the map

## Auto-wrap

After each character, the driver increments the column counter. When the counter reaches `cols_`, it emits a newline

```cpp
lcd.setPosition(18, 0);
lcd.print("abcd"); // "ab" on row 0, then "cd" on row 1
```

This matches the `Print` class convention and means you rarely need to call `setPosition` explicitly. Let the text flow

On the last row, the newline wraps back to row 0

```cpp
lcd.setPosition(0, 3);
lcd.print("01234567890123456789abc"); // "abc" appears at (0, 0)
```

## Manual newline and carriage return

`write('\n')` moves to column 0 of the next row. `write('\r')` moves to column 0 of the current row

```cpp
lcd.print("Loading\n");
lcd.print("100%\r");
lcd.print("done ");
```

The `\r` rewinds to the start of the row so the second print overwrites the first. Useful for updating a status line in place

## Backspace

`\b` is destructive. It steps back one column, writes a space, then steps back again

```cpp
lcd.print("abc\b "); // "ab" with the 'c' erased
```

At column 0, backspace steps into the previous row's last column. On row 0 it wraps to the last row

Backspace exists for terminal-style output. For redrawing a label, `setPosition` is clearer and one call shorter

## Home and clear

`returnHome` moves the cursor to (0, 0) and leaves the contents alone

```cpp
lcd.returnHome();
```

`clear` wipes the screen and also calls `returnHome`

```cpp
lcd.clear();
```

Both commands take around 1.5 ms on the HD44780. The library sleeps that long after sending them so the next write does not race the controller

`clear` on a buffered build erases the shadow buffer and does not send `CLEAR_DISPLAY` to the panel. The screen updates on the next `flush`. See [LCD_USE_BUFFER](../../configuration/LCD_USE_BUFFER.md)

## Reading the current position

The driver tracks `cur_x_` and `cur_y_` internally and updates them on every write. They are `protected`, so a subclass can read them

```cpp
class MyLcd : public Lcd2004 {
public:
    uint8_t column() const { return cur_x_; }
    uint8_t row()    const { return cur_y_; }
};
```

For a plain sketch, call `setPosition` to a known spot before writing instead of querying

## See also

- [Print integration](print-integration.md)
- [Display control](display-control.md)
