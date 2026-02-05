#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_PRIMITIVES_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_PRIMITIVES_HPP

#include <array>
#include <mdspan/mdspan.hpp>
#include <cstdint>

struct Point {
    int x;
    int y;
};

constexpr bool operator==(const Point& lhs, const Point& rhs) noexcept { return (lhs.x == rhs.x) && (lhs.y == rhs.y); }

constexpr bool operator!=(const Point& lhs, const Point& rhs) noexcept { return !(lhs == rhs); }

constexpr Point& operator+=(Point& lhs, const Point& rhs) noexcept
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

constexpr Point operator+(const Point& lhs, const Point& rhs)
{
    Point res = lhs;
    res += rhs;
    return res;
}

constexpr Point& operator-=(Point& lhs, const Point& rhs) noexcept
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

constexpr Point operator-(const Point& lhs, const Point& rhs)
{
    Point res = lhs;
    res -= rhs;
    return res;
}

constexpr Point operator*(const Point& lhs, int scaler)
{
    Point res = lhs;
    res.x *= scaler;
    res.y *= scaler;
    return res;
}

struct Line {
    Point begin;
    Point end;
};

struct Rectangle {
    Point begin;
    int width;
    int height;
};

template <int WIDTH_V, int HEIGHT_V, unsigned int COLOR_DEPTH_V = 1>
struct Bitmap {
    static constexpr int width = WIDTH_V;
    static constexpr int height = HEIGHT_V;
    static constexpr std::size_t size() noexcept { return height * ((width * COLOR_DEPTH_V + 7) / 8); }
    std::array<std::uint8_t, size()> data;    // round up
};

struct BitmapView {
    constexpr BitmapView() noexcept = default;

    template <int WIDTH_V, int HEIGHT_V, unsigned int COLOR_DEPTH_V>
    constexpr BitmapView(const Bitmap<WIDTH_V, HEIGHT_V, COLOR_DEPTH_V>& bmp) noexcept
     : width(bmp.width)
     , height(bmp.height)
     , data(bmp.data.data(), height, (width * COLOR_DEPTH_V + 7) / 8)    // TODO how to ensure that
                                                                         // the size matches the
                                                                         // array?
    { }

    constexpr BitmapView(std::span<const std::uint8_t> raw, int width, int height) noexcept
     : width(width)
     , height(height)
     , data(raw.data(), height, (width * 4 + 7) / 8)    // TODO color depth hardcoded?
    { }

    int width{};
    int height{};
    Kokkos::mdspan<const std::uint8_t, Kokkos::dextents<unsigned int, 2>> data{};
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_PRIMITIVES_HPP
