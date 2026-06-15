# Lcd2004
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/ania-7abc/library/Lcd2004.svg)](https://registry.platformio.org/libraries/ania-7abc/Lcd2004)
[![GitHub License](https://img.shields.io/github/license/ania-7abc/Lcd2004)](https://github.com/ania-7abc/Lcd2004/blob/main/LICENSE)
[![GitHub Downloads (all assets, latest release)](https://img.shields.io/github/downloads/ania-7abc/Lcd2004/latest/total)](https://github.com/ania-7abc/Lcd2004/releases/latest)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ania-7abc/Lcd2004)

A clean, lightweight library for liquid crystal displays that is easy to set up

## Usage
Take a look at the examples

### Available `#define` options and their effects

- **`LCD_D_DISABLE_SPECIAL_CHARACTERS`**  
  Disables handling of `\n`, `\r`, `\b` as special commands. They are printed as ordinary characters; cursor position does not change.

- **`LCD_D_USE_CLASSICAL_SPECIAL_CHARACTERS`**  
  Enables classical terminal behavior: `\n` moves down, `\r` returns to column 0, `\b` deletes previous character. Without this macro, extra handling (e.g., `\n` also issues `\r`) is added.

- **`LCD_D_DISABLE_PRINT`**  
  Removes inheritance from the `Print` class. You cannot use `print()` or `println()`; only `write(uint8_t)` works. Saves code space.

- **`LCD_D_DISABLE_4_BIT_MODE`**  
  Disables 4‑bit mode, forcing 8‑bit parallel interface. Requires all 8 data pins (D0–D7). Automatically disables I2C.

- **`LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL`**  
  Removes methods to turn display/cursor on/off and cursor blink. Display and cursor are always on (default state after init).

- **`LCD_D_DISABLE_CLEAR_FUNCTION`**  
  Removes `clear()` method. Screen cannot be cleared programmatically; you must overwrite or reset.

- **`LCD_D_DISABLE_RETURN_HOME_FUNCTION`**  
  Removes `returnHome()` method. Cursor cannot return to (0,0) except by using `setPosition()`.

- **`LCD_D_DISABLE_CUSTOM_CHARACTERS`**  
  Removes `saveCustomChar()` method. Cannot store custom character bitmaps in CGRAM. Saves Flash/RAM.

- **`LCD_D_DISABLE_AUTO_LINE_BREAK`**  
  Disables automatic line wrapping when cursor reaches end of line. Text continues beyond column limit (undefined behavior).

- **`LCD_D_DISABLE_4_LINES_SUPPORT`**  
  Treats display as having only 2 lines (or 1), even if hardware has 4. Saves code for smaller displays.

- **`LCD_D_DISABLE_SET_POSITION_FUNCTION`**  
  Removes `setPosition(x, y)` method. Cursor positioning is impossible; rely on auto‑line break or sequential writes.

- **`LCD_D_DISABLE_PRE_INIT_DELAY`**  
  Skips the 40 ms delay at the start of `init()`. Faster startup, but may cause unreliable initialization on some LCD modules.

- **`LCD_D_DISABLE_PIN_MODE`**  
  Prevents calling `pinMode()` for control pins in the constructor. You must set pin modes externally before creating the object. Saves code.

- **`LCD_D_DISABLE_FONT_SELECTION`**  
  Disables font choice (5×8 vs 5×10). Always uses 5×8 font; the `font` parameter in `init()` is ignored. Saves code.

- **`LCD_D_REMOVE_COLS_AND_ROWS_VARS`**  
  Removes storage of `cols` and `rows` member variables. Auto line break and 4‑line support are also disabled. Saves RAM.

- **`LCD_D_REMOVE_CUR_X_AND_Y_VARS`**  
  Removes `cur_x` and `cur_y` cursor position variables. Auto line break and buffer are disabled. Saves RAM.

- **`LCD_D_USE_BUFFER`**  
  Enables an internal buffer for output. Writes go to buffer; call `flush()` to send to display. Improves speed, uses RAM (`LCD_S_BUFFER_SIZE` bytes).

- **`LCD_D_FULL_ACCESS`**  
  Makes all protected members (`RS`, `E`, `D0`–`D7`, etc.) public. Allows low‑level access for advanced hacking. Use with caution.

- **`LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS`**  
  Ultra‑minimal mode. Disables nearly all features (special characters, `Print`, 4‑bit mode, display control, clear, home, custom chars, auto line break, 4‑line support, setPosition, pre‑init delay, font selection, remove vars, buffer, reset). Only `init()` and basic `write()` remain.

- **`LCD_D_DISABLE_RESET_FUNCTION`**  
  Removes `reset()` method. Cannot soft‑reset the display; only power‑cycle or re‑init. Saves code.

- **`LCD_D_NO_I2C`**  
  Disables I2C support completely. `Wire.h` is not included, and the I2C constructor is unavailable. Only parallel mode works. Saves code.

- **`LCD_S_BUFFER_SIZE`**  
  Sets the size of the internal buffer when `LCD_D_USE_BUFFER` is defined. Default is `0x50` (80 bytes). Larger buffer stores more characters but consumes more RAM.

## Installation
The library is available in PlatformIO named ania-7abc/Lcd2004

If you want to install it in the Arduino IDE, click Sketch → Include Library → Add .ZIP Library and select the zip with the library

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