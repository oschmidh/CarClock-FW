#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_TEXTBOX_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_TEXTBOX_HPP

#include "Widget.hpp"
#include "Label.hpp"
#include "Primitives.hpp"
#include "Font.hpp"

// TODO move into class (or source file):
static constexpr Bitmap<4, 4, 4> frameCornerTopLeft{0xff, 0xf0, 0xf0, 0x00, 0xf0, 0x00};
static constexpr Bitmap<4, 4, 4> frameCornerTopRight{0xff, 0xf0, 0x00, 0xf0, 0x00, 0xf0};
static constexpr Bitmap<4, 4, 4> frameCornerBotLeft{0xf0, 0x00, 0xf0, 0x00, 0xff, 0xf0};
static constexpr Bitmap<4, 4, 4> frameCornerBotRight{0x00, 0xf0, 0x00, 0xf0, 0xff, 0xf0};

class Textbox : public Widget {
    static constexpr unsigned int sideOffset = 6;
    static constexpr unsigned int topOffset = 2;

  public:
    // enum class HzAlignment { Left, Right, Center };
    // enum class VtAlignment { Top, Bottom, Center };
    enum class Frame { None, Corners, Full };
    struct Config {
        Frame frame = Frame::None;
        bool invert = false;
    };

    // constexpr Label(Point pos, unsigned int width, unsigned int height, Font font, Config cfg) noexcept
    //  : Widget(pos, width, height)
    //  , _font(font)
    //  , _cfg(cfg)
    // { }

    constexpr Textbox(Point pos, Font font, Config cfg, Alignment align = {}) noexcept
     : Widget(pos, align)
     , _font(font)
     , _cfg(cfg)
    { }

    constexpr Textbox(Point pos, Font font, Alignment align = {}) noexcept
     : Textbox(pos, font, {}, align)
    { }

    void draw(auto& display) const noexcept
    {
        // const unsigned int xAlignOffset = _align.hz == HzAlignment::Center  ? boxWidth() / 2
        //                                   : _align.hz == HzAlignment::Right ? boxWidth()
        //                                                                     : 0;

        // const unsigned int yAlignOffset = _align.vt == VtAlignment::Center   ? (_height - boxHeight()) / 2
        //                                   : _align.vt == VtAlignment::Bottom ? _height - boxHeight()
        //                                                                      : 0;

        // const auto pos = Point(_pos.x + xAlignOffset, _pos.y + yAlignOffset);
        printk("_pos: %d,%d\n", _pos.x, _pos.y);
        printk("boxWidth: %d\n", boxWidth());
        printk("boxHeight: %d\n", boxHeight());
        const auto pos = _pos + alignmentOffset(boxWidth(), boxHeight());
        printk("alignmentOffset: %d,%d\n", alignmentOffset(boxWidth(), boxHeight()).x,
               alignmentOffset(boxWidth(), boxHeight()).y);

        display.clear({pos, boxWidth(), boxHeight()});

        printk("pos: %d,%d\n", pos.x, pos.y);

        const auto textOffset = _cfg.frame == Frame::None ? Point{} : Point(sideOffset, topOffset);
        printk("textOffset: %d,%d\n", textOffset.x, textOffset.y);

        display.draw(_text, pos + textOffset, _font);
        printk("draw text at %d,%d\n", (pos + textOffset).x, (pos + textOffset).y);

        switch (_cfg.frame) {
            case Frame::Corners:
                display.draw(frameCornerTopLeft, pos);
                display.draw(frameCornerTopRight, {pos.x + boxWidth() - 3, pos.y});
                display.draw(frameCornerBotLeft, {pos.x, pos.y + boxHeight() - 3});
                display.draw(frameCornerBotRight, {pos.x + boxWidth() - 3, pos.y + boxHeight() - 3});

                if (_cfg.invert) {
                    display.drawHLine(pos + Point{4, 0}, boxWidth() - 8, 2);
                    display.drawHLine(pos + Point{4, boxHeight() - 2}, boxWidth() - 8, 2);
                    display.drawVLine(pos + Point{0, 4}, boxHeight() - 8, 2);
                    display.drawVLine(pos + Point{boxWidth() - 2, 4}, boxHeight() - 8, 2);
                }
                break;
            case Frame::Full:
                display.draw(Rectangle{.begin = pos, .width = boxWidth(), .height = boxHeight()}, false);
                break;
            case Frame::None: [[fallthrough]];
            default:;
        }

        if (_cfg.invert) {
            // display.invert({pos + Point{2, 2}, boxWidth() - 4, boxHeight() - 4});
            display.invert({pos - Point{1, 1}, boxWidth() + 2, boxHeight() + 2});
            printk("inverting from %d,%d %d and %d\n", (pos + Point{2, 2}).x, (pos + Point{2, 2}).y, boxWidth() - 4,
                   boxHeight() - 4);
        }
    }

    // constexpr void setPos(Point pos) noexcept { _pos = pos; }
    constexpr void setText(std::string_view text) noexcept { _text = text; }
    constexpr void setFont(Font font) noexcept { font = _font; }
    constexpr void setInvert(bool enable) noexcept { _cfg.invert = enable; }
    // constexpr void resize(unsigned int width, unsigned int heigth) noexcept
    // {
    //     _width = width;
    //     _height = height;
    // }

  private:
    constexpr unsigned int boxWidth() const noexcept
    {
        const auto textwidth = _text.size() ? _text.size() * (_font.width + _font.kerning) - _font.kerning : 0;
        if (_cfg.frame == Frame::None) {
            return textwidth;
        }
        return textwidth + 2 * sideOffset;
    }

    constexpr unsigned int boxHeight() const noexcept
    {
        if (_cfg.frame == Frame::None) {
            return _font.height;
        }
        return _font.height + 2 * topOffset;
    }

    // Point _pos;
    std::string_view _text;
    Font _font;
    Config _cfg;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_TEXTBOX_HPP
