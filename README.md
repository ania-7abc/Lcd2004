# Lcd2004
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/ania-7abc/library/Lcd2004.svg)](https://registry.platformio.org/libraries/ania-7abc/Lcd2004)
[![GitHub License](https://img.shields.io/github/license/ania-7abc/Lcd2004)](https://github.com/ania-7abc/Lcd2004/blob/main/LICENSE)
[![GitHub Downloads (all assets, latest release)](https://img.shields.io/github/downloads/ania-7abc/Lcd2004/latest/total)](https://github.com/ania-7abc/Lcd2004/releases/latest)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ania-7abc/Lcd2004)

A clean, lightweight library for liquid crystal displays that is easy to set up

## Lcd2004 — the main conductor

This class rules the display. It can talk to it over 8 wires, 4 wires, or I2C (unless you forbid it). It buddies up with `Print`, so you can shove strings, numbers, and even floats into it via `print()` — unless you chop that feature off to save memory.

---

### Constructors — how they look in different universes

All constructors have a dummy trailing parameter `_` (default 0). It just sits there, reserved for future extensions. The other parameters jump in and out depending on what you've defined.

#### 8‑bit mode — always with you

```cpp
Lcd2004(RS, E, D0,D1,D2,D3, D4,D5,D6,D7, [cols, rows], [BLA], _);
```

- If you define `LCD_D_REMOVE_COLS_AND_ROWS_VARS` — the library stops storing the resolution. Then the `cols` and `rows` parameters **disappear** from the constructor. You don't pass them, and the library simply knows they have nowhere to live.
- If you add `LCD_D_BACKLIGHT_CONTROL_SUPPORT` — a `BLA` (backlight pin) parameter appears right before the `_`.
- Order: first all pins, then (if present) `cols, rows`, then (if present) `BLA`, then `_`.

#### 4‑bit mode — alive until you kill it

Available unless `LCD_D_DISABLE_4_BIT_MODE` is defined.

```cpp
Lcd2004(RS, E, D4,D5,D6,D7, [cols, rows], [BLA], _);
```

Internally it just calls the 8‑bit constructor, feeding D4–D7 as both high and low nibbles (because in 4‑bit mode the same pins are used for both halves). The same rules for dropping `cols/rows` and adding `BLA` apply.

#### I2C mode — alive until you turn it off

Available unless `LCD_D_NO_I2C` is defined.

```cpp
Lcd2004(addr, [cols, rows], _);
```

Hard‑wired for PCF8574: RS=0, E=2, D4=4, D5=5, D6=6, D7=7, and if backlight support is on, BLA=3.  
Again, if `LCD_D_REMOVE_COLS_AND_ROWS_VARS` is defined, `cols` and `rows` vanish, and the constructor becomes just `Lcd2004(addr, _);`.

---

### `init()` — it can shapeshift too

`init()` is not a fixed function. Its signature dances to the tune of two defines:

```cpp
void init(
  [uint8_t rows,]
  [uint8_t font = LCD_FONT_5X8,]
  uint8_t _ = 0
);
```

- If `LCD_D_REMOVE_COLS_AND_ROWS_VARS` is defined — a `rows` parameter appears (because we don't store the resolution, but we still need to know how many lines the display has during initialisation).
- If `LCD_D_DISABLE_FONT_SELECTION` is **not** defined — a `font` parameter appears (you can choose 5x8 or 5x10).
- If both conditions are met — `init(rows, font, _)`. If only the first — `init(rows, _)`. If only the second — `init(font, _)`. If none — `init(_)` or just `init()` (since `_` has a default).

What does `init()` do inside?  
- Waits 40 ms, unless `LCD_D_DISABLE_PRE_INIT_DELAY` is defined.
- Initialises the display in the chosen mode (8/4 bit, number of lines, font).
- Turns the display on, cursor (if allowed), and if `clear()` is not disabled — clears the screen (or, if the buffer is enabled, just fills it with spaces and then calls `flush()`).

---

### Other methods — who, where, and when they vanish

#### `clear()` — wipes everything
- Disappears if `LCD_D_DISABLE_CLEAR_FUNCTION` is defined.
- Without buffer: sends the clear command immediately.
- With buffer: fills the buffer with spaces; the actual clear happens on `flush()`.

#### `returnHome()` — back to the start
- Vanishes with `LCD_D_DISABLE_RETURN_HOME_FUNCTION`.
- Sends the "home" command and resets internal coordinates (if stored) and buffer position.

#### `reset()` — full reset
- Gone if `LCD_D_DISABLE_RESET_FUNCTION` is defined.
- Does `clear()`, then (if buffer is used) `flush()`, then `setPosition(0,0)` (if available).

#### `setOn(bool)` and `setCursor(bool, bool)` — screen and cursor control
- Both disappear if `LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL` is defined.
- The first turns the display on/off, the second enables the cursor and its blinking.

#### `setPosition(x, y)` — jump to a spot
- Disappears with `LCD_D_DISABLE_SET_POSITION_FUNCTION`.
- Computes the DDRAM address considering columns, rows, and the 4‑line addressing trick (addresses: 0x00, 0x40, 0x14, 0x54 — unless 4‑line support is disabled via `LCD_D_DISABLE_4_LINES_SUPPORT`).  
- Updates `cur_x`, `cur_y` (if they are not removed) and buffer position (if buffer exists).

#### `saveCustomChar(code, symbol[8])` — store your own glyph
- Vanishes with `LCD_D_DISABLE_CUSTOM_CHARACTERS`.
- Writes 8 bytes to CGRAM under the given `code` (0–7). After writing, it restores the cursor to its previous position using `setPosition()` (if it's still alive).

#### `setBacklight(bool)` — backlight control
- Appears **only** if `LCD_D_BACKLIGHT_CONTROL_SUPPORT` is defined.
- Toggles the pin (or changes the bit in the I2C byte).

#### `write(uint8_t)` — the main worker method
It's always there (even if `Print` is disabled, it remains as a plain method, just not overriding a virtual function).  
Its behaviour is a tangle of conditionals:

- If special character handling is **not** disabled (`LCD_D_DISABLE_SPECIAL_CHARACTERS`), it recognises `\n`, `\r`, `\b`:
  - `\n` — move to next line. Unless the classical special‑characters mode is on (`LCD_D_USE_CLASSICAL_SPECIAL_CHARACTERS`), it also inserts a `\r` so the cursor goes to the beginning of the line.
  - `\r` — move to column 0.
  - `\b` — step back. If classical mode is off, it also overwrites the previous character with a space (so it acts like a real backspace).
- For a regular character:
  - If buffer is enabled (`LCD_D_USE_BUFFER`) — stores the byte in the buffer at the current position.
  - Without buffer — sends the byte directly to the display with a tiny delay (4 µs).
- If auto line break is **not** disabled (`LCD_D_DISABLE_AUTO_LINE_BREAK`), after writing it checks whether the cursor has hit the right edge. If so, it calls `setPosition(0, ++cur_y)` and jumps to the next line.

#### `flush()` — drain the buffer
- Lives only when `LCD_D_USE_BUFFER` is defined.
- Walks through the buffer and sends all bytes to the display, respecting 1‑line and 4‑line addressing. The buffer is not cleared afterwards — if you want it empty, call `clear()`.

---

### And what about dependencies? They're tricky.

The library twists the knobs for you so you don't shoot yourself in the foot:

- If you disable `setPosition()` — auto line break and custom characters are automatically disabled (without positioning they are useless).
- If you disable auto line break — special characters are automatically disabled (because `\n` without wrapping makes no sense).
- If you remove the `cols/rows` variables — auto line break and 4‑line support are automatically disabled. And if you disable both auto line break and 4‑line support — the library will define `LCD_D_REMOVE_COLS_AND_ROWS_VARS` itself to save RAM.
- If you remove `cur_x/cur_y` — auto line break is disabled, and the buffer is forcibly turned off (because without coordinates the buffer doesn't know where to write). If auto line break is off and there's no buffer, the library will automatically remove `cur_x/cur_y`.
- If you disable 4‑bit mode — I2C also gets disabled (since I2C works through the 4‑bit protocol).
- There's also the total preset `LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS` — it kills everything it can, including I2C and the buffer, leaving only a naked 8‑bit driver with `init()` and `write()`. Perfect for the tiniest projects.

---

That's it. Constructors shift, `init()` dances, methods disappear like magic, and dependencies are twisted into a tight knot that the library unties for you. All so you can squeeze the most out of your Arduino, even if you have only 2 bytes of RAM left.

---

## Now about the others — briefly

### Lcd2004gfx — drawing pixels on a character display

This is a tertiary (and heaviest) beast. It requires the **GyverGFX** library and forces you to enable the buffer (`LCD_D_USE_BUFFER`) and disable `Print` (`LCD_D_DISABLE_PRINT`), otherwise it won't compile.

It turns your 20×4 (or whatever) display into a graphical canvas. By default each character is split into **2×2** pixels, so the resolution becomes `(cols*2) × (rows*2)`. If you want taller — define `LCD_GFX_LARGE`, and you'll get **2×3** pixels per character (then `rows*3` in height).

It works simply: draw lines, circles, rectangles — everything from `GyverGFX`, then call `update()`, and the library magically turns your picture into a set of custom characters, stuffs them into slots, and outputs them to the screen. It's magical, but slow — don't expect 60 fps animation.

---

### Lcd2004ru — Russian language and UTF-8

A secondary class for those who want to write in Russian. It knows how to parse UTF-8, extract Cyrillic letters, and substitute them with ready‑made patterns (big and small — like on old‑school Soviet displays). Letters that look like Latin ones (`A`, `C`, `O`, etc.) are canonicalised to avoid duplicating the same glyph in the slots.

No new methods — just create an object and `print("Привет!")`. Everything else is done automatically: decoding, drawing glyphs, writing them into free slots.

---

### Internal `_Lcd2004sc` — slot manager (and a warning!)

Both `gfx` and `ru` inherit from **`_Lcd2004sc`**, which does not exist in the base `Lcd2004`. This class adds a smart dynamic table: it remembers which custom character lives in which slot, and reuses slots if the same glyph is requested again.

**Important warning:** if you manually mess with the slots (via `saveCustomChar()` with codes 0–7) and occupy all 8 slots, and then `gfx` or `ru` ask for one more — **`_Lcd2004sc` will not gently evict one slot. It will simply throw away all your manual characters and wipe the entire table** to free up space. This is done on purpose to avoid deadlocks. If you don't want such a surprise, either don't interfere with the automatics, or don't occupy all slots with your own hands.

That's all. These two classes are add‑ons to `Lcd2004`; they are not mandatory, but they are useful if you need to draw or write in Russian. And `_Lcd2004sc` is their shared internal kitchen — something worth knowing so you don't get an unexpected reset.

## Feedback
If you find a bug, create an [Issue](https://github.com/ania-7abc/Lcd2004/issues). If you want to get an answer to the problem faster, write to me in [Telegram](https://t.me/ania_7a)

If you write to Telegram, it's better to just send a link to [Telegraph](https://telegra.ph)

### Be sure to specify:
* If you write by email, Telegram: Library name (Lcd2004)
* Library version
* The MC where the bug occurs
* If available: SDK version for development
* How do I get the bug?
* Display type. Examples: 16x2, 20x4, 6x1
* Is there a bug in the examples?
* How should I address you?
