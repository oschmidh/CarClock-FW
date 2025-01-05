#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_PRIMITIVES_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_PRIMITIVES_HPP

#include <array>
#include <cstdint>

struct Point {
    int x;
    int y;

    constexpr bool operator==(const Point& other) const noexcept { return (x == other.x) && (y == other.y); }
    constexpr bool operator!=(const Point& other) const noexcept { return !(*this == other); }
};

struct Line {
    Point begin;
    Point end;
};

template <int WIDTH_V, int HEIGHT_V>
struct Bitmap {
    static constexpr int width = WIDTH_V;
    static constexpr int height = HEIGHT_V;
    std::array<std::uint8_t, (height + 1 / 8) * width> data;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_PRIMITIVES_HPP
