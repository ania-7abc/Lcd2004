# Lcd2004
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/ania-7abc/library/Lcd2004.svg)](https://registry.platformio.org/libraries/ania-7abc/Lcd2004)
[![GitHub License](https://img.shields.io/github/license/ania-7abc/Lcd2004)](https://github.com/ania-7abc/Lcd2004/blob/main/LICENSE)
[![GitHub Downloads (all assets, latest release)](https://img.shields.io/github/downloads/ania-7abc/Lcd2004/latest/total)](https://github.com/ania-7abc/Lcd2004/releases/latest)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ania-7abc/Lcd2004)

A clean, lightweight library for liquid crystal displays that is easy to set up

## 📖 Documentation

### Installation

**PlatformIO**  
Add to `platformio.ini`:
```ini
lib_deps = ania-7abc/Lcd2004
```

**Arduino IDE**  
Download ZIP from [GitHub Releases](https://github.com/ania-7abc/Lcd2004/releases) and install via `Sketch → Include Library → Add .ZIP Library`.

---

### Constructors

| Mode | Constructor | Description |
|------|-------------|-------------|
| **8‑bit parallel** | `Lcd2004(RS, E, D0..D7, cols, rows)` | Full 8‑bit data bus. Uses all D0–D7 pins. |
| **4‑bit parallel** | `Lcd2004(RS, E, D4, D5, D6, D7, cols, rows)` | 4‑bit data bus (D4–D7). Saves 4 pins. |
| **I2C (PCF8574)** | `Lcd2004(addr, cols, rows)` | Standard I2C backpack. Bus: SDA, SCL. Need to call `Wire.begin()` before `init()`. |

All constructors automatically call `pinMode()` (can be disabled by `LCD_D_DISABLE_PIN_MODE`).

---

### Public Methods

| Method | Description | Notes |
|--------|-------------|-------|
| `init(font = LCD_FONT_5X8)` | Initialises display. Must be called once in `setup()`. Optional font: `LCD_FONT_5X8` or `LCD_FONT_5X10`. | |
| `clear()` | Clears screen and returns home. | Disabled by `LCD_D_DISABLE_CLEAR_FUNCTION`. |
| `returnHome()` | Moves cursor to (0,0) without clearing. | Disabled by `LCD_D_DISABLE_RETURN_HOME_FUNCTION`. |
| `setPosition(x, y)` | Moves cursor to column `x`, row `y`. | Disabled by `LCD_D_DISABLE_SET_POSITION_FUNCTION`. |
| `setOn(bool on)` | Turns display on/off. | Disabled by `LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL`. |
| `setCursor(bool enable, bool blink)` | Controls cursor visibility and blinking. | Same macro. |
| `saveCustomChar(uint8_t code, uint8_t bitmap[8])` | Stores custom character in CGRAM (`code` 0…7). | Disabled by `LCD_D_DISABLE_CUSTOM_CHARACTERS`. |
| `write(uint8_t byte)` | Writes one character (inherited from `Print`). | |
| `print()` / `println()` | Available if `Print` not disabled. | Disabled by `LCD_D_DISABLE_PRINT`. |
| `flush()` | Sends buffered data to display. | Only when `LCD_D_USE_BUFFER` is defined. |
| `reset()` | Soft reset: clears, returns home, sets position (0,0). | Disabled by `LCD_D_DISABLE_RESET_FUNCTION`. |
| `setBacklight(bool on)` | Turns the display backlight on/off. | Available only when `LCD_D_BACKLIGHT_CONTROL_SUPPORT` is defined |

---

### Configuration Macros

Define any of these **before** including `Lcd2004.h` to trim features and save memory.

| Macro | Effect |
|-------|--------|
| `LCD_D_DISABLE_SPECIAL_CHARACTERS` | No handling of `\n`, `\r`, `\b`. |
| `LCD_D_USE_CLASSICAL_SPECIAL_CHARACTERS` | Classical terminal behaviour (no extra corrections). |
| `LCD_D_DISABLE_PRINT` | Remove `Print` inheritance. Only `write()` remains. |
| `LCD_D_DISABLE_4_BIT_MODE` | Force 8‑bit mode. I2C automatically disabled. |
| `LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL` | No `setOn()`/`setCursor()`. Display always on. |
| `LCD_D_DISABLE_CLEAR_FUNCTION` | No `clear()`. |
| `LCD_D_DISABLE_RETURN_HOME_FUNCTION` | No `returnHome()`. |
| `LCD_D_DISABLE_CUSTOM_CHARACTERS` | No `saveCustomChar()`. |
| `LCD_D_DISABLE_AUTO_LINE_BREAK` | No automatic line wrap. |
| `LCD_D_DISABLE_4_LINES_SUPPORT` | Treat display as 2 lines max. |
| `LCD_D_DISABLE_SET_POSITION_FUNCTION` | No `setPosition()`. |
| `LCD_D_DISABLE_PRE_INIT_DELAY` | Skip 40 ms init delay (faster, less reliable). |
| `LCD_D_DISABLE_PIN_MODE` | Do not call `pinMode()`. Set pins externally. |
| `LCD_D_DISABLE_FONT_SELECTION` | Ignore `font` parameter, always 5×8. |
| `LCD_D_REMOVE_COLS_AND_ROWS_VARS` | Remove `cols`, `rows` members. Disables auto line break & 4‑line support. |
| `LCD_D_REMOVE_CUR_X_AND_Y_VARS` | Remove `cur_x`, `cur_y`. Disables auto line break & buffer. |
| `LCD_D_USE_BUFFER` | Enable internal buffer. Use `flush()` to output. |
| `LCD_D_FULL_ACCESS` | Make protected members public (low‑level hacking). |
| `LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS` | Ultra minimal: only `init()` and `write()`. |
| `LCD_D_DISABLE_RESET_FUNCTION` | Remove `reset()`. |
| `LCD_D_NO_I2C` | Completely disable I2C (no `Wire.h`). |
| `LCD_S_BUFFER_SIZE` | Set buffer size (default 0x50). Used only with `LCD_D_USE_BUFFER`. |
| `LCD_D_BACKLIGHT_CONTROL_SUPPORT` | Enables backlight control |

---

### Example

```cpp
#include <Lcd2004.h>

Lcd2004 lcd(0x27, 16, 2);

void setup() {
    Wire.begin();
    lcd.init();
    lcd.print("Hello, world!");
}

void loop() {}
```

## 🇷🇺 Russian Language Support (`lcd2004ru.h`)

This extension adds automatic Cyrillic (Russian) support to the base `Lcd2004` library. It handles UTF‑8 input and manages CGRAM slots for you – no manual glyph creation is needed.

### ⚠️ Important Notes

- The library **automatically uses CGRAM slots** for Cyrillic characters.
- **Do not use `saveCustomChar()`** while this extension is active – it will interfere with automatic slot management.
- If all 8 CGRAM slots are occupied by your own custom characters, the library will be forced to **clear all slots** to make room for Cyrillic. This will erase **ALL** your custom characters
- For proper operation, **leave at least 1–2 slots free** or avoid character saving altogether

### `freeCustomChar(uint8_t code)`

Marks a user‑occupied CGRAM slot as free, allowing the library to reuse it for Cyrillic glyphs.

- **Parameters:** `code` – slot number (0..7).
- **Note:** Only works if the slot was previously marked as `LCD_USER_USED`.

### Example

```cpp
#include <lcd2004ru.h>

Lcd2004ru lcd(0x27, 16, 2);

void setup() {
    lcd.init();
    lcd.print("Привет, мир!"); // UTF‑8 string
}
```

All other methods (`print`, `setPosition`, `clear`, etc.) work exactly as in the base `Lcd2004` class.

### How It Works

- **UTF‑8 Parsing** – the library intercepts each byte, decodes UTF‑8 sequences, and correctly handles 1‑, 2‑, and 3‑byte characters (4‑byte are ignored).
- **Glyph Selection** – for Russian letters (А‑Я, а‑я), the library uses built‑in 5×7 pixel patterns stored in Flash. Visually identical Latin and Cyrillic characters (like `P` and `Р`) share the same glyph to save CGRAM space.
- **Slot Management** – each new non‑ASCII character gets its own CGRAM slot. Existing slots are reused when possible. If no free slots are available, the library tries to reuse unprotected slots. If all slots are occupied by your custom characters, **all slots are cleared** to make room for Cyrillic.
- **Rendering** – once stored, the character is displayed using its slot number (`0..7`), without any extra code on your side.

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
