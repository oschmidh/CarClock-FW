def img_to_arr(image):  # TODO add colordepth param
    raw = list(image.getdata())

    print(raw)

    if max(raw) > 15:
        multiplier = 1  # TODO better name
        divisor = 16  # TODO better name
    elif max(raw) > 15:
        multiplier = 1
        divisor = 1
    else:
        multiplier = 15
        divisor = 1

    raw = [(x * multiplier) // divisor for x in raw]

    print(raw)

    return raw

    # def invert(x):
    #     if x:
    #         return 0
    #     else:
    #         return 1

    # # raw = [invert(x) for x in raw]  # invert
    # # print(raw)

    # def merge(a, b):
    #     a = a * multiplier
    #     b = b * multiplier
    #     # return a * 16 + b
    #     return a // divisor * 16 + b // divisor
    #     # print(f"merging {a} and {b} (={hex(ret)})")
    #     # return ret


def merge(a, b):
    return a * 16 + b
