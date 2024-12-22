#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_HOMESCREEN_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_HOMESCREEN_HPP

#include "TimeProvider.hpp"

#include <zephyr/kernel.h>

namespace App {

class HomeScreen {
  public:
    HomeScreen(Display& disp, const TimeProvider& time) noexcept
     : _display(disp)
     , _time(time)
    {
        // TODO setup gui
    }

    ~HomeScreen() noexcept
    {
        // _display.clear();
    }

    void run() noexcept
    {
        const auto time = _time.getTime();
        _display.draw(time);
        _display.write();
        k_msleep(K_MSEC(1000));
    }

    // void onEvent() noexcept{}   // TODO

  private:
    Display& _display;
    const TimeProvider& _time;
};

}    // namespace App

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_HOMESCREEN_HPP
