#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_STATISTICS_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_STATISTICS_HPP

#include "AppBase.hpp"
#include "AccelerationProvider.hpp"
#include "Primitives.hpp"

#include <chrono>

namespace App {

class AccelGraph {
  public:
    AccelGraph(auto& display) noexcept
    {
        (void)display;
        // TODO draw frame
    }

    constexpr void update(auto& display, AccelerationProvider::Acceleration accel) noexcept
    {

        Point dot{.x = accel.x * frame.width / 2 / range, .y = accel.y * frame.width / 2 / range};

        dot += origin;

        if (dot == _prevDot) {
            return;
        }

        display.draw(Line{_prevDot, dot}, 1, traceColor);

        _prevDot = dot;
    }

  private:
    static constexpr std::uint8_t traceColor = 0x4;

    static constexpr int range = 2048;    // +- 2048mg

    static constexpr Rectangle frame{.begin = {0, 0}, .width = 64, .height = 64};
    static constexpr Point origin = frame.begin + Point{frame.width / 2, frame.height / 2};

    Point _prevDot = origin;
};

class Statistics : public AppBase {
  public:
    Statistics([[maybe_unused]] auto& display, [[maybe_unused]] const auto& providers) noexcept
     : Statistics(display, providers.template get<AccelerationProvider>())
    { }

    Statistics(auto& display, const AccelerationProvider& accel) noexcept
     : _accelGraph(display)
     , _accel(accel)
    {
        // display.draw(statsScreenBackground, {0, 0}); // TODO
    }

    ~Statistics() noexcept { }

    auto run(auto& display) noexcept -> std::optional<std::chrono::milliseconds>
    {
        using namespace std::literals::chrono_literals;

        const auto accel = _accel.get();
        if (!accel) {
            return 1000ms;
        }

        const auto displayAccel = [&display](int val, Textbox& label) noexcept {
            std::array<char, 6> buf{};
            // '°' is mapped to ';' and 'C' is mapped to '<':
            std::snprintf(buf.data(), buf.size(), "%01d.%03d<", val / 1000, std::abs(val % 1000));
            label.setText(std::string_view{buf.data()});
            label.draw(display);
        };

        displayAccel(accel.value().x, _accelXLabel);
        displayAccel(accel.value().y, _accelYLabel);

        _accelGraph.update(display, accel.value());

        // return 10ms;
        return 1000ms;    // TODO for test
    }

  private:
    static constexpr auto accelXPos = Point{93, 32};
    static constexpr auto accelYPos = accelXPos + Point{0, 14};

    AccelGraph _accelGraph;
    Textbox _accelXLabel{accelXPos, smallNumbers};
    Textbox _accelYLabel{accelYPos, smallNumbers};
    const AccelerationProvider& _accel;
};

}    // namespace App

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_STATISTICS_HPP
