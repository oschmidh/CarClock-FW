#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_FONT_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_FONT_HPP

#include "Primitives.hpp"

#include <algorithm>

template <char BEGIN_V, char END_V, int WIDTH_V, int HEIGHT_V, int COLOR_DEPTH_V = 1>
struct FontDefinition {
    static constexpr char firstChar = BEGIN_V;
    static constexpr char lastChar = END_V;
    static constexpr int width = WIDTH_V;
    static constexpr int height = HEIGHT_V;
    const int kerning;
    const std::array<Bitmap<width, height, COLOR_DEPTH_V>, lastChar - firstChar + 1> chars;

    constexpr const Bitmap<width, height, COLOR_DEPTH_V>& getBitmap(char c) const noexcept
    {
        // TODO ??
        // __ASSERT(c >= firstChar && c <= lastChar);

        // printk("getBitmap(%c), firstChar: %c, lastChar: %c\n", c, firstChar, lastChar);

        // TODO test
        c = std::clamp(c, firstChar, lastChar);    // TEST
        // printk("clamped char: %c\n", c);

        return chars[c - firstChar];
    }
};

// template <char BEGIN_V, char END_V, unsigned int WIDTH_V, unsigned int HEIGHT_V, unsigned int COLOR_DEPTH_V = 1>
// struct FontDefinition {
//     static constexpr char firstChar = BEGIN_V;
//     static constexpr char lastChar = END_V;
//     static constexpr unsigned int width = WIDTH_V;
//     static constexpr unsigned int height = HEIGHT_V;
//     const unsigned int kerning;
//     const std::array<Bitmap<width, height, COLOR_DEPTH_V>, lastChar - firstChar + 1> chars;

//     template <unsigned int WIDTH_V, unsigned int HEIGHT_V, unsigned int COLOR_DEPTH_V = 1>
//     struct Bitmap {
//         static constexpr unsigned int width = WIDTH_V;
//         static constexpr unsigned int height = HEIGHT_V;
//         static constexpr std::size_t size() noexcept { return height * ((width * COLOR_DEPTH_V + 7) / 8); }
//         std::array<std::uint8_t, size()> data;    // round up
//     };

//     constexpr const Bitmap<width, height, COLOR_DEPTH_V>& getBitmap(char c) const noexcept
//     {
//         // TODO ??
//         // __ASSERT(c >= firstChar && c <= lastChar);

//         // printk("getBitmap(%c), firstChar: %c, lastChar: %c\n", c, firstChar, lastChar);

//         // TODO test
//         c = std::clamp(c, firstChar, lastChar);    // TEST
//         // printk("clamped char: %c\n", c);

//         return chars[c - firstChar];
//     }
// };

// struct Font {
//     char firstChar;
//     char lastChar;
//     unsigned int width;
//     unsigned int height;
//     unsigned int kerning;

//     template <char BEGIN_V, char END_V, unsigned int WIDTH_V, unsigned int HEIGHT_V, unsigned int COLOR_DEPTH_V = 1>
//     constexpr Font(const FontDefinition<BEGIN_V, END_V, WIDTH_V, HEIGHT_V, COLOR_DEPTH_V>& fontDef) noexcept
//      : firstChar(fontDef.firstChar)
//      , lastChar(fontDef.lastChar)
//      , width(fontDef.width)
//      , height(fontDef.height)
//      , kerning(fontDef.kerning)
//      //  , chars(fontDef.chars.data(), fontDef.chars.size())
//      , chars(fontDef.chars.data(), fontDef.chars.size(), sizeof(fontDef.chars[0]))
//     { }

//     constexpr Font(const Font& other) noexcept
//      : firstChar(other.firstChar)
//      , lastChar(other.lastChar)
//      , width(other.width)
//      , height(other.height)
//      , kerning(other.kerning)
//      , chars(other.chars)
//     { }

//     constexpr Font& operator=(const Font& other) noexcept
//     {
//         firstChar = other.firstChar;
//         lastChar = other.lastChar;
//         width = other.width;
//         height = other.height;
//         kerning = other.kerning;
//         chars = other.chars;
//         return *this;
//     }

//     // const std::span<BitmapView> chars;

//     Kokkos::mdspan<const std::uint8_t, Kokkos::dextents<unsigned int, 2>> chars;

//     constexpr BitmapView getBitmap(char c) const noexcept
//     {
//         c = std::clamp(c, firstChar, lastChar);
//         // return chars[c - firstChar, 0];
//         // return {.width = width, .height = height, .data = chars[c - firstChar, 0]};
//         BitmapView ret{};
//         ret.width = width;
//         ret.height = height;
//         // ret.data = Kokkos::mdspan<const std::uint8_t, Kokkos::dextents<unsigned int, 2>>{&chars[c - firstChar, 0],
//         // width, height};
//         ret.data = Kokkos::mdspan{&chars[c - firstChar, 0], height, (width * 4 + 7) / 8};    // TODO ugly?
//         return ret;
//     }
// };

struct Font {
    char firstChar;
    char lastChar;
    int width;
    int height;
    int kerning;

    template <char BEGIN_V, char END_V, int WIDTH_V, int HEIGHT_V, int COLOR_DEPTH_V = 1>
    constexpr Font(const FontDefinition<BEGIN_V, END_V, WIDTH_V, HEIGHT_V, COLOR_DEPTH_V>& fontDef) noexcept
     : firstChar(fontDef.firstChar)
     , lastChar(fontDef.lastChar)
     , width(fontDef.width)
     , height(fontDef.height)
     , kerning(fontDef.kerning)
     , chars(fontDef.chars[0].data.data(), fontDef.chars.size() * fontDef.chars[0].data.size())    // TODO
                                                                                                   // ugly
                                                                                                   // af
    { }

    constexpr Font(const Font& other) noexcept
     : firstChar(other.firstChar)
     , lastChar(other.lastChar)
     , width(other.width)
     , height(other.height)
     , kerning(other.kerning)
     , chars(other.chars)
    { }

    constexpr Font& operator=(const Font& other) noexcept
    {
        firstChar = other.firstChar;
        lastChar = other.lastChar;
        width = other.width;
        height = other.height;
        kerning = other.kerning;
        chars = other.chars;
        return *this;
    }

    // const std::span<BitmapView> chars;

    std::span<const std::uint8_t> chars;

    constexpr BitmapView getBitmap(char c) const noexcept
    {
        c = std::clamp(c, firstChar, lastChar);
        // return chars[c - firstChar, 0];
        // return {.width = width, .height = height, .data = chars[c - firstChar, 0]};
        // BitmapView ret{};
        // ret.width = width;
        // ret.height = height;
        // // ret.data = Kokkos::mdspan<const std::uint8_t, Kokkos::dextents<unsigned int, 2>>{&chars[c - firstChar, 0],
        // // width, height};
        // ret.data = Kokkos::mdspan{&chars[c - firstChar, 0], height, (width * 4 + 7) / 8};    // TODO ugly?
        // return ret;

        // printk("idx: %d\n", (c - firstChar) * height * ((width * 4 + 7) / 8));
        // printk("c: %d\n", c);
        // printk("firstChar: %d\n", firstChar);
        // printk("height: %d\n", height);
        // printk("width: %d\n", width);

        // const auto subspan = chars.subspan((c - firstChar) * height * (width * 4 + 7) / 8);

        // printk("raw span:");
        // for (unsigned int i = 0; i < 15; ++i) {
        //     printk(" 0x%02x", subspan[i]);
        // }

        return BitmapView{chars.subspan((c - firstChar) * height * ((width * 4 + 7) / 8)), width,
                          height};    // TODO color depth hardcoded?
    }
};

// struct BitmapView {
//     template <unsigned int WIDTH_V, unsigned int HEIGHT_V, unsigned int COLOR_DEPTH_V>
//     constexpr BitmapView(const Bitmap<WIDTH_V, HEIGHT_V, COLOR_DEPTH_V>& bmp) noexcept
//      : width(bmp.width)
//      , height(bmp.height)
//      , data(bmp.data.data(), height, (width * COLOR_DEPTH_V + 7) / 8)    // TODO how to ensure that
//                                                                          // the size matches the
//                                                                          // array?
//     { }
//     unsigned int width;
//     unsigned int height;
//     Kokkos::mdspan<const std::uint8_t, Kokkos::dextents<unsigned int, 2>> data;
// };

namespace Fonts {

// static constexpr FontDefinition<'/', ':', 12, 21> CyberNumbers{
//     .kerning = 2,
//     // .chars = {{.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}},
//     //           {.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}},
//     //           {.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}},
//     //           {.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}},
//     //           {.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}},
//     //           {.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}},
//     //           {.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}},
//     //           {.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}},
//     //           {.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}},
//     //           {.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}},
//     //           {.data = {0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc}}}};
//     .chars = {
//         {{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
//          {{0xfc, 0xfe, 0xff, 0x7f, 0xf7, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc, 0xff, 0xff, 0xff, 0x00, 0x10,
//          0x0f,
//            0xe7, 0xf0, 0xc0, 0xff, 0xff, 0xff, 0x07, 0x0f, 0x1f, 0x1c, 0x1c, 0x1c, 0x1c, 0x1d, 0x1f, 0x1f, 0x0f,
//            0x07}},
//          {{0xfc, 0xfe, 0xff, 0x7f, 0x77, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc,
//            0xff, 0xff, 0xff, 0x00, 0x10, 0x0f, 0xe7, 0xf0, 0xc0, 0xff, 0xff, 0xff}},
//          {{0xfc, 0xfe, 0xff, 0x7f, 0x77, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc,
//            0xff, 0xff, 0xff, 0x00, 0x10, 0x0f, 0xe7, 0xf0, 0xc0, 0xff, 0xff, 0xff}},
//          {{0xfc, 0xfe, 0xff, 0x7f, 0x77, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc,
//            0xff, 0xff, 0xff, 0x00, 0x10, 0x0f, 0xe7, 0xf0, 0xc0, 0xff, 0xff, 0xff}},
//          {{0xfc, 0xfe, 0xff, 0x7f, 0x77, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc,
//            0xff, 0xff, 0xff, 0x00, 0x10, 0x0f, 0xe7, 0xf0, 0xc0, 0xff, 0xff, 0xff}},
//          {{0xfc, 0xfe, 0xff, 0x7f, 0x77, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc,
//            0xff, 0xff, 0xff, 0x00, 0x10, 0x0f, 0xe7, 0xf0, 0xc0, 0xff, 0xff, 0xff}},
//          {{0xfc, 0xfe, 0xff, 0x7f, 0x77, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc,
//            0xff, 0xff, 0xff, 0x00, 0x10, 0x0f, 0xe7, 0xf0, 0xc0, 0xff, 0xff, 0xff}},
//          {{0xfc, 0xfe, 0xff, 0x7f, 0x77, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc,
//            0xff, 0xff, 0xff, 0x00, 0x10, 0x0f, 0xe7, 0xf0, 0xc0, 0xff, 0xff, 0xff}},
//          {{0xfc, 0xfe, 0xff, 0x7f, 0x77, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc,
//            0xff, 0xff, 0xff, 0x00, 0x10, 0x0f, 0xe7, 0xf0, 0xc0, 0xff, 0xff, 0xff}},
//          {{0xfc, 0xfe, 0xff, 0x7f, 0x77, 0xc7, 0x07, 0x07, 0x07, 0xff, 0xfe, 0xfc,
//            0xff, 0xff, 0xff, 0x00, 0x10, 0x0f, 0xe7, 0xf0, 0xc0, 0xff, 0xff, 0xff}},
//          {{0xc0, 0x71, 0x00, 0xc0, 0x71, 0x00, 0xc0, 0x71, 0x00, 0x00, 0x00, 0x00,
//            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}}};

// static constexpr FontDefinition<' ', 'z', 3, 7> MiniFont{.kerning = 1,
//                                                          .chars = {{
//                                                              {{0x00, 0x00, 0x00}},    // ' '
//                                                              {{0xff, 0xff, 0xff}},    // '!'
//                                                              {{0xff, 0xff, 0xff}},    // '"'
//                                                              {{0xff, 0xff, 0xff}},    // '#'
//                                                              {{0xff, 0xff, 0xff}},    // '$'
//                                                              {{0xff, 0xff, 0xff}},    // '%'
//                                                              {{0xff, 0xff, 0xff}},    // '&'
//                                                              {{0xff, 0xff, 0xff}},    // '''
//                                                              {{0x0e, 0x11, 0x00}},    // '('
//                                                              {{0x00, 0x11, 0x0e}},    // ')'
//                                                              {{0xff, 0xff, 0xff}},    // '*'
//                                                              {{0xff, 0xff, 0xff}},    // '+'
//                                                              {{0xff, 0xff, 0xff}},    // ','
//                                                              {{0x04, 0x04, 0x04}},    // '-'
//                                                              {{0x00, 0x10, 0x00}},    // '.'
//                                                              {{0x18, 0x04, 0x03}},    // '/'

//                                                              {{0x0e, 0x11, 0x0e}},    // '0'
//                                                              {{0x12, 0x1f, 0x10}},    // '1'
//                                                              {{0x19, 0x15, 0x12}},    // '2'
//                                                              {{0x11, 0x15, 0x0a}},    // '3'
//                                                              {{0x07, 0x04, 0x1f}},    // '4'
//                                                              {{0x17, 0x15, 0x09}},    // '5'
//                                                              {{0x1f, 0x15, 0x1d}},    // '6'
//                                                              {{0x19, 0x05, 0x03}},    // '7'
//                                                              {{0x1f, 0x15, 0x1f}},    // '8'
//                                                              {{0x17, 0x15, 0x1f}},    // '9'

//                                                              {{0x00, 0x0a, 0x00}},    // ':'
//                                                              {{0x20, 0x14, 0x00}},    // ';'
//                                                              {{0x00, 0x08, 0x14}},    // '<'
//                                                              {{0xff, 0xff, 0xff}},    // '='
//                                                              {{0x14, 0x08, 0x00}},    // '>'
//                                                              {{0x01, 0x15, 0x02}},    // '?'
//                                                              {{0xff, 0xff, 0xff}},    // '@'

//                                                              {{0x1e, 0x05, 0x1e}},    // 'A'
//                                                              {{0x1f, 0x15, 0x0a}},
//                                                              {{0x0e, 0x11, 0x11}},
//                                                              {{0x1f, 0x11, 0x0e}},
//                                                              {{0x1f, 0x15, 0x11}},    // 'E'
//                                                              {{0x1f, 0x05, 0x01}},    // 'F'
//                                                              {{0x0e, 0x11, 0x19}},
//                                                              {{0x1f, 0x04, 0x1f}},
//                                                              {{0x11, 0x1f, 0x11}},
//                                                              {{0x13, 0x11, 0x0f}},
//                                                              {{0x1f, 0x04, 0x1b}},    // 'K'
//                                                              {{0x1f, 0x10, 0x10}},
//                                                              {{0x1f, 0x06, 0x1f}},    // 'M'
//                                                              {{0x1f, 0x04, 0x1f}},
//                                                              {{0x0e, 0x11, 0x0e}},    // 'O'
//                                                              {{0x1f, 0x05, 0x07}},
//                                                              {{0x0e, 0x19, 0x16}},
//                                                              {{0x1f, 0x0d, 0x17}},    // 'R'
//                                                              {{0x12, 0x15, 0x09}},    // 'S'
//                                                              {{0x01, 0x1f, 0x01}},
//                                                              {{0x0f, 0x10, 0x1f}},
//                                                              {{0x0f, 0x10, 0x0f}},
//                                                              {{0x1f, 0x0c, 0x1f}},    // 'W'
//                                                              {{0x1b, 0x04, 0x1b}},    // 'X'
//                                                              {{0x03, 0x1c, 0x03}},    // 'Y'
//                                                              {{0x19, 0x15, 0x13}},    // 'Z'

//                                                              {{0x1f, 0x11, 0x00}},    // '['
//                                                              {{0x03, 0x04, 0x18}},    // '\'
//                                                              {{0x00, 0x11, 0x1f}},    // ']'
//                                                              {{0x02, 0x01, 0x02}},    // '^'
//                                                              {{0x20, 0x20, 0x20}},    // '_'
//                                                              {{0x01, 0x02, 0x00}},    // '`'

//                                                              {{0x08, 0x14, 0x1c}},    // 'a'
//                                                              {{0x1f, 0x14, 0x08}},
//                                                              {{0x08, 0x14, 0x14}},
//                                                              {{0x08, 0x14, 0x1f}},
//                                                              //  {{0x0c, 0x16, 0x16}},    // 'e'
//                                                              {{0x08, 0x1c, 0x1c}},    // 'e'
//                                                              {{0x1e, 0x09, 0x02}},
//                                                              {{0x08, 0x54, 0x3c}},    // 'g'
//                                                              {{0x1f, 0x04, 0x18}},    // 'h'
//                                                              {{0x14, 0x1d, 0x10}},    // 'i'
//                                                              {{0x24, 0x1d, 0x00}},    // 'j'
//                                                              {{0x1f, 0x08, 0x14}},
//                                                              {{0x11, 0x1f, 0x10}},    // 'l'
//                                                              {{0x1c, 0x1c, 0x18}},
//                                                              {{0x1c, 0x04, 0x18}},
//                                                              {{0x08, 0x14, 0x08}},    // 'o'
//                                                              {{0x7c, 0x14, 0x08}},
//                                                              {{0x08, 0x14, 0x7c}},
//                                                              {{0x1c, 0x04, 0x04}},
//                                                              //  {{0x14, 0x1e, 0x0a}},    // 's'
//                                                              {{0x18, 0x1c, 0x0c}},    // 's'
//                                                              {{0x02, 0x0f, 0x12}},    // 't'
//                                                              {{0x0c, 0x10, 0x1c}},    // 'u'
//                                                              {{0x0c, 0x10, 0x0c}},
//                                                              {{0x1c, 0x18, 0x1c}},    // 'w'
//                                                              {{0x14, 0x08, 0x14}},    // 'x'
//                                                              {{0x24, 0x28, 0x1c}},    // 'y'
//                                                              // {{0x0a, 0x1e, 0x14}}, // 'z'
//                                                              {{0x14, 0x1c, 0x14}},    // 'z'
//                                                          }}};

}    // namespace Fonts

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_FONT_HPP
