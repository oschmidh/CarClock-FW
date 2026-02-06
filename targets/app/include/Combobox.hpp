#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_COMBOBOX_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_COMBOBOX_HPP

#include "Widget.hpp"
#include "Primitives.hpp"
#include "Label.hpp"

#include <string_view>

// TODO move into class (or source file):
static constexpr Bitmap<3, 5, 4> arrowLeft{0x00, 0xf0, 0x0f, 0xf0, 0xff, 0xf0, 0x0f, 0xf0, 0x00, 0xf0};
static constexpr Bitmap<3, 5, 4> arrowRight{0xf0, 0x00, 0xff, 0x00, 0xff, 0xf0, 0xff, 0x00, 0xf0, 0x00};

template <std::size_t NUM_OPTS_V>
class Combobox : public Widget {    // TODO hoist base class to avoid bloat
    int sideDist = 4;               // space between arrow and text

  public:
    struct Option {
        unsigned int id;
        std::string_view name;
    };

    // struct Config {
    //     // Widget::HzAlignment hzAlignment = Widget::HzAlignment::Left;
    //     // Widget::VtAlignment vtAlignment = Widget::VtAlignment::Top;
    // };

    constexpr Combobox(Point pos, Font font, std::array<Option, NUM_OPTS_V> options, Alignment align) noexcept
     : Widget(pos, align)
     , _font(font)
    //  , _cfg(cfg)
    {
        // std::ranges::copy(options, _options);
        std::copy(options.begin(), options.end(), _options.begin());
    }

    void draw(auto& display) noexcept
    {
        display.clear({_pos, _width, _height});

        const auto pos = _pos + alignmentOffset(contentWidth(), contentHeight());
        display.draw(arrowLeft, pos);
        // TODO draw little marking to show current sel
        const Point textOffset{arrowLeft.width + sideDist, 0};
        display.draw(_options[_sel].name, pos + textOffset, _font);
        const Point rightArrowOffset = textOffset + Point{textWidth() + sideDist, 0};
        display.draw(arrowRight, pos + rightArrowOffset);
    }

    constexpr void selNext() noexcept
    {
        if (++_sel >= NUM_OPTS_V) {
            _sel = 0;
        }
    }

    constexpr void selPrev() noexcept
    {
        if (_sel <= 0) {
            _sel = NUM_OPTS_V;
        }
        --_sel;
    }

    // constexpr void setPos(Point pos) noexcept { _label.setPos(pos); }
    // constexpr void resize(unsigned int width, unsigned int height) noexcept { _label.resize(width, height); }

    constexpr int contentWidth() const noexcept
    {
        return arrowLeft.width + textWidth() + arrowRight.width + 2 * sideDist;
    }

    constexpr int contentHeight() const noexcept { return textHeight(); }

  private:
    constexpr int textWidth() const noexcept
    {
        return _options[_sel].name.size() ? _options[_sel].name.size() * (_font.width + _font.kerning) - _font.kerning
                                          : 0;
    }

    constexpr int textHeight() const noexcept { return _font.height; }

    Font _font;
    // Config _cfg;
    unsigned int _sel{};
    std::array<Option, NUM_OPTS_V> _options;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_COMBOBOX_HPP
