#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_HOMESCREEN_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_HOMESCREEN_HPP

#include "TimeProvider.hpp"

#include <zephyr/kernel.h>

#include <chrono>

namespace App {

template <typename DISPLAY_T>
class HomeScreen {
  public:
    HomeScreen(DISPLAY_T& disp, const TimeProvider& time) noexcept
     : _display(disp)
     , _time(time)
    {
        // TODO setup gui
    }

    ~HomeScreen() noexcept
    {
        // _display.clear();
    }

    template <typename PROVIDER_MANAGER_T>
    static HomeScreen<DISPLAY_T> create(DISPLAY_T& display, PROVIDER_MANAGER_T& providers) noexcept
    {
        return HomeScreen(display, providers.template get<TimeProvider>());
    }

    auto run() noexcept -> std::optional<std::chrono::milliseconds>
    {
        const auto time = _time.getTime();
        // _display.draw(time); // TODO implement
        _display.update();
        return std::chrono::milliseconds(1000);
    }

    // void onEvent() noexcept{}   // TODO

  private:
    DISPLAY_T& _display;
    const TimeProvider& _time;
};

}    // namespace App

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_HOMESCREEN_HPP
