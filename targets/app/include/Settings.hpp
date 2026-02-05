#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_SETTINGS_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_SETTINGS_HPP

#include "AppBase.hpp"
#include "SettingsProvider.hpp"
#include "Checkbox.hpp"
#include "Combobox.hpp"
#include "Grid.hpp"
#include "ScrollingList.hpp"

#include "smallText.hpp"

#include <optional>

namespace App {

static constexpr const char* str0 = "HELLO";
static constexpr const char* str1 = "WORLD";
static constexpr const char* str2 = "SCROLLING";
static constexpr const char* str3 = "LIST";
static constexpr const char* str4 = "ITEMS";

class Settings : public AppBase {
  public:
    Settings([[maybe_unused]] auto& display, [[maybe_unused]] const auto& providers) noexcept
    //  : _timeFmt12hBtn(
    //        display, {132, 18}, smallText,
    //        {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Center, .frame = Textbox::Frame::Corners})
    //  , _timeFmt24hBtn(
    //        display, {220, 18}, smallText,
    //        {.hz = Widget::HzAlignment::Right, .vt = Widget::VtAlignment::Center, .frame = Textbox::Frame::Corners})

    // Settings(auto& display, const SettingsProvider& settings) noexcept
    //  : _timeFmt12hBtn(display, {132, 18}, smallText,
    //                   {.hzAlignment = Label::HzAlignment::Left,
    //                    .vtAlignment = Label::VtAlignment::Center,
    //                    .frame = Label::Frame::Corners})
    //  , _timeFmt24hBtn(display, {220, 18}, smallText,
    //                   {.hzAlignment = Label::HzAlignment::Right,
    //                    .vtAlignment = Label::VtAlignment::Center,
    //                    .frame = Label::Frame::Corners})
    {
        // printk("ctor (%p)\n", this);
        // printk("_timeFmtSettingLabel addr: %p\n", &_timeFmtSettingLabel);
        // printk("_timeFmt24hBtn addr: %p\n", &_timeFmt24hBtn);
        // printk("_timeFmt12hBtn addr: %p\n", &_timeFmt12hBtn);

        _timeFmtSettingLabel.setText("TIME@FORMAT");    // space is mapped to '@'
        _timeFmt12hBtn.setText("12H");
        _timeFmt24hBtn.setText("24H");
        _timeFmt24hBtn.setChecked(true);

        _tempUnitSettingLabel.setText("TEMP@UNIT");    // space is mapped to '@'
        // _timeFmt12hBtn.draw(display);
        // _timeFmt24hBtn.draw(display);

        // _labels[0].setText("Hello");
        // _labels[1].setText("World");
        // _labels[2].setText("Scrolling");
        // _labels[3].setText("List");
        // _labels[4].setText("Items");

        _labels[0].setText(str0);
        _labels[1].setText(str1);
        _labels[2].setText(str2);
        _labels[3].setText(str3);
        _labels[4].setText(str4);

        _settingsList.draw(display);

        // printk("_timeFmtSettingLabel addr: %p\n", &_timeFmtSettingLabel);
        // printk("_timeFmt12hBtn addr: %p\n", &_timeFmt12hBtn);
        // printk("_timeFmt24hBtn addr: %p\n", &_timeFmt24hBtn);
        // printk("_timeFmtSetting addr: %p\n", &_timeFmtSetting);
    }

    // static Settings create([[maybe_unused]] auto& display, [[maybe_unused]] auto& providers) noexcept
    // {
    //     return Settings(display, providers.template get<SettingsProvider>());
    // }

    auto run(auto& display) noexcept -> std::optional<std::chrono::milliseconds>
    {
        // printk("===========\n   run\n");
        // printk("run (%p)\n", this);
        // printk("_timeFmtSettingLabel addr: %p\n", &_timeFmtSettingLabel);
        // printk("_timeFmt24hBtn addr: %p\n", &_timeFmt24hBtn);
        // printk("_timeFmt12hBtn addr: %p\n", &_timeFmt12hBtn);
        // static bool _24h;
        // _timeFmt12hBtn.setChecked(!_24h);
        // _timeFmt24hBtn.setChecked(_24h);
        // _24h = !_24h;
        // _timeFmt12hBtn.draw(display);
        // _timeFmt24hBtn.draw(display);

        if (up) {
            _settingsList.selUp();
            if (--i <= 0) {
                up = false;
            }
        } else {
            _settingsList.selDown();
            if (++i >= 5) {
                up = true;
            }
        }
        _settingsList.draw(display);

        return std::chrono::milliseconds(1000);
    }

  private:
    int i{};      // TEST
    bool up{};    // TEST
    Checkbox _timeFmt12hBtn{{132, 18},
                            smallText,
                            {.frame = Textbox::Frame::Corners},
                            {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Center}};
    Checkbox _timeFmt24hBtn{{220, 18},
                            smallText,
                            {.frame = Textbox::Frame::Corners},
                            {.hz = Widget::HzAlignment::Right, .vt = Widget::VtAlignment::Center}};
    std::array<Label, 5> _labels{
        {{{}, smallText, {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Center}},
         {{}, smallText, {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Center}},
         {{}, smallText, {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Center}},
         {{}, smallText, {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Center}},
         {{}, smallText, {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Center}}}};
    Label _timeFmtSettingLabel{{}, smallText, {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Center}};
    Grid<1, 3, Label&, Checkbox&, Checkbox&> _timeFmtSetting{_timeFmtSettingLabel, _timeFmt24hBtn, _timeFmt12hBtn};

    Label _tempUnitSettingLabel{{}, smallText, {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Center}};
    Combobox<3> _tempUnitSelCBox{
        {},
        smallText,
        {{{.id = 0, .name = "CELSIUS"}, {.id = 1, .name = "KELVIN"}, {.id = 2, .name = "FAHRENHEIT"}}},
        {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Center}};
    Grid<1, 2, Label&, Combobox<3>&> _tempUnitSetting{_tempUnitSettingLabel, _tempUnitSelCBox};

    ScrollingList<Grid<1, 3, Label&, Checkbox&, Checkbox&>&, Grid<1, 2, Label&, Combobox<3>&>&, Label&, Label&, Label&,
                  Label&, Label&>
        _settingsList{{28, 2},    213,        62,         _timeFmtSetting, _tempUnitSetting,
                      _labels[0], _labels[1], _labels[2], _labels[3],      _labels[4]};

    // Grid<1, 3> _timeFmtSetting{&_timeFmtSettingLabel, &_timeFmt24hBtn, &_timeFmt12hBtn};
    // ScrollingList<6> _settingsList{{28, 2},     &_timeFmtSetting, &_labels[0], &_labels[1],
    //                                &_labels[2], &_labels[3],      &_labels[4]};
};

}    // namespace App

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_SETTINGS_HPP
