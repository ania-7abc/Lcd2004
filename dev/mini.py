# gen_groups.py
import unicodedata

def to_hex(c):
    return f"0x{ord(c):04X}"

def generate_groups(pairs):
    print(f"#define CANONICAL_GLYPH_GROUPS_SIZE {len(pairs)}")
    print("const uint16_t canonicalGlyphGroups[CANONICAL_GLYPH_GROUPS_SIZE][2] PROGMEM = {")
    for latin, cyrillic in pairs:
        print(f"    {{{to_hex(latin)}, {to_hex(cyrillic)}}},")
    print("};")

if __name__ == "__main__":
    # Пример: пары (латиница, кириллица)
    pairs = [
        ('A', 'А'),
        ('B', 'В'),
        ('C', 'С'),
        ('E', 'Е'),
        ('H', 'Н'),
        ('K', 'К'),
        ('M', 'М'),
        ('O', 'О'),
        ('P', 'Р'),
        ('T', 'Т'),
        ('X', 'Х'),
        ('a', 'а'),
        ('c', 'с'),
        ('e', 'е'),
        ('k', 'к'),
        ('m', 'м'),
        ('o', 'о'),
        ('p', 'р'),
        ('x', 'х'),
        ('y', 'у'),
        ('3', 'З'),
    ]
    generate_groups(pairs)