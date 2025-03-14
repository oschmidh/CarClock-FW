#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP

#include "Primitives.hpp"

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/devicetree.h>

#include <mdspan/mdspan.hpp>
#include <mdspan/mdarray.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>

#define DT_DISPLAY_FRAMEBUF_DEFINE(name, nodeId) \
    static FrameBufferType<DT_PROP(nodeId, width), DT_PROP(nodeId, height)> name { }

// template <std::size_t WIDTH_V, std::size_t HEIGHT_V>
// class FrameBuffer {
//   public:
//     std::uint8_t& operator[](std::size_t i) noexcept { return _buf[i]; }
//     std::uint8_t* data() noexcept { return _buf.data(); }

//     constexpr std::size_t size() const noexcept { return _buf.size(); }

//   private:
//     static_assert(!(WIDTH_V % 8));
//     // std::array<std::uint8_t, WIDTH_V * HEIGHT_V * 4 / 8> _buf{};
//     using Extents = Kokkos::extents<unsigned int, HEIGHT_V, WIDTH_V * 4 / 8>;    // TODO template for color_depth
//     Kokkos::Experimental::mdarray<std::uint32_t, Extents> _buf{};
// };

// template <std::size_t WIDTH_V, std::size_t HEIGHT_V>
// struct FrameBuffer {
//     static_assert(!(WIDTH_V % 8));
//     using Extents = Kokkos::extents<unsigned int, HEIGHT_V, WIDTH_V * 4 / 8>;    // TODO template for color_depth
//     Kokkos::Experimental::mdarray<std::uint32_t, Extents> _buf{};
// };

template <std::size_t WIDTH_V, std::size_t HEIGHT_V>
    requires(!(WIDTH_V * 4 % 8))    // TODO template for color_depth
using FrameBufferType =
    Kokkos::Experimental::mdarray<std::uint8_t,
                                  Kokkos::extents<unsigned int, HEIGHT_V, WIDTH_V * 4 / 8>>;    // TODO template for
                                                                                                // color_depth

template <std::size_t WIDTH_V, std::size_t HEIGHT_V>
class Display {
  public:
    static constexpr std::size_t width = WIDTH_V;
    static constexpr std::size_t height = HEIGHT_V;

    Display(const device* const dev, FrameBufferType<WIDTH_V, HEIGHT_V>& frameBuf) noexcept
     : _frameBuf(frameBuf)
     , _dev(dev)
    { }

    bool init() noexcept
    {
        if (!device_is_ready(_dev)) {
            printk("Display not ready\n");
            // LOG_ERR("Display not ready");
            return false;
        }

        if (display_set_pixel_format(_dev, PIXEL_FORMAT_MONO01) != 0) {
            // printk("Failed to set required pixel format\n");
            // LOG_ERR("Failed to set required pixel format");  // TODO
            return false;
        }

        // display_set_contrast(_dev, 0xff);

        // display_blanking_off(_dev);
        return true;
    }

    template <typename FONT_T>    // TODO get rid of template
    void draw(std::string_view text, Point pos, const FONT_T& font) noexcept
    {
        // printk("draw %s", text.data());

        for (char c : text) {
            if (c == '\0') {
                return;
            }

            draw(font.getBitmap(c), pos);
            pos.x += font.width + font.kerning;
        }
    }

    void draw(const Line& line, unsigned int thickness) noexcept
    {
        // TODO implement thickness

        // Bresenham-algorithm, https://de.wikipedia.org/wiki/Bresenham-Algorithmus
        Point p = line.begin;

        const int dx = std::abs(static_cast<int>(line.end.x - line.begin.x));
        const int dy = -std::abs(static_cast<int>(line.end.y - line.begin.y));
        const int sx = line.begin.x < line.end.x ? 1 : -1;
        const int sy = line.begin.y < line.end.y ? 1 : -1;
        int err = dx + dy;

        while (1) {
            draw(p);
            if (p == line.end) {
                return;
            }

            const int e2 = 2 * err;
            if (e2 > dy) {
                err += dy;
                p.x += sx;
            }

            if (e2 < dx) {
                err += dx;
                p.y += sy;
            }
        }
    }

    void draw(const Point& point) noexcept
    {
        const std::uint8_t mask = 1 << point.y % 8u;
        const unsigned int idx = point.y / 8 * width + point.x;
        _frameBuf[idx] |= mask;
    }

    void drawHLine(const Point& begin, unsigned int length, unsigned int thickness) noexcept
    {
        for (unsigned int i = 0; i < thickness; ++i) {
            drawHLine({begin.x, begin.y + static_cast<int>(i)}, length);    // TODO brute-force solution...
        }
    }

    void drawVLine(const Point& begin, unsigned int length, unsigned int thickness) noexcept
    {
        for (unsigned int i = 0; i < thickness; ++i) {
            drawVLine({begin.x + static_cast<int>(i), begin.y}, length);
        }
    }

    // template <typename T>
    // void draw(const T& bmp, std::size_t x, std::size_t y) noexcept requires {}

    // template <int W_V, int H_V>    // TODO remove template to avoid bloat
    // void draw(const Bitmap<W_V, H_V>& bmp, Point pos) noexcept
    // {
    //     // unsigned int height = H_V;
    //     int bmpIndex = 0;

    //     // for (unsigned int y = 0; y < H_V; ++y) {
    //     // int y = pos.y;
    //     const int yPos = pos.y / 8;
    //     int yLineOffset = 0;
    //     int yBitOffset = pos.y % 8u;
    //     const int bitAlignOffset = pos.y % 8u;

    //     // if (yBitOffset == 0) {
    //     while ((yLineOffset * 8 + yBitOffset) <= bmp.height + 1) {

    //         unsigned int fbIdx = (yPos + yLineOffset) * width + pos.x;
    //         const unsigned int maskHeight = std::min(bitAlignOffset + bmp.height - yLineOffset * 8, 8) - yBitOffset;
    //         // const unsigned int maskHeight = std::min(bmp.height - yLineOffset * 8, 8);
    //         const std::uint8_t mask = ((2 << (maskHeight - 1)) - 1) << yBitOffset;

    //         for (unsigned int x = 0; x < bmp.width; ++x) {
    //             std::uint8_t byte = bmp.data[bmpIndex] << yBitOffset;
    //             // if (bitAlignOffset != 0) {
    //             // byte << bitAlignOffset;
    //             // if (yLineOffset > 0) {
    //             if (yBitOffset > 0) {
    //                 // byte |= bmp.data[bmpIndex - 1] >> (8 - yBitOffset);
    //                 byte |= bmp.data[bmpIndex - bmp.width] >> (8 - yBitOffset);
    //             }
    //             // }
    //             // _frameBuf[fbIdx + x] |= bmp.data[bmpIndex++] & mask;

    //             _frameBuf[fbIdx + x] &= ~mask;
    //             _frameBuf[fbIdx + x] |= byte & mask;
    //             // ++fbIdx;
    //             ++bmpIndex;
    //         }
    //         bmpIndex -= 8;
    //         ++yLineOffset;
    //         yBitOffset = 0;

    //     }
    //     // } else { // bitmap position not aligned to 8bit boundary
    //     // }

    //     // for (std::uint8_t byte : bmp.data) {
    //     // }

    //     // while (height > 0) {
    //     //     _frameBuf[] |= bmp.data[] & mask;
    //     // }

    //     /*unsigned int pixelCnt = 0;
    //     const unsigned int offset = pos.y % 8u;
    //     // const unsigned int vBytes =

    //     for (int x = pos.x; x < pos.x + bmp.width; ++x) {
    //         const unsigned int idx = pos.y / 8 * width + x;
    //         unsigned int remHeight = bmp.height;

    //         if (x <= pos.x + 2) {
    //         }
    //         if (offset) {
    //             // std::uint8_t mask{};
    //             // for (unsigned int i = 0; i < std::min(remHeight, 8u); ++i) {
    //             //     mask |= 1 << (offset + i);
    //             // }
    //             const unsigned int h = std::min(remHeight, 8u);
    //             const unsigned int bmpIndex = pixelCnt / 8;

    //             const std::uint8_t mask = BIT_MASK(h) << offset;
    //             const std::uint8_t data = bmp.data[bmpIndex / 8] << offset;
    //             if (x <= pos.x + 2) {
    //             }
    //             _frameBuf[idx] &= ~mask;
    //             _frameBuf[idx] |= data & mask;
    //             // ++bmpIndex;
    //             // idx += width;
    //             // remHeight -= (h - offset); // TODO if offset > h?
    //             // remHeight = std::min(remHeight - (8 - offset));
    //             const unsigned int written = h - offset;
    //             pixelCnt += written;
    //             remHeight -= written;
    //         }
    //         while (remHeight >= 8) {
    //             // ++bmpIndex;
    //             const unsigned int bmpIndex = pixelCnt / 8;

    //             const std::uint8_t data =
    //                 offset ? (bmp.data[bmpIndex + 1] << offset) | (bmp.data[bmpIndex] >> (8u - offset))
    //                        : bmp.data[bmpIndex];
    //             if (x <= pos.x + 2) {
    //             }
    //             _frameBuf[idx] = data;
    //             // idx += width;
    //             remHeight -= 8;
    //             pixelCnt += 8;
    //         }

    //         // std::uint8_t mask{};

    //         const unsigned int bmpIndex = pixelCnt / 8;

    //         const std::uint8_t data = offset
    //                                       ? (bmp.data[bmpIndex + 1] << offset) | (bmp.data[bmpIndex] >> (8u -
    //     offset)) : bmp.data[bmpIndex];
    //         // offset ? (bmp.data[bmpIndex] << offset) : bmp.data[bmpIndex];
    //         const std::uint8_t mask = BIT_MASK(remHeight);
    //         if (x <= pos.x + 2) {
    //         }
    //         // for (unsigned int i = 0; i < remHeight; ++i) {
    //         //     mask |= 1 <<
    //         // }
    //         _frameBuf[idx] &= ~mask;
    //         _frameBuf[idx] |= data & mask;
    //         // ++bmpIndex;
    //         pixelCnt += remHeight;
    //         pixelCnt = (pixelCnt + 7) / 8 * 8;    // TODO ugly, round to nearest multiple of 8
    //         // pixelCnt += remHeight;
    //     }*/
    // }

    /*void drawVerticalAddressingSingleBitOled(BitmapView bmp, Point pos) noexcept
    {
        const int yPos = pos.y / 8;
        int yLineOffset = 0;
        const int yBitOffset = pos.y % 8u;
        const int bitAlignOffset = pos.y % 8u;

        unsigned int fbIdx = (yPos + yLineOffset) * width + pos.x;

        unsigned int remHeight = bmp.height;

        if (yBitOffset) {
            // bitmap y starting point not aligned to 8-bit boundary
            const unsigned int maskHeight = std::min(bitAlignOffset + bmp.height - yLineOffset * 8u, 8u) - yBitOffset;
            const std::uint8_t mask = ((2 << (maskHeight - 1)) - 1) << yBitOffset;

            for (unsigned int x = 0; x < bmp.width; ++x) {
                const std::uint8_t data = bmp.data[x] << yBitOffset;
                _frameBuf[fbIdx + x] &= ~mask;
                _frameBuf[fbIdx + x] |= data & mask;
            }
            fbIdx += width;
            remHeight -= maskHeight;
        }

        int bmpIndex = 0;
        while (remHeight >= 8) {
            for (unsigned int x = 0; x < bmp.width; ++x) {
                const std::uint8_t data = yBitOffset ? (bmp.data[bmpIndex + bmp.width] << yBitOffset) |
                                                           (bmp.data[bmpIndex] >> (8u - yBitOffset))
                                                     : bmp.data[bmpIndex];
                _frameBuf[fbIdx + x] = data;
                ++bmpIndex;
            }
            fbIdx += width;
            remHeight -= 8;
        }

        const std::uint8_t mask = BIT_MASK(remHeight);
        for (unsigned int x = 0; x < bmp.width; ++x) {
            std::uint8_t data{};
            if (yBitOffset) {
                data = (bmp.data[bmpIndex + bmp.width] << yBitOffset) | (bmp.data[bmpIndex] >> (8u - yBitOffset));
            } else {
                data = bmp.data[bmpIndex];
            }
            _frameBuf[fbIdx + x] &= ~mask;
            _frameBuf[fbIdx + x] |= data & mask;
            ++bmpIndex;
        }
    }*/

    void draw(BitmapView bmp, Point pos) noexcept
    {
        const unsigned int horizBitOffset = pos.x % 2;    // TODO 2 is 8/COLOR_DEPTH

        for (unsigned int y = 0; y < bmp.height; ++y) {
            const unsigned int hzBytes = (bmp.width + 1) / 2;    // NOTE div by 2, rounded up

            if (!horizBitOffset) {
                // fast algorithm for positions aligned to full byte boundaries
                std::copy_n(&bmp.data[y, 0], hzBytes, &_frameBuf[pos.y + y, pos.x / 2]);
            } else {
                // slow algorithm for unaligned positions
                _frameBuf[pos.y + y, pos.x / 2] &= 0xf;
                _frameBuf[pos.y + y, pos.x / 2] |= (bmp.data[y, 0] & 0xf0) >> 4u;

                for (unsigned int x = 1; x < hzBytes; ++x) {
                    // nibble swap:
                    const std::uint8_t merged = ((bmp.data[y, x - 1] & 0xf) << 4u) | ((bmp.data[y, x] & 0xf0) >> 4u);

                    _frameBuf[pos.y + y, pos.x / 2 + x] = merged;
                }
                _frameBuf[pos.y + y, pos.x / 2 + hzBytes] &= 0xf0;
                _frameBuf[pos.y + y, pos.x / 2 + hzBytes] |= (bmp.data[y, hzBytes - 1] & 0xf) << 4u;
            }
        }

        // // fast algorithm for positions aligned to full byte boundaries
        // for (unsigned int y = 0; y < bmp.height; ++y) {
        //     const unsigned int horizBytes = (bmp.width + 1) / 2;            // NOTE div by 2, rounded up
        //     const unsigned int fbOffset = (pos.y * WIDTH_V + pos.x) / 2;    // TODO handle odd x values
        //     std::copy_n(&bmp.data[y * horizBytes], horizBytes, &_frameBuf[fbOffset + y * WIDTH_V / 2]);
        // }
    }

    // void draw(BitmapView bmp, Point pos) noexcept { std::copy(bmp.data.begin(), bmp.data.end(), _frameBuf.data()); }

    void invert(const Rectangle& area) noexcept
    {
        for (std::size_t x = area.begin.x; x < area.begin.x + area.width; ++x) {
            // TODO almost same as drawVLine, extract common part:
            unsigned int idx = area.begin.y / 8 * width + x;
            unsigned int height = area.height;
            const unsigned int bit = area.begin.y % 8u;

            if (bit != 0) {
                // std::uint8_t mask{};
                // for (unsigned int i = 0; i < std::min(height, 8u); ++i) {
                //     mask |= 1 << (bit + i);
                // }
                const unsigned int h = std::min(height, 8u);
                const std::uint8_t mask = BIT_MASK(h) << bit;
                _frameBuf[idx] ^= mask;
                idx += width;
                height -= (h - bit);
            }

            while (height >= 8) {
                _frameBuf[idx] ^= 0xff;
                idx += width;
                height -= 8;
            }

            // std::uint8_t mask{};
            const std::uint8_t mask = BIT_MASK(height);
            // for (unsigned int i = 0; i < height; ++i) {
            //     mask |= 1 << i;
            // }
            _frameBuf[idx] ^= mask;
        }
    }

    void update() noexcept
    {
        // TODO split the framebuffer into tiles and only update the modified tiles

        const display_buffer_descriptor desc{
            .buf_size = _frameBuf.size(), .width = width, .height = height, .pitch = width};
        if (const auto err = display_write(_dev, 0, 0, &desc, _frameBuf.data()); err < 0) {
            printk("failed to write, %d\n", err);
        }
    }

  private:
    void drawHLine(const Point& begin, unsigned int length) noexcept
    {    // TODO only valid in horizontal addressing mode?
        const unsigned int indexOffset = begin.y / 8 * width;

        const std::uint8_t mask = 1 << (begin.y % 8);    // TODO could implement thickness here?

        for (unsigned int i = 0; i < length; ++i) {
            _frameBuf[indexOffset + begin.x + i] |= mask;
        }
    }

    void drawVLine(const Point& begin, unsigned int length) noexcept
    {    // TODO only valid in horizontal addressing mode?
        unsigned int idx = begin.y / 8 * width + begin.x;
        const unsigned int bit = begin.y % 8u;    // TODO assumes that width % 8 == 0; ?

        if (bit != 0) {
            const unsigned int maskHeight = std::min(length, 8u - bit);
            const std::uint8_t mask = BIT_MASK(maskHeight) << bit;
            _frameBuf[idx] |= mask;
            idx += width;
            length -= maskHeight;
        }

        while (length >= 8) {
            _frameBuf[idx] |= 0xff;
            idx += width;
            length -= 8;
        }

        const std::uint8_t mask = BIT_MASK(length);
        _frameBuf[idx] |= mask;
    }

    // std::array<std::uint8_t, width * height / 8> _frameBuf{};    // TODO verify that width *height is evenly
    // divisible
    //                                                               // by 8
    // TODO dont put framebuffer onto the stack?
    FrameBufferType<width, height>& _frameBuf;
    const device* const _dev;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP
