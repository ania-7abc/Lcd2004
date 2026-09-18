#!/usr/bin/env python3
"""
convert_glyphs.py

Converts the old HD44780 custom symbol format (8 bytes per symbol,
byte = row, bits 0..4 = pixels from left to right) to the new format
(5 bytes per symbol, byte = column, bit 0 = top pixel, bit 7 = bottom).

Usage:
    python convert_glyphs.py input.c
    python convert_glyphs.py < input.c
    python convert_glyphs.py input.c --bin
"""

import re
import sys
import argparse

# Regular expression for searching array declarations
# Supports: const unsigned char name[] = {...};
#               uint8_t name[8] = {...};
ARRAY_RE = re.compile(
    r'(?:const\s+)?(?:unsigned\s+char|uint8_t)\s+(\w+)\s*\[\s*\d*\s*\]\s*=\s*\{([^}]*)\}\s*;',
    re.MULTILINE | re.DOTALL
)

def strip_comments(text: str) -> str:
    """Removes // and /* */ comments."""
    text = re.sub(r'//.*', '', text)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.DOTALL)
    return text

def parse_initializer(init_str: str):
    """Parses the contents of curly braces into a list of integers."""
    init_str = strip_comments(init_str)
    tokens = [t.strip() for t in init_str.split(',') if t.strip()]
    values = []
    for tok in tokens:
        # Support 0x..., 0b..., decimal
        try:
            values.append(int(tok, 0))
        except ValueError:
            raise ValueError(f"Failed to parse value: {tok!r}")
    return values

def convert_8_to_5(old_bytes):
    """
    old_bytes: a list of 8 integers (0..255).
    Returns a list of 5 integers — the new format.
    """
    if len(old_bytes) != 8:
        raise ValueError("Exactly 8 bytes per character are expected")
    new_bytes = []
    for col in range(5):
        # col=0 — left column, bit 4 of each row
        # col=4 — right column, bit 0
        bit_pos = 4 - col
        val = 0
        for row in range(8):
            if (old_bytes[row] >> bit_pos) & 1:
                val |= (1 << row)
        new_bytes.append(val)
    return new_bytes

def format_array(name: str, data, binary=False) -> str:
    """Generates C code for a new array."""
    line = f"const uint8_t {name}[] = {{"
    if binary:
        s = '\n    ' + ',\n    '.join(f'0b{b:08b}' for b in data) + '\n'
    else:
        s = ', '.join(f'0x{b:02X}' for b in data)
    line += f"{s}}};"
    return line

def process_text(text: str, binary=False):
    """Finds all arrays, converts them, and returns a list of strings."""
    results = []
    for match in ARRAY_RE.finditer(text):
        name = match.group(1)
        init_str = match.group(2)
        try:
            old_data = parse_initializer(init_str)
        except ValueError as e:
            print(f"# Error in array {name}: {e}", file=sys.stderr)
            continue

        if len(old_data) % 8 != 0:
            print(f"# Warning: the length of array {name} ({len(old_data)}) is not a multiple of 8. Skipped.", file=sys.stderr)
            continue

        new_data = []
        for i in range(0, len(old_data), 8):
            chunk = old_data[i:i+8]
            new_data.extend(convert_8_to_5(chunk))

        results.append(format_array(name, new_data, binary))
    return results

def main():
    parser = argparse.ArgumentParser(
        description="Converter of old custom HD44780 symbols (8 bytes/symbol) to the new format (5 bytes/symbol)."
    )
    parser.add_argument(
        "input",
        nargs="?",
        default="-",
        help="Input C file. Default is stdin."
    )
    parser.add_argument(
        "--bin",
        action="store_true",
        help="Output bytes in binary form (0b...) instead of hex."
    )
    args = parser.parse_args()

    if args.input == "-":
        text = sys.stdin.read()
    else:
        with open(args.input, "r", encoding="utf-8") as f:
            text = f.read()

    converted = process_text(text, binary=args.bin)
    if not converted:
        print("# No suitable arrays found.", file=sys.stderr)
        sys.exit(1)

    if args.bin:
        print('\n\n'.join(converted))
    else:
        print('\n'.join(converted))

if __name__ == "__main__":
    main()
