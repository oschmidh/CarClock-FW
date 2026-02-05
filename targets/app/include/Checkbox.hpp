#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_CHECKBOX_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_CHECKBOX_HPP

#include "Primitives.hpp"
#include "Textbox.hpp"

class Checkbox {    // TODO is rather a button than a checkbox...
  public:
    struct Config {
        // Label::HzAlignment hzAlignment = Label::HzAlignment::Left;
        // Label::VtAlignment vtAlignment = Label::VtAlignment::Top;
        Textbox::Frame frame = Textbox::Frame::None;
    };

    // constexpr Checkbox(Point pos, Font font, Config cfg = {}) noexcept
    //  : _label(pos, font,
    //           {.hzAlignment = cfg.hzAlignment, .vtAlignment = cfg.vtAlignment, .frame = cfg.frame, .invert = false})
    // { }

    constexpr Checkbox(Point pos, Font font, Config cfg, Widget::Alignment align = {}) noexcept
     : _label(pos, font, {.frame = cfg.frame, .invert = false}, align)
    { }

    constexpr Checkbox(Point pos, Font font, Widget::Alignment align = {}) noexcept
     : _label(pos, font, {.invert = false}, align)
    { }

    void draw(auto& display) noexcept
    {
        _label.setInvert(_checked);
        _label.draw(display);
    }

    constexpr void setPos(Point pos) noexcept { _label.setPos(pos); }
    constexpr void setText(std::string_view text) noexcept { _label.setText(text); }
    constexpr void resize(unsigned int width, unsigned int height) noexcept { _label.resize(width, height); }
    constexpr void setChecked(bool state) noexcept { _checked = state; }
    constexpr bool isChecked() const noexcept { return _checked; }

  private:
    Textbox _label;
    bool _checked{};
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_CHECKBOX_HPP
