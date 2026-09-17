# Installation

## PlatformIO

Add the library to `platformio.ini`

```ini
lib_deps =
    ania-7abc/Lcd2004
```

Then include the header you need in your source

```cpp
#include <Lcd2004.h>
```

## Arduino IDE

1. Download the archive from the [latest release](https://github.com/ania-7abc/Lcd2004/releases/latest)
2. Open *Sketch → Include Library → Add .ZIP Library*
3. Pick the downloaded file

The library shows up under *Sketch → Include Library* afterwards

## Manual install

Copy the `Lcd2004` folder into your project's `lib` directory (PlatformIO) or into `~/Arduino/libraries` (Arduino IDE)

## Headers

The library ships four headers. Include only the one you need, since each pulls in more code than the previous one

| Header         | Adds                                   |
|----------------|----------------------------------------|
| `Lcd2004.h`    | Core driver, parallel and I2C          |
| `Lcd2004xs.h`  | Virtual glyph slots on top of the core |
| `Lcd2004ru.h`  | UTF-8 parser and Cyrillic glyphs       |
| `Lcd2004gfx.h` | Pixel graphics on top of virtual slots |

`Lcd2004gfx.h` needs the [GyverGFX](https://github.com/GyverLibs/GyverGFX) library installed. The other three have no external dependencies

## Next

- [Wiring](wiring.md) for a parallel display
- [I2C quickstart](i2c-quickstart.md) for a PCF8574 backpack
- [Hello, world](hello-world.md) for the first sketch
