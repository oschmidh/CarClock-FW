#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_LABEL_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_LABEL_HPP

#include <Primitives.hpp>

class Label {
    static constexpr unsigned int sideOffset = 6;
    static constexpr unsigned int topOffset = 2;

  public:
    enum class Alignment { Left, Right, Center };
    enum class Frame { None, Corners, Full };
    struct Config {
        Alignment alignment = Alignment::Left;
        Frame frame = Frame::None;
        bool invert = false;
    };

    Label(auto& display, Point pos, Config cfg = {}) noexcept
     : _pos(pos)
     , _cfg(cfg)
    { }

    template <typename FONT_T>
    void setText(auto& display, std::string_view text, const FONT_T& font) noexcept
    {

        const unsigned int xAlignOffset = _cfg.alignment == Alignment::Center  ? boxWidth(text, font) / 2
                                          : _cfg.alignment == Alignment::Right ? boxWidth(text, font)
                                                                               : 0;

        const auto pos = Point(_pos.x - xAlignOffset, _pos.y);

        static constexpr auto textOffset = Point(sideOffset, topOffset);
        display.draw(text, pos + textOffset, font);

        switch (_cfg.frame) {
            case Frame::Corners:
                display.drawHLine(pos, 3);
                display.drawVLine(pos, 3);

                display.drawHLine({pos.x + boxWidth(text, font) - 3, pos.y}, 2);
                display.drawVLine({pos.x + boxWidth(text, font) - 1, pos.y}, 3);

                display.drawHLine({pos.x, pos.y + boxHeight(text, font) - 1}, 3);
                display.drawVLine({pos.x, pos.y + boxHeight(text, font) - 3}, 2);

                display.drawHLine({pos.x + boxWidth(text, font) - 3, pos.y + boxHeight(text, font) - 1}, 2);
                display.drawVLine({pos.x + boxWidth(text, font) - 1, pos.y + boxHeight(text, font) - 3}, 3);
                break;
            case Frame::Full:
                display.draw(Rectangle{.begin = pos, .width = boxWidth(text, font), .height = boxHeight(text, font)},
                             false);
                break;
            case Frame::None: [[fallthrough]];
            default:;
        }

        if (_cfg.invert) {
            display.invert({pos, boxWidth(text, font), boxHeight(text, font)});
        }
    }

  private:
    template <typename FONT_T>
    static constexpr unsigned int boxWidth(std::string_view text, const FONT_T& font) noexcept
    {
        return text.size() * (font.width + font.kerning) - font.kerning + 2 * sideOffset;
    }

    template <typename FONT_T>
    static constexpr unsigned int boxHeight(std::string_view text, const FONT_T& font) noexcept
    {
        return font.height + 2 * topOffset;
    }

    Point _pos;
    const Config _cfg;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_LABEL_HPP
