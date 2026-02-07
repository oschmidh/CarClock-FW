#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_HOMESCREEN_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_HOMESCREEN_HPP

#include "timeFont.hpp"
#include "smallNumbers.hpp"
#include "smallText.hpp"
#include "homeScreenBackground.hpp"
#include "dateIcon.hpp"

#include "AppBase.hpp"
#include "TimeProvider.hpp"
#include "TemperatureProvider.hpp"
#include "Textbox.hpp"
#include "ProgressBar.hpp"
#include "Textbox.hpp"
#include "Primitives.hpp"
#include "Font.hpp"

#include <zephyr/kernel.h>

#include <chrono>

// static constexpr Bitmap<7, 7> tempIconSmall{0x00, 0x30, 0x4e, 0x51, 0x4e, 0x30, 0x00};
// static constexpr Bitmap<50, 13> tempBox{
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x18, 0x00, 0x1c, 0x00, 0x1e, 0x00, 0x1f};
// static constexpr Bitmap<21, 17> mitsubishiIconSmall{
//     0x00, 0x00, 0x01, 0x00, 0xc0, 0x01, 0x00, 0xf0, 0x01, 0x00, 0xf8, 0x01, 0x00, 0xf8, 0x01, 0x00,
//     0xf8, 0x01, 0x00, 0xf8, 0x01, 0x20, 0xf8, 0x00, 0xf8, 0x38, 0x00, 0xfe, 0x0b, 0x00, 0xff, 0x07,
//     0x00, 0xfe, 0x0b, 0x00, 0xf8, 0x38, 0x00, 0x20, 0xf8, 0x00, 0x00, 0xf8, 0x01, 0x00, 0xf8, 0x01,
//     0x00, 0xf8, 0x01, 0x00, 0xf8, 0x01, 0x00, 0xf0, 0x01, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x01};

// static constexpr const char* const text0 = "[ OK ]  Started LSB: successful.";
// static constexpr const char* const text1 = "[ OK ]  Reached target Host.";
// static constexpr const char* const text2 = "        Starting target Host...";
// static constexpr const char* const text3 = "[ OK ]  Started Thermal Daemon.";
// static constexpr const char* const text4 = "[ OK ]  PCI Bridge (bus 00-ff).";
// static constexpr const char* const text5 = "[ OK ]  Started Clock Deamon.";
// static constexpr const char* const text6 = "[ OK ]  Mounted /root.";

static constexpr const char* const texts[] = {"[ OK ]  Started LSB: successful.",
                                              "[ OK ]  Reached target Host.",
                                              "        Starting target Host...",
                                              "[ OK ]  Started Thermal Daemon.",
                                              "[ OK ]  PCI Bridge (bus 00-ff).",
                                              "[ OK ]  Started Clock Deamon.",
                                              "[ OK ]  Mounted /root."};

static constexpr auto timeStringPos = Point(158, 16);
// static constexpr auto backgroundPos = Point(0, 0);
static constexpr auto weekdayPos = Point(149, 59);
static constexpr auto dateIconPos = Point(192, 2);
static constexpr auto dateStringPos = dateIconPos + Point(15, 0);
static constexpr auto timezonePos = Point(227, 59);
static constexpr auto tempInsidePos = Point(15, 31);
static constexpr auto tempOutsidePos = tempInsidePos + Point(0, 11);

static constexpr auto mitsubishiTextPos = Point(0, 0);

// colors from 0-15
// static constexpr auto dimmedColor = 8;    // TODO find better name

namespace App {

class HomeScreen : public AppBase {
  public:
    HomeScreen([[maybe_unused]] auto& display, [[maybe_unused]] const auto& providers) noexcept
     : HomeScreen(display, providers.template get<TimeProvider>(), providers.template get<TemperatureProvider>())
    { }

    HomeScreen(auto& display, const TimeProvider& time, const TemperatureProvider& temp) noexcept
     //  : _bar(display, {5, 56}, 100, 13)
     //  , _tBox(display, Font::MiniFont, {5, 0}, 100)
     : /*_weekdayLabel(smallText, weekdayPos,
                     {.hzAlignment = Textbox::HzAlignment::Center, .frame = Textbox::Frame::Corners})
     , */
     _time(time)
     , _temp(temp)
    {

        display.draw(homeScreenBackground, {0, 0});

        // // Draw deco frame
        // display.draw(mitshubishiBmp, mitsubishiTextPos); // TODO add image

        // display.drawHLine({0, 11}, 256);    // TODO bake into bitmaps at compile time?

        // display.drawHLine({0, 56}, 123);
        // display.drawVLine({122, 56}, 8);      // TODO length correct?
        // display.drawHLine({125, 56}, 130);    // TODO length correct?
        // display.drawVLine({125, 56}, 8);      // TODO length correct?

        // display.drawHLine({0, 31}, 25, 2);
        // // display.draw(Line{{25, 31}, {50, 56}}, 2);
        // display.draw(Line{{25, 31}, {50, 56}}, 1);    // TODO because thickness is not supported
        // display.draw(Line{{25, 32}, {50, 57}}, 1);
        // display.drawHLine({51, 56}, 77, 2);

        // // display.draw(tempBox, {2, 13});
        // display.draw(tempIconSmall, {4, 15});
        // // display.invert({{2, 13}, tempBox.width, tempBox.height});
        // display.draw(mitsubishiIconSmall, {10, 40});
        // display.draw("XYZ[\\]^_`", {10, 5}, Font::MiniFont);
        // display.draw("abcdefghijklmnopqrstuvwxyz", {10, 13}, Font::MiniFont);
        // display.draw("ABCDEFGHIJKLMNOPQRSTUVWXYZ", {10, 21}, Font::MiniFont);
        // display.draw("0123456789", {10, 29}, Font::MiniFont);

        // Temperature box
        // display.draw(Rectangle{{0, 16}, 47, 35}, false);
        // display.drawHLine({0, 28}, 47);
        // TODO show temperatures

        // display.draw(dateIcon, dateIconPos);    // TODO should be a dimmed color
        // display.drawVLine(dateIconPos + Point{11, -1}, 9, 1, dimmedColor);

        constexpr std::array<const char*, 7> wdayStrings = {"MONDAY", "TUESDAY",  "WEDNESDAY", "THURSDAY",
                                                            "FRIDAY", "SATURDAY", "SUNDAY"};

        const auto date = _time.getDate();    // TODO update periodically?
        // display.drawVLine(weekdayPos + Point{-4, -1}, 6, 1, dimmedColor);
        _weekdayLabel.setText(wdayStrings[date.wday]);
        _weekdayLabel.draw(display);

        // display.drawVLine(timezonePos + Point{-4, -1}, 6, 1, dimmedColor);
        _timezoneLabel.setText("GMT+1");    // TODO hardcoded
        _timezoneLabel.draw(display);

        std::array<char, 11> dateStr{};
        std::snprintf(dateStr.data(), dateStr.size(), "%02d.%02d.%04d", date.day, date.month, date.year + 1900);
        // display.draw(dateStr, dateStringPos, smallNumbers);
        display.draw(std::string_view{dateStr.data()}, dateStringPos, smallNumbers);
    }

    ~HomeScreen() noexcept { }

    // static HomeScreen create([[maybe_unused]] auto& display, [[maybe_unused]] auto& providers) noexcept
    // {
    //     return HomeScreen(display, providers.template get<TimeProvider>());
    // }

    auto run(auto& display) noexcept -> std::optional<std::chrono::milliseconds>
    {
        const auto time = _time.getTime();
        std::array<char, 6> timeString{};
        std::snprintf(timeString.data(), timeString.size(), "%02d%c%02d", time.hours, _colonBlink ? ':' : '/',
                      time.minutes);    // NOTE ' ' is mapped to '/' in font

        _colonBlink = !_colonBlink;

        // printk("run (%s)\n", timeString.data());

        // TODO print temperature

        const auto displayTemp = [&display](std::expected<int, int> temp, Textbox& label) noexcept {
            if (temp.has_value()) {
                std::array<char, 7> buf{};
                // '°' is mapped to ';' and 'C' is mapped to '<':
                std::snprintf(buf.data(), buf.size(), "%02d.%01d;<", temp.value() / 10, std::abs(temp.value() % 10));
                label.setText(std::string_view{buf.data()});
            } else {
                // ' ' is mapped to '*':
                label.setText("*---");
            }

            label.draw(display);
        };

        displayTemp(_temp.get(TemperatureProvider::Sensors::Inside), _tempInsideLabel);    // TODO test negative temps
        displayTemp(_temp.get(TemperatureProvider::Sensors::Outside), _tempOutsideLabel);

        // _bar.increment(display);

        // static int i;
        // switch (i) {
        //     case 0: display.draw(text0, {5, 0}, Font::MiniFont); break;
        //     case 1: display.draw(text1, {5, 8}, Font::MiniFont); break;
        //     case 2: display.draw(text2, {5, 16}, Font::MiniFont); break;
        //     case 3: display.draw(text3, {5, 24}, Font::MiniFont); break;
        //     case 4: display.draw(text4, {5, 32}, Font::MiniFont); break;
        //     case 5: display.draw(text5, {5, 40}, Font::MiniFont); break;
        //     case 6: display.draw(text6, {5, 48}, Font::MiniFont); break;
        //     default: break;
        // }
        // ++i;

        // if (i <= 6) {
        //     printk("adding line\n");
        //     _tBox.addLine(display, texts[i]);
        //     ++i;
        // }

        display.draw(std::string_view{timeString}, timeStringPos, timeFont);
        // display.update();

        return std::chrono::milliseconds(1000);
    }

    // void onEvent() noexcept{}   // TODO

  private:
    // ProgressBar _bar;
    // Textbox<decltype(Font::MiniFont), 4> _tBox;
    Textbox _weekdayLabel{weekdayPos,
                          smallText,
                          {.frame = Textbox::Frame::None, .invert = false},
                          {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Top}};
    Textbox _timezoneLabel{timezonePos,
                           smallText,
                           {.frame = Textbox::Frame::None, .invert = false},
                           {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Top}};
    Textbox _tempInsideLabel{tempInsidePos,
                             smallNumbers,
                             {.frame = Textbox::Frame::None, .invert = false},
                             {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Top}};
    Textbox _tempOutsideLabel{tempOutsidePos,
                              smallNumbers,
                              {.frame = Textbox::Frame::None, .invert = false},
                              {.hz = Widget::HzAlignment::Left, .vt = Widget::VtAlignment::Top}};
    bool _colonBlink = true;
    const TimeProvider& _time;
    const TemperatureProvider& _temp;
};

}    // namespace App

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_HOMESCREEN_HPP
