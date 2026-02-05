from PIL import Image
from pathlib import Path
import re


# with open(, "rb") as file:
img = Image.open("CarClock-FW/resources/fonts/timeFont.bmp")


start_char = "/"
end_char = ":"
kerning = 2
color_depth_bits = 4

# horizontal address mode:
# for pixel in list(img.getdata()):

# print(img.info)

raw = list(img.getdata())


def invert(x):
    if x:
        return 0
    else:
        return 1


# raw = [invert(x) for x in raw]  # invert
# print(raw)


def merge(a, b):
    a = a * 15
    b = b * 15
    return a * 16 + b
    # ret = a // 16 * 16 + b // 16
    # print(f"merging {a} and {b} (={hex(ret)})")
    # return ret


num_chars = ord(end_char) - ord(start_char) + 1
font_width = (img.width - (num_chars - 1)) // num_chars
font_height = img.height

# print(f"num_chars: {num_chars}")
# print(f"font width: {font_width}")
# print(f"font height: {font_height}")
# print(f"img width: {img.width}")

char_defs = []
# print(len(raw))

for i in range(num_chars):
    char_def = []
    x_offset = i * (font_width + 1)
    for y in range(font_height):
        # print(y)
        line_offset = x_offset + img.width * y
        # print(f"x_offset: {x_offset}")
        # print(f"line_offset: {line_offset}")
        for x in range(line_offset, line_offset + font_width):
            # print(x)
            char_def.append(raw[x])
        if font_width % 2:
            # print(f"font not evenly divisible")
            # need to have an evenly divisible amount on each line, because always 2 are merged together later
            char_def.append(0)

    merged = [merge(x[0], x[1]) for x in zip(char_def[::2], char_def[1::2])]
    char_defs.append(merged)


# merged = [merge(x[0], x[1]) for x in zip(raw[3::8], raw[7::8])]


image_name = Path(img.filename).stem
output_filename = f"{image_name}.hpp"

with open(output_filename, "w") as file:
    include_guard_str = re.sub(r"[^\w\s]", "_", output_filename.upper())
    file.write(f"#ifndef {include_guard_str}\n")
    file.write(f"#define {include_guard_str}\n\n")

    file.write('#include "Font.hpp"\n\n')

    file.write(
        f"static constexpr FontDefinition<'{start_char}','{end_char}',{font_width}, {font_height}, {color_depth_bits}> {image_name} {{\n"
    )
    file.write(f".kerning = {kerning},\n")
    file.write(".chars = {{")

    file.write("{")
    file.write(hex(char_defs[0][0]))
    for i in char_defs[0][1:]:
        file.write(f", {hex(i)}")
    file.write("}")
    for char in char_defs[1:]:
        file.write(", {")
        file.write(hex(char[0]))
        for i in char[1:]:
            file.write(f", {hex(i)}")
        file.write("}")
    file.write(" }}};\n\n")

    file.write(f"#endif // {include_guard_str}\n")


# print(f"static constexpr Bitmap<{img.width}, {img.height}> {img.filename} {{ ", end="")
# print(hex(merged[0]), end="")
# for i in merged[1:]:
#     print(f", {hex(i)}", end="")
# print(" };")
