#!/usr/bin/env python3
"""
Convert HD44780 custom symbol format (8 bytes per symbol) to new format (5 bytes per symbol)
"""

import re
import sys
import argparse

DECL_RE = re.compile(
    r'(?P<const>const\s+)?'
    r'(?:unsigned\s+char|uint8_t|byte)\s+'
    r'(?P<name>\w+)\s*'
    r'(?P<dims>(?:\s*\[[^]]*])*)\s*=\s*\{',
    re.MULTILINE | re.DOTALL
)


def strip_comments(text):
    text = re.sub(r'//.*', '', text)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.DOTALL)
    return text


def extract_initializer(text, start_idx):
    depth = 0
    i = start_idx
    while i < len(text):
        if text[i] == '{':
            depth += 1
        elif text[i] == '}':
            depth -= 1
            if depth == 0:
                return text[start_idx + 1:i], i + 1
        i += 1
    raise ValueError("Unmatched braces")


def parse_values(init_str):
    tokens = re.findall(r'0[xX][0-9a-fA-F]+|0[bB][01]+|[bB][01]+|\d+', init_str)
    values = []
    for tok in tokens:
        try:
            if tok[0] in 'Bb' and not tok.startswith(('0b', '0B')):
                values.append(int(tok[1:], 2))
            else:
                values.append(int(tok, 0))
        except ValueError:
            raise ValueError(f"Failed to parse value: {tok!r}")
    return values

def convert_8_to_5(old_bytes):
    if len(old_bytes) != 8:
        raise ValueError("Exactly 8 bytes per character are expected")
    new_bytes = []
    for col in range(5):
        bit_pos = 4 - col
        val = 0
        for row in range(8):
            if (old_bytes[row] >> bit_pos) & 1:
                val |= (1 << row)
        new_bytes.append(val)
    return new_bytes


def fmt_byte(b, binary):
    return f'0b{b:08b}' if binary else f'0x{b:02X}'


def get_size_expr(dims_str, num_symbols):
    dims = re.findall(r'\[([^]]*)]', dims_str)
    if not dims:
        return None

    if len(dims) >= 2:
        expr = dims[0].strip()
        return expr if expr else None

    dim = dims[0].strip()
    if not dim:
        return None

    try:
        val = int(dim, 0)
        if val == 8:
            return None
        if val % 8 == 0:
            n = val // 8
            return None if n == 1 else str(n)
        return None
    except ValueError:
        pass

    m = re.match(r'^(.*?)\s*\*\s*8\s*$', dim)
    if m:
        inner = m.group(1).strip()
        if inner:
            return inner

    if num_symbols == 1:
        return None
    return str(num_symbols)


def process_text(text, binary=False):
    text = strip_comments(text)
    results = []
    pos = 0
    while True:
        m = DECL_RE.search(text, pos)
        if not m:
            break

        name = m.group('name')
        has_const = m.group('const') is not None
        brace_idx = m.end() - 1

        try:
            init_str, end_idx = extract_initializer(text, brace_idx)
        except ValueError as e:
            print(f"# Error in array {name}: {e}", file=sys.stderr)
            pos = m.end()
            continue

        try:
            old_data = parse_values(init_str)
        except ValueError as e:
            print(f"# Error in array {name}: {e}", file=sys.stderr)
            pos = end_idx
            continue

        if not old_data:
            pos = end_idx
            continue

        if len(old_data) % 8 != 0:
            print(f"# Warning: length of array {name} ({len(old_data)}) is not a multiple of 8, skipped",
                  file=sys.stderr)
            pos = end_idx
            continue

        num_symbols = len(old_data) // 8
        new_data = []
        for i in range(0, len(old_data), 8):
            new_data.extend(convert_8_to_5(old_data[i:i + 8]))

        prefix = "const " if has_const else ""
        size_expr = get_size_expr(m.group('dims'), num_symbols)

        if size_expr is None:
            vals = ', '.join(fmt_byte(b, binary) for b in new_data)
            results.append(f"{prefix}uint8_t {name}[] = {{{vals}}};")
        else:
            lines = []
            for i in range(0, len(new_data), 5):
                chunk = new_data[i:i + 5]
                lines.append('    ' + ', '.join(fmt_byte(b, binary) for b in chunk) + ',')
            body = '\n'.join(lines)
            results.append(f"{prefix}uint8_t {name}[{size_expr} * 5] = {{\n{body}\n}};")

        pos = end_idx

    return results

def main():
    parser = argparse.ArgumentParser(
        description="Convert HD44780 custom symbols (8 bytes/symbol) to new format (5 bytes/symbol)"
    )
    parser.add_argument(
        "input",
        nargs="?",
        default="-",
        help="Input C file, default is stdin"
    )
    parser.add_argument(
        "--bin",
        action="store_true",
        help="Output bytes in binary form (0b...) instead of hex"
    )
    args = parser.parse_args()

    if args.input == "-":
        text = sys.stdin.read()
    else:
        with open(args.input, "r", encoding="utf-8") as f:
            text = f.read()

    converted = process_text(text, binary=args.bin)
    if not converted:
        print("# No suitable arrays found", file=sys.stderr)
        sys.exit(1)

    if args.bin:
        print('\n\n'.join(converted))
    else:
        print('\n'.join(converted))

if __name__ == "__main__":
    main()
