#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_WIDGET_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_WIDGET_HPP

#include "Primitives.hpp"

class Widget {
  public:
    enum class HzAlignment { Left, Right, Center };
    enum class VtAlignment { Top, Bottom, Center };
    struct Alignment {
        HzAlignment hz = HzAlignment::Left;
        VtAlignment vt = VtAlignment::Top;
    };

    // constexpr Widget(Point pos, Alignment align = {}) noexcept
    //  : _pos(pos)
    //  , _align(align)
    // { }

    constexpr Widget(Point pos, Alignment align) noexcept
     : _pos(pos)
     , _align(align)
    { }

    constexpr Widget(Point pos) noexcept
     : Widget(pos, {})
    { }

    constexpr void setPos(Point pos) noexcept { _pos = pos; }

    constexpr void resize(unsigned int width, unsigned int height) noexcept
    {
        _width = width;
        _height = height;
    }

  protected:
    constexpr Point alignmentOffset(int contentWidth, int contentHeight) const noexcept
    {
        const int xAlignOffset = _align.hz == HzAlignment::Center  ? (_width - contentWidth) / 2
                                 : _align.hz == HzAlignment::Right ? _width - contentWidth
                                                                   : 0;

        const int yAlignOffset = _align.vt == VtAlignment::Center   ? (_height - contentHeight) / 2
                                 : _align.vt == VtAlignment::Bottom ? _height - contentHeight
                                                                    : 0;

        return {xAlignOffset, yAlignOffset};
    }

    int _width{};
    int _height{};
    Point _pos{};

  private:
    const Alignment _align{};
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_WIDGET_HPP
