#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_FRAMEBUFFER_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_FRAMEBUFFER_HPP

#include "Primitives.hpp"

#include <mdspan/mdspan.hpp>
#include <mdspan/mdarray.hpp>

#include <cstdint>

enum class Mode {    // TODO find better name
    Rgb,
    Rgba,
    Grayscale,
};

template <unsigned int WIDTH_V, unsigned int HEIGHT_V, unsigned int COLOR_DEPTH_BITS_V>
    requires(!(WIDTH_V * COLOR_DEPTH_BITS_V % 8))
class Framebuffer {
    using Extent = Kokkos::extents<unsigned int, HEIGHT_V, WIDTH_V * COLOR_DEPTH_BITS_V / 8>;
    using BufferType = Kokkos::Experimental::mdarray<std::uint8_t, Extent>;

    static constexpr unsigned int bitPerPixel = COLOR_DEPTH_BITS_V;
    static constexpr unsigned int pxPerByte = 8 / bitPerPixel;

  public:
    constexpr std::size_t size() const noexcept { return _buf.size(); }
    static constexpr std::size_t width() noexcept { return WIDTH_V; }
    static constexpr std::size_t height() noexcept { return HEIGHT_V; }
    const std::uint8_t* data() const noexcept { return _buf.data(); }

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
        const unsigned int idx = point.y / 8 * WIDTH_V + point.x;
        _buf[idx] |= mask;
    }

    void drawHLine(const Point& begin, unsigned int length, unsigned int thickness = 1) noexcept
    {
        for (unsigned int i = 0; i < thickness; ++i) {
            _drawHLine({begin.x, begin.y + i}, length);
        }
    }

    void drawVLine(const Point& begin, unsigned int length, unsigned int thickness = 1) noexcept
    {
        const unsigned int horizBitOffset = begin.x % pxPerByte;

        Point pos = {begin.x, begin.y};
        if (horizBitOffset) {
            _drawVLineMasked({pos.x, pos.y}, length, 0x0f);
            --thickness;
            ++pos.x;
        }

        for (unsigned int x = 1; x < thickness; x += pxPerByte) {
            _drawVLineMasked({pos.x + x, pos.y}, length, 0xff);
        }

        if (thickness % pxPerByte) {
            _drawVLineMasked({pos.x + thickness, pos.y}, length, 0xf0);
        }
    }

    void draw(const Rectangle& rect, bool fill) noexcept
    {
        // TODO fill not yet implemented
        drawHLine(rect.begin, rect.width);
        drawHLine({rect.begin.x, rect.begin.y + rect.height}, rect.width);
        drawVLine(rect.begin, rect.height);
        drawVLine({rect.begin.x + rect.width, rect.begin.y}, rect.height);
    }

    void draw(BitmapView bmp, Point pos) noexcept
    {
        const unsigned int horizBitOffset = pos.x % pxPerByte;

        for (unsigned int y = 0; y < bmp.height; ++y) {

            if (!horizBitOffset) {
                // fast algorithm for positions aligned to full byte boundaries
                const unsigned int hzBytes = bmp.width / pxPerByte;
                std::copy_n(&bmp.data[y, 0], hzBytes, &_buf[pos.y + y, pos.x / pxPerByte]);

                if (const auto rem = bmp.width - hzBytes * pxPerByte; rem > 0) {
                    _buf[pos.y + y, pos.x / pxPerByte + hzBytes] &= ~0xf0;
                    _buf[pos.y + y, pos.x / pxPerByte + hzBytes] |= bmp.data[y, hzBytes] & 0xf0;
                }
            } else {
                // slow algorithm for unaligned positions
                _buf[pos.y + y, pos.x / pxPerByte] &= ~0xf;
                _buf[pos.y + y, pos.x / pxPerByte] |= (bmp.data[y, 0] & 0xf0) >> 4u;

                const unsigned int hzBytes = (bmp.width - 1) / pxPerByte;
                for (unsigned int x = horizBitOffset; x < hzBytes + horizBitOffset; ++x) {
                    // nibble swap:
                    const std::uint8_t merged =
                        ((bmp.data[y, x - horizBitOffset] & 0xf) << 4u) | ((bmp.data[y, x] & 0xf0) >> 4u);

                    _buf[pos.y + y, pos.x / pxPerByte + x] = merged;
                }

                if (const auto rem = bmp.width - hzBytes * pxPerByte - horizBitOffset; rem > 0) {
                    _buf[pos.y + y, pos.x / pxPerByte + hzBytes + horizBitOffset] &= ~0xf0;
                    _buf[pos.y + y, pos.x / pxPerByte + hzBytes + horizBitOffset] |= bmp.data[y, hzBytes] << 4u;
                }
            }
        }
        /*
                // printk("\nfb:");
                // for (int n = 0; n < 3; ++n) {
                printk("char in fb at %d/%d:\n", pos.x, pos.y);
                for (int i = 0; i < 5; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        printk(" 0x%02x", _buf[pos.y + i, pos.x / 2 + j]);
                    }
                    printk("\n");
                }
                printk("\n");*/
        // }
    }

    void invert(const Rectangle& area) noexcept
    {
        const unsigned int horizBitOffset = area.begin.x % pxPerByte;

        for (unsigned int y = 0; y < area.height; ++y) {
            auto pos = area.begin + Point{0, y};

            if (horizBitOffset) {
                _buf[pos.y, pos.x / pxPerByte] ^= 0x0f;
                pos += Point{1, 0};
            }

            const unsigned int hzBytes = (area.begin.x + area.width - pos.x) / pxPerByte;
            const auto invert = [](std::uint8_t data) noexcept { return data ^ 0xff; };
            std::transform(&_buf[pos.y, pos.x / pxPerByte], &_buf[pos.y, pos.x / pxPerByte + hzBytes],
                           &_buf[pos.y, pos.x / pxPerByte], invert);
            pos += Point{hzBytes * pxPerByte, 0};

            if (const auto rem = area.begin.x + area.width - pos.x; rem > 0) {
                _buf[pos.y, pos.x / pxPerByte] ^= 0xf0;
            }
        }
    }

    void clear(const Rectangle& area) noexcept
    {
        const unsigned int horizBitOffset = area.begin.x % pxPerByte;

        for (unsigned int y = 0; y < area.height; ++y) {
            auto pos = area.begin + Point{0, y};

            if (horizBitOffset) {
                _buf[pos.y, pos.x / pxPerByte] &= ~0x0f;
                pos += Point{1, 0};
            }

            const unsigned int hzBytes = (area.begin.x + area.width - pos.x) / pxPerByte;
            std::fill(&_buf[pos.y, pos.x / pxPerByte], &_buf[pos.y, pos.x / pxPerByte + hzBytes], 0x00);
            pos += Point{hzBytes * pxPerByte, 0};

            if (const auto rem = area.begin.x + area.width - pos.x; rem > 0) {
                _buf[pos.y, pos.x / pxPerByte] &= ~0xf0;
            }
        }
    }

  private:
    void _drawHLine(const Point& begin, unsigned int length) noexcept
    {
        const unsigned int horizBitOffset = begin.x % pxPerByte;
        Point pos = {begin.x, begin.y};

        if (horizBitOffset) {
            _buf[pos.y, pos.x / pxPerByte] |= 0xf;
            --length;
            ++pos.x;
        }

        const unsigned int hzBytes = length / pxPerByte;
        std::fill_n(&_buf[pos.y, pos.x / pxPerByte], hzBytes, 0xff);

        if (const auto rem = length - hzBytes * pxPerByte; rem > 0) {
            _buf[pos.y, pos.x / pxPerByte + hzBytes] |= 0xf0;
        }
    }

    void _drawVLineMasked(const Point& begin, unsigned int length, std::uint8_t mask) noexcept
    {
        for (unsigned int y = 0; y < length; ++y) {
            _buf[begin.y + y, begin.x / pxPerByte] |= mask;
        }
    }

  private:
    BufferType _buf{};
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_FRAMEBUFFER_HPP
