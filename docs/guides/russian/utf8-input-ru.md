# UTF-8 input

`Lcd2004ru` decodes UTF-8 inside the `preWrite` hook. Every byte the `Print` layer produces passes through the parser before it reaches the panel

## The parser

The parser is a state machine that consumes one byte per call and tracks whether a multi-byte sequence is in progress

```cpp
bool parseUtf8(const uint8_t byte)
```

The return value means "a full code point is ready"

- `true` — the byte completed a code point, `buf` holds the value
- `false` — the byte was part of a sequence, keep calling

The `preWrite` override uses that

```cpp
bool preWrite(uint8_t& byte) override
{
    if (!parseUtf8(byte))
        return false;
    if (!utf8)
        return true;

    buf = getCanonical(buf);
    if (buf <= 0x7F)
    {
        byte = buf;
        return true;
    }

    const uint8_t code = toVirtualCode(buf);
    if (!isRegistered(code))
    {
        uint8_t symbol[8];
        getGlyph(buf, symbol);
        saveCustomChar(code, symbol);
    }
    byte = loadVirtualChar(code);
    return true;
}
```

Returning `false` tells the base class to skip write. Returning `true` with a modified `byte` sends the resolved hardware slot. The `utf8` flag distinguishes a complete single-byte code point (passed through unchanged) from a complete multi-byte sequence (resolved through the canonical table and the virtual slot machinery)

## Sequence lengths

| First byte | Length | Codepoint bits    |
|------------|--------|-------------------|
| `0xxxxxxx` | 1      | 7                 |
| `110xxxxx` | 2      | 11                |
| `1110xxxx` | 3      | 16                |
| `11110xxx` | 1      | 8 (not supported) |
| `10xxxxxx` | —      | Continuation      |

Cyrillic is always 2 bytes in UTF-8, so the parser's 2-byte path covers the whole Russian alphabet. A 4-byte lead byte is treated as a single-byte code point equal to the byte itself; the parser does not consume the trailing continuation bytes as part of that sequence

## Codepoint to slot

`toVirtualCode` maps a code point to a virtual slot number

```cpp
static uint8_t toVirtualCode(uint16_t code)
{
    if (code >= 0x0410 && code <= 0x042F)
    {
        uint8_t idx = 0;
        for (uint16_t c = 0x0410; c < code; ++c)
            if (getCanonical(c) == c)
                ++idx;

        return LCD_VIRTUAL_SLOTS - 43 + idx;
    }

    if (code >= 0x0430 && code <= 0x044F)
    {
        uint8_t idx = 20;
        for (uint16_t c = 0x0430; c < code; ++c)
            if (getCanonical(c) == c)
                ++idx;

        return (LCD_VIRTUAL_SLOTS - 43 + idx);
    }

    return (code & 0x3F) % LCD_VIRTUAL_SLOTS - 43;
}
```

The 43 non-canonical Cyrillic letters are packed into the top of the virtual range, starting at `LCD_VIRTUAL_SLOTS - 43`

| Subrange                 | Count | Index base |
|--------------------------|-------|------------|
| Uppercase, non-canonical | 20    | 0          |
| Lowercase, non-canonical | 23    | 20         |

Uppercase maps to slots `LCD_VIRTUAL_SLOTS - 43` through `LCD_VIRTUAL_SLOTS - 24`, lowercase to `LCD_VIRTUAL_SLOTS - 23` through `LCD_VIRTUAL_SLOTS - 1`

## Writing from a subclass

Because the parser lives in `preWrite`, a subclass of `Lcd2004ru` can override the hook and add behavior before calling the parent

```cpp
class MyLcd : public Lcd2004ru {
protected:
    bool preWrite(uint8_t& byte) override {
        // Skip a code point entirely if some condition holds
        if (suppressNext && byte == '\n') {
            suppressNext = false;
            return false;
        }
        return Lcd2004ru::preWrite(byte);
    }

public:
    bool suppressNext = false;
};
```

The base class call must run, otherwise the parser state is never updated and multi-byte sequences break

## Debugging

If Cyrillic shows as garbage, check the source encoding first

## See also

- [Cyrillic support](cyrillic-support.md)
- [Print integration](../core/print-integration.md)
