#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_CHECKBOX_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_CHECKBOX_HPP

#include "Primitives.hpp"
#include "Label.hpp"

class Checkbox {
  public:
    struct Config {
        Label::HzAlignment hzAlignment = Label::HzAlignment::Left;
        Label::VtAlignment vtAlignment = Label::VtAlignment::Top;
        Label::Frame frame = Label::Frame::None;
    };

    constexpr Checkbox(auto& display, Point pos, Font font, Config cfg = {}) noexcept
     : _label(pos, font, {.hzAlignment = cfg.hzAlignment, .frame = cfg.frame, .invert = false})
    { }

    void draw(auto& display) noexcept
    {
        _label.setInvert(_checked);
        _label.draw(display);
    }

    constexpr void setPos(Point pos) noexcept { _label.setPos(pos); }
    constexpr void setText(std::string_view text) noexcept { _label.setText(text); }
    constexpr void setChecked(bool state) noexcept { _checked = state; }
    constexpr bool isChecked() const noexcept { return _checked; }

  private:
    Label _label;
    bool _checked{};
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_CHECKBOX_HPP
