#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_PRIMITIVES_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_PRIMITIVES_HPP

#include <array>
#include <mdspan/mdspan.hpp>
#include <cstdint>

struct Point {
    unsigned int x;
    unsigned int y;

    constexpr bool operator==(const Point& other) const noexcept { return (x == other.x) && (y == other.y); }
    constexpr bool operator!=(const Point& other) const noexcept { return !(*this == other); }

    constexpr Point& operator+=(const Point& other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    friend Point operator+(Point lhs, const Point& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    constexpr Point& operator-=(const Point& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    friend Point operator-(Point lhs, const Point& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
};

struct Line {
    Point begin;
    Point end;
};

struct Rectangle {
    Point begin;
    unsigned int width;
    unsigned int height;
};

template <unsigned int WIDTH_V, unsigned int HEIGHT_V, unsigned int COLOR_DEPTH_V = 1>
struct Bitmap {
    static constexpr unsigned int width = WIDTH_V;
    static constexpr unsigned int height = HEIGHT_V;
    static constexpr std::size_t size() noexcept { return height * ((width * COLOR_DEPTH_V + 7) / 8); }
    std::array<std::uint8_t, size()> data;    // round up
};

struct BitmapView {
    template <unsigned int WIDTH_V, unsigned int HEIGHT_V, unsigned int COLOR_DEPTH_V>
    constexpr BitmapView(const Bitmap<WIDTH_V, HEIGHT_V, COLOR_DEPTH_V>& bmp) noexcept
     : width(bmp.width)
     , height(bmp.height)
     , data(bmp.data.data(), height, (width * COLOR_DEPTH_V + 7) / 8)    // TODO how to ensure that
                                                                         // the size matches the
                                                                         // array?
    { }
    unsigned int width;
    unsigned int height;
    Kokkos::mdspan<const std::uint8_t, Kokkos::dextents<unsigned int, 2>> data;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_PRIMITIVES_HPP
