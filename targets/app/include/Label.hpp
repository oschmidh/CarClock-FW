#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_LABEL_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_LABEL_HPP

#include <Widget.hpp>
#include <Primitives.hpp>
#include <Font.hpp>

class Label : public Widget {
  public:
    // struct Config {
    //     HzAlignment hzAlignment = HzAlignment::Left;
    //     VtAlignment vtAlignment = VtAlignment::Top;
    // };

    // constexpr Label(Point pos, unsigned int width, unsigned int height, Font font, Config cfg) noexcept
    //  : Widget(pos, width, height)
    //  , _font(font)
    //  , _cfg(cfg)
    // { }

    constexpr Label(Point pos, Font font, Alignment align) noexcept
     : Widget(pos, align)
     , _font(font)
    //  , _cfg(cfg)
    { }

    constexpr Label(Point pos, Font font) noexcept
     : Label(pos, font, {})
    { }

    void draw(auto& display) const noexcept
    {
        // const unsigned int xAlignOffset = _align.hz == HzAlignment::Center  ? textWidth() / 2
        //                                   : _align.hz == HzAlignment::Right ? textWidth()
        //                                                                     : 0;

        // const unsigned int yAlignOffset = _align.vt == VtAlignment::Center   ? (_height - textHeight()) / 2
        //                                   : _align.vt == VtAlignment::Bottom ? _height - textHeight()
        //                                                                      : 0;

        const auto pos = _pos + alignmentOffset(textWidth(), textHeight());

        display.clear({pos, textWidth(), textHeight()});
        display.draw(_text, pos, _font);
    }

    constexpr void setText(std::string_view text) noexcept { _text = text; }
    constexpr void setFont(Font font) noexcept { font = _font; }

  private:
    constexpr unsigned int textWidth() const noexcept
    {
        return _text.size() ? _text.size() * (_font.width + _font.kerning) - _font.kerning : 0;
    }

    constexpr unsigned int textHeight() const noexcept { return _font.height; }

    std::string_view _text;
    Font _font;
    // Config _cfg;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_LABEL_HPP
