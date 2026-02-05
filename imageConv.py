from PIL import Image
from pathlib import Path
import re


# with open(, "rb") as file:
img = Image.open("CarClock-FW/resources/images/homeScreenBackground.bmp")
image_name = Path(img.filename).stem
img = img.convert("L")

# img = img.transpose(Image.ROTATE_90)

color_depth_bits = 4

# horizontal address mode:
# for pixel in list(img.getdata()):

# print(img.getcolors())

print(img.info)

raw = list(img.getdata())
print(f"len{len(raw)}")
print(f"img.w*h{img.width * img.height}")
print(f"img.width{img.width }")
print(f"img.height{ img.height}")
# print(raw)

print(img.info["compression"])
# if img.info == 1:
raw = [i // 16 for i in raw]

# print(len(raw[3::4]))
# print(raw)
# pixel = raw[0]
# print(pixel)


def merge(a, b):
    return a * 16 + b
    # ret = a // 16 * 16 + b // 16
    # print(f"merging {a} and {b} (={hex(ret)})")
    # return ret


# merged = [merge(x[0], x[1]) for x in zip(raw[3::8], raw[7::8])]
merged = [merge(x[0], x[1]) for x in zip(raw[::2], raw[1::2])]

# image_name = Path(img.filename).stem
output_filename = f"{image_name}.hpp"

with open(output_filename, "w") as file:
    include_guard_str = re.sub(r"[^\w\s]", "_", output_filename.upper()) + "_HPP"
    file.write(f"#ifndef {include_guard_str}\n")
    file.write(f"#define {include_guard_str}\n\n")

    file.write('#include "Primitives.hpp"\n\n')

    file.write(
        f"static constexpr Bitmap<{img.width}, {img.height}, {color_depth_bits}> {image_name} {{ "
    )
    file.write(hex(merged[0]))
    for i in merged[1:]:
        file.write(f", {hex(i)}")
    file.write(" };\n\n")

    file.write(f"#endif // {include_guard_str}\n")


# print(f"static constexpr Bitmap<{img.width}, {img.height}> {img.filename} {{ ", end="")
# print(hex(merged[0]), end="")
# for i in merged[1:]:
#     print(f", {hex(i)}", end="")
# print(" };")
