#ifndef CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_HOMESCREEN_H
#define CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_HOMESCREEN_H

#include "TimeProvider.hpp"

#include <zephyr/kernel.h>

#include <chrono>

namespace App {

class HomeScreen {
  public:
    HomeScreen(const TimeProvider& time) noexcept
     : _time(time)
    {
        // TODO setup gui
    }

    ~HomeScreen() noexcept { }

    static HomeScreen create([[maybe_unused]] auto& display, [[maybe_unused]] auto& providers) noexcept
    {
        return HomeScreen(providers.template get<TimeProvider>());
    }

    auto run(auto& display) noexcept -> std::optional<std::chrono::milliseconds>
    {
        const auto time = _time.getTime();
        // display.draw(time); // TODO implement
        display.update();
        return std::chrono::milliseconds(1000);
    }

    // void onEvent() noexcept{}   // TODO

  private:
    const TimeProvider& _time;
};

}    // namespace App

#endif    // CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_HOMESCREEN_H
