# LCD_USE_BUFFER

Keeps an in-RAM shadow copy of the screen so updates do not flicker

## Define

```cpp
#define LCD_USE_BUFFER
#include <Lcd2004.h>
```

## Effect

Two things change in the base driver

`write` no longer talks to the display directly. Characters go into a `buffer_` array indexed by the current cursor position. The display only updates when `flush` is called

`clear` fills the buffer with spaces instead of issuing the HD44780 clear command. This is instant and does not incur the 1.5 ms hardware delay

## When to use

- You redraw part of the screen on every loop iteration and see flicker
- You want atomic updates: prepare the whole frame, then push it in one go
- You already keep your own frame buffer and want the library to own it instead

## Buffer size

`0x68` bytes, or 104 characters. That covers a 20x4 display with room to spare and matches the DDRAM layout: two 40-character rows interleaved for the four-row mode

Rows 0 and 2 share one address range, rows 1 and 3 share another. `setPosition` computes the correct DDRAM offset, so the buffer stays in sync

## flush

`flush` sends `LCD_CMD_RETURN_HOME` and then walks the buffer, writing each byte to the display. It skips addresses 0x28 to 0x3F on multi-row displays, since those do not exist in hardware

Each character is followed by a 40 us delay, so a full 20x4 refresh takes around 4 ms

`flush` is declared `override` and derives from `Print`. Code that only needs to push once can call it directly after `print` calls fill the buffer

## Initial state

`init` calls `clear` then `flush`. The buffer starts as spaces and the display starts blank

## Example

```cpp
#define LCD_USE_BUFFER
#include <Lcd2004.h>

Lcd2004 lcd(12, 11, 5, 4, 3, 2, 20, 4);

void setup() {
    lcd.init();
}

void loop() {
    lcd.clear();
    lcd.print(millis());
    lcd.flush();
    delay(100);
}
```
