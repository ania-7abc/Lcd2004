# Print integration

`Lcd2004` derives from Arduino's `Print` class. Anything that works with `Serial` also works here: numbers, floats, characters, strings stored in flash, and custom classes that implement `Print`

## The write override

`Print` funnels every output through a single method

```cpp
size_t write(uint8_t byte) override;
```

The driver's override handles four things

1. Runs the optional `preWrite` hook
2. Interprets `\n`, `\r` and `\b`
3. Sends the byte to DDRAM, or stores it in the shadow buffer
4. Advances the column and emits a newline when the column reaches `cols_`

Every `print` and `println` overload eventually calls this one method. Overriding `preWrite` in a subclass is enough to intercept all output. That is how `Lcd2004ru` plugs UTF-8 decoding in without touching the base class

## Number formatting

Everything from `Print` is available

```cpp
lcd.print(42);            // "42"
lcd.print(-17);           // "-17"
lcd.print(3.14159, 2);    // "3.14"
lcd.print(255, HEX);      // "FF"
lcd.print(255, BIN);      // "11111111"
lcd.print('A');           // "A"
lcd.print("text");        // "text"
```

For small MCUs, prefer integer math and format manually

```cpp
lcd.print(value / 10);
lcd.print('.');
lcd.print(value % 10);
```

## Flash strings

Wrap constant strings in `F()` to keep them out of RAM on AVR

```cpp
lcd.println(F("Booting..."));
```

Without `F()`, the string is copied into SRAM at startup and stays there

## The preWrite hook

A subclass can inspect and modify each byte before it reaches the panel

```cpp
class Upper : public Lcd2004 {
protected:
    bool preWrite(uint8_t& byte) override {
        if (byte >= 'a' && byte <= 'z')
            byte -= 32;
        return true;
    }
};
```

Return `false` to swallow the byte. The base class skips the DDRAM write and returns from `write` as if the byte had been sent

This hook is used by `Lcd2004ru` to consume UTF-8 continuation bytes. The parser returns `false` for each byte that is part of a multi-byte sequence, and only forwards the final resolved code point

## Writing raw bytes

`write(uint8_t)` is public. Use it directly when you do not want number formatting

```cpp
lcd.write('A');
lcd.write(0xDF); // Custom char at code 0xDF, if registered
```

Codes `0` to `7` map to hardware CGRAM slots on the base class

## See also

- [Text navigation](text-navigation.md)
- [Custom chars](../custom-chars/cgram-basics.md)
- [UTF-8 input](../russian/utf8-input-ru.md)
