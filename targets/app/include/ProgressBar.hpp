#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_PROGRESSBAR_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_PROGRESSBAR_HPP

#include "Primitives.hpp"

class ProgressBar {
  public:
    static constexpr unsigned int lineThickness = 2;
    static constexpr unsigned int airGap = 1;
    static constexpr unsigned int max = 256;

    constexpr ProgressBar(auto& display, Point pos, unsigned int width, unsigned int height) noexcept
     : _pos(pos)
     , _width(width)
     , _height(height)
    {
        display.drawHLine(_pos, _width, lineThickness);
        display.drawHLine({_pos.x, _pos.y + _height - lineThickness}, _width, lineThickness);
        display.drawVLine(_pos, _height, lineThickness);
        display.drawVLine({_pos.x + _width - lineThickness, _pos.y}, _height, lineThickness);
    }

    // From 0 to 255
    void increment(auto& display) noexcept
    {
        if (_cnt >= max) {
            return;
        }

        const unsigned int barWidth = _width - 2 * (lineThickness + airGap);
        const unsigned int oldPixelWidth = _cnt * barWidth / max;
        ++_cnt;
        const unsigned int pixelWidth = _cnt * barWidth / max;

        for (unsigned int i = oldPixelWidth; i < pixelWidth; ++i) {
            if ((i + 1) % 5) {    // separated into blocks of 4 pixel
                const Point barOrigin = {_pos.x + lineThickness + airGap, _pos.y + lineThickness + airGap};
                const unsigned int barHeight = _height - 2 * (lineThickness + airGap);
                display.drawVLine({barOrigin.x + i, barOrigin.y}, barHeight, 1);
            }
        }
    }

    bool isDone() const noexcept { return _cnt >= max; }

  private:
    unsigned int _cnt{};
    Point _pos;
    unsigned int _width;
    unsigned int _height;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_PROGRESSBAR_HPP
