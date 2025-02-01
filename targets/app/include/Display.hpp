#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP

#include "Primitives.hpp"

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/devicetree.h>

#include <algorithm>
#include <cmath>
#include <cstdint>

#define DT_DISPLAY_FRAMEBUF_DEFINE(name, nodeId) \
    static FrameBuffer<DT_PROP(nodeId, width), DT_PROP(nodeId, height)> name { }

template <std::size_t WIDTH_V, std::size_t HEIGHT_V>
class FrameBuffer {
  public:
    std::uint8_t& operator[](std::size_t i) noexcept { return _buf[i]; }
    std::uint8_t* data() noexcept { return _buf.data(); }
    constexpr std::size_t size() const noexcept { return _buf.size(); }

  private:
    static_assert(!(HEIGHT_V % 8));
    std::array<std::uint8_t, WIDTH_V * HEIGHT_V / 8> _buf{};
};

template <std::size_t WIDTH_V, std::size_t HEIGHT_V>
class Display {
  public:
    static constexpr std::size_t width = WIDTH_V;
    static constexpr std::size_t height = HEIGHT_V;

    Display(const device* const dev, FrameBuffer<WIDTH_V, HEIGHT_V>& frameBuf) noexcept
     : _frameBuf(frameBuf)
     , _dev(dev)
    { }

    bool init() noexcept
    {
        if (!device_is_ready(_dev)) {
            // LOG_ERR("Display not ready");
            return false;
        }

        if (display_set_pixel_format(_dev, PIXEL_FORMAT_MONO01) != 0) {
            // LOG_ERR("Failed to set required pixel format");  // TODO
            return false;
        }

        display_blanking_off(_dev);
        return true;
    }

    template <typename FONT_T>    // TODO get rid of template
    void draw(std::string_view text, Point pos, const FONT_T& font) noexcept
    {
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

        const int dx = std::abs(line.end.x - line.begin.x);
        const int dy = -std::abs(line.end.y - line.begin.y);
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

    template <int W_V, int H_V>    // TODO remove template to avoid bloat
    void draw(const Bitmap<W_V, H_V>& bmp, Point pos) noexcept
    {
        const int yPos = pos.y / 8;
        int yLineOffset = 0;
        const int yBitOffset = pos.y % 8u;
        const int bitAlignOffset = pos.y % 8u;

        unsigned int fbIdx = (yPos + yLineOffset) * width + pos.x;

        unsigned int remHeight = bmp.height;

        if (yBitOffset) {
            // bitmap y starting point not aligned to 8-bit boundary
            const unsigned int maskHeight = std::min(bitAlignOffset + bmp.height - yLineOffset * 8, 8) - yBitOffset;
            printk("maskHeight: %d\n", maskHeight);
            printk("fbIdx: %d\n", fbIdx);
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
                printk("_frameBuf[%d] |= 0x%02x\n", fbIdx + x, data);
                _frameBuf[fbIdx + x] = data;
                ++bmpIndex;
            }
            fbIdx += width;
            remHeight -= 8;
        }

        const std::uint8_t mask = BIT_MASK(remHeight);
        printk("fbIdx: %d\n", fbIdx);
        for (unsigned int x = 0; x < bmp.width; ++x) {
            std::uint8_t data{};
            if (yBitOffset) {
                data = (bmp.data[bmpIndex + bmp.width] << yBitOffset) | (bmp.data[bmpIndex] >> (8u - yBitOffset));
                printk("data = (0x%02x << %d) | (0x%02x >> %d)\n", bmp.data[bmpIndex + bmp.width], yBitOffset,
                       bmp.data[bmpIndex], 8u - yBitOffset);
            } else {
                data = bmp.data[bmpIndex];
                printk("data = 0x%02x\n", bmp.data[bmpIndex]);
            }
            _frameBuf[fbIdx + x] &= ~mask;
            _frameBuf[fbIdx + x] |= data & mask;
            ++bmpIndex;
        }
    }

    void invert(const Rectangle& area) noexcept
    {
        for (std::size_t x = area.begin.x; x < area.begin.x + area.width; ++x) {
            // TODO almost same as drawVLine, extract common part:
            unsigned int idx = area.begin.y / 8 * width + x;
            unsigned int height = area.height;
            const unsigned int bit = area.begin.y % 8u;

            // printk("inverting line\n-------------\n");
            // printk("height: %d\n", height);

            if (bit != 0) {
                // std::uint8_t mask{};
                // for (unsigned int i = 0; i < std::min(height, 8u); ++i) {
                //     mask |= 1 << (bit + i);
                // }
                const unsigned int h = std::min(height, 8u);
                const std::uint8_t mask = BIT_MASK(h) << bit;
                // printk("first mask: 0x%02x\n", mask);
                // printk("h: %d\n", h);
                _frameBuf[idx] ^= mask;
                idx += width;
                height -= (h - bit);
            }

            while (height >= 8) {
                // printk("mask: 0x%02x\n", 0xff);
                _frameBuf[idx] ^= 0xff;
                idx += width;
                height -= 8;
            }

            // std::uint8_t mask{};
            const std::uint8_t mask = BIT_MASK(height);
            // printk("last mask: 0x%02x\n\n", mask);
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
        display_write(_dev, 0, 0, &desc, _frameBuf.data());
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

        printk("drawVLine() x: %d, y: %d, len: %d\n", begin.x, begin.y, length);

        if (bit != 0) {
            const unsigned int maskHeight = std::min(length, 8u - bit);
            const std::uint8_t mask = BIT_MASK(maskHeight) << bit;
            _frameBuf[idx] |= mask;
            idx += width;
            length -= maskHeight;
            printk(" > drawing first %d bits\n", maskHeight);
        }

        while (length >= 8) {
            _frameBuf[idx] |= 0xff;
            idx += width;
            length -= 8;
            printk(" > drawing 8 bits\n");
        }

        const std::uint8_t mask = BIT_MASK(length);
        _frameBuf[idx] |= mask;
        printk(" > drawing last %d bits\n", length);
    }

    // std::array<std::uint8_t, width * height / 8> _frameBuf{};    // TODO verify that width *height is evenly
    // divisible
    //                                                               // by 8
    // TODO dont put framebuffer onto the stack?
    FrameBuffer<width, height>& _frameBuf;
    const device* const _dev;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP
