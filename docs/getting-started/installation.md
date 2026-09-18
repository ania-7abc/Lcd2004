# Installation

## Introduction

This page explains how to add the Lcd2004 library to your project. The steps differ for PlatformIO and Arduino IDE. The library itself has no mandatory dependencies for basic use. If you plan to use the graphics class, you also need the GyverGFX library

## PlatformIO

In PlatformIO, add the library to your `platformio.ini` file under `lib_deps`

```ini
lib_deps =
    ania-7abc/Lcd2004
```

Then build your project. PlatformIO will download and install the library automatically

## Arduino IDE

For Arduino IDE, you need to download the library as a ZIP file. Go to the PlatformIO registry page for `ania-7abc/Lcd2004` and use the download option to get the ZIP. Then open Arduino IDE, go to Sketch > Include Library > Add .ZIP Library... and select the downloaded file

## Dependencies

If you want to use the graphics features, install the GyverGFX library as well. It is available through the Arduino Library Manager and PlatformIO. The other classes have no external dependencies

## See also

- [Getting started index](index.md)
- [Quickstart](quickstart.md)
- [Configuration index](../configuration/index.md)
