#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_STATISTICS_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_STATISTICS_HPP

#include "Framebuffer.hpp"

#include "AppBase.hpp"
#include "AccelerationProvider.hpp"
#include "Primitives.hpp"

#include <chrono>
#include <numbers>
#include <cmath>

namespace App {

static constexpr Bitmap<7, 7, 4> indicator = {0x0f, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff,
                                              0xff, 0xf0, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xf0,
                                              0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0x00};

class Envelope {
  public:
    /*constexpr*/ void update(AccelerationProvider::Acceleration accel) noexcept
    {
        printk("env update (accel: [%d, %d])\n", accel.x, accel.y);
        Point pos{.x = accel.x * 60 / 2 / range, .y = accel.y * 60 / 2 / range};    // TODO canvas size hardcoded?
        const auto [distance, angle] = toPolar(pos.x, pos.y);
        printk("polar: [%f, %f])\n", distance, angle);

        const std::size_t index =
            std::round((angle + std::numbers::pi_v<float>)*(radialResolution - 1) / (2 * std::numbers::pi_v<float>));
        printk("angle index: %d)\n", index);
        if (distance < _peaks[index]) {
            return;
        }

        _peaks[index] = distance;
        // TODO redraw envelope
    }

    constexpr void draw(auto& display) const noexcept
    {
        // for (unsigned int p : _peaks) {
        std::array<Point, radialResolution> envelopePoints;

        for (unsigned int i = 0; i < _peaks.size(); ++i) {
            const float angle = i * 2 * std::numbers::pi_v<float> / (radialResolution - 1);

            const auto [x, y] = toCartesian(_peaks[i], angle);
            // printk("env point[%d]: [%f, %f])\n", i, x, y);
            const Point p{.x = std::round(x), .y = -std::round(y)};    // TODO explain y axis sign flip
            printk("env point[%d]: [%d, %d])\n", i, p.x, p.y);
            envelopePoints[i] = p + origin;
        }

        for (unsigned int i = 0; i < envelopePoints.size() - 1; ++i) {
            printk("draw line from [%d, %d] to [%d, %d]\n", envelopePoints[i].x, envelopePoints[i].y,
                   envelopePoints[i + 1].x, envelopePoints[i + 1].y);
            display.draw(Line{envelopePoints[i], envelopePoints[i + 1]}, 1, traceColor);
            // printk("line [%d-%d] drawn\n", i, i + 1);
        }
    }

  private:
    static constexpr int range = 2048;                        // +- 2048mg
    static constexpr std::uint8_t traceColor = 0x4;           // TODO rename evelopeColor?
    static constexpr Point origin = Point{60 / 2, 60 / 2};    // TODO canvas size hardcoded

    static constexpr std::pair<float, float> toCartesian(float distance, float angle) noexcept
    {
        const auto x = std::cos(angle) * distance;
        const auto y = std::sin(angle) * distance;
        return std::make_pair(x, y);
    }

    static constexpr std::pair<float, float> toPolar(float x, float y) noexcept
    {
        // const auto distance = x * x + y * y;    // TODO is actually distance^2
        const auto distance = std::sqrt(x * x + y * y);
        const auto angle = std::atan2(y, x);
        return std::make_pair(distance, angle);
    }

    // static constexpr width = 60;     // TODO define externally?
    // static constexpr height = 60;    // TODO define externally?
    // static constexpr std::size_t radialResolution = 16;
    static constexpr std::size_t radialResolution = 8;
    std::array<unsigned int, radialResolution> _peaks{};
};

// template <unsigned int WIDTH_V, unsigned int HEIGHT_V>
// class AccelGraph {
//   public:
//     AccelGraph(auto& display) noexcept
//     {
//         // TODO round corners
//         constexpr int thickness = 2;
//         display.drawHLine(frame.begin, WIDTH_V, thickness);
//         display.drawHLine(frame.begin + Point{0, HEIGHT_V - thickness}, WIDTH_V, thickness);
//         display.drawVLine(frame.begin, HEIGHT_V, thickness);
//         display.drawVLine(frame.begin + Point{WIDTH_V - thickness, 0}, HEIGHT_V, thickness);
//         drawScale(display);
//         // TODO draw small indicators
//     }

//     constexpr void update(auto& display, AccelerationProvider::Acceleration accel) noexcept
//     {
//         Point pos{.x = accel.x * canvas.width / 2 / range, .y = accel.y * canvas.width / 2 / range};
//         pos += origin;
//         if (pos == _prevPos) {
//             return;
//         }

//         _graphCanvas.draw(Line{_prevPos, pos}, 1, traceColor);
//         _prevPos = pos;

//         display.clear(canvas);
//         drawScale(display);
//         display.draw(BitmapView{_graphCanvas.data(), _graphCanvas.width(), _graphCanvas.height()}, canvas.begin,
//         true); display.draw(indicator, pos - Point{3, 3} + canvas.begin);    // TODO fix indicator exceeding sides
//     }

//   private:
//     constexpr void drawScale(auto& display) const noexcept
//     {
//         static constexpr std::uint8_t scaleColor = 0x2;
//         for (int i = 0; i < 4; ++i) {
//             constexpr int spacing = 8;
//             display.drawHLine(origin + canvas.begin + Point{-1, spacing * i}, 2, 1, scaleColor);
//             display.drawHLine(origin + canvas.begin + Point{-1, spacing * -i}, 2, 1, scaleColor);
//             display.drawVLine(origin + canvas.begin + Point{spacing * i, -1}, 2, 1, scaleColor);
//             display.drawVLine(origin + canvas.begin + Point{spacing * -i, -1}, 2, 1, scaleColor);
//         }
//     }

//     static constexpr std::uint8_t traceColor = 0x4;

//     static constexpr int range = 2048;    // +- 2048mg

//     static constexpr Rectangle frame{.begin = {0, 0}, .width = WIDTH_V, .height = HEIGHT_V};

//     static constexpr int offset = 4;
//     static constexpr Rectangle canvas{.begin = frame.begin + Point{offset, offset},
//                                       .width = frame.width - offset * 2,
//                                       .height = frame.height - offset * 2};
//     // static constexpr Point origin = canvas.begin + Point{canvas.width / 2, canvas.height / 2};
//     static constexpr Point origin = Point{canvas.width / 2, canvas.height / 2};    // origin is relative to canvas

//     Framebuffer<canvas.width, canvas.height, 4> _graphCanvas{};
//     Point _prevPos = origin;
// };

template <unsigned int WIDTH_V, unsigned int HEIGHT_V>
class AccelGraph {
  public:
    AccelGraph(auto& display) noexcept
    {
        // TODO round corners
        constexpr int thickness = 2;
        display.drawHLine(frame.begin, WIDTH_V, thickness);
        display.drawHLine(frame.begin + Point{0, HEIGHT_V - thickness}, WIDTH_V, thickness);
        display.drawVLine(frame.begin, HEIGHT_V, thickness);
        display.drawVLine(frame.begin + Point{WIDTH_V - thickness, 0}, HEIGHT_V, thickness);
        drawScale(display);
        // TODO draw small indicators
    }

    constexpr void update(auto& display, AccelerationProvider::Acceleration accel) noexcept
    {
        Point pos{.x = accel.x * graphArea.width / 2 / range, .y = accel.y * graphArea.width / 2 / range};
        pos += origin;
        if (pos == _prevPos) {
            return;
        }

        _prevPos = pos;

        _envelope.update(accel);
        printk("env updated\n");

        // _graphCanvas.draw(Line{_prevPos, pos}, 1, traceColor);

        display.clear(graphArea);
        drawScale(display);
        printk("scale drawn\n");
        _envelope.draw(display);
        printk("env drawn\n");
        display.draw(indicator, pos - Point{3, 3} + graphArea.begin);    // TODO fix indicator exceeding sides
    }

  private:
    constexpr void drawScale(auto& display) const noexcept
    {
        static constexpr std::uint8_t scaleColor = 0x2;
        for (int i = 0; i < 4; ++i) {
            constexpr int spacing = 8;
            display.drawHLine(origin + graphArea.begin + Point{-1, spacing * i}, 3, 1, scaleColor);
            display.drawHLine(origin + graphArea.begin + Point{-1, spacing * -i}, 3, 1, scaleColor);
            display.drawVLine(origin + graphArea.begin + Point{spacing * i, -1}, 3, 1, scaleColor);
            display.drawVLine(origin + graphArea.begin + Point{spacing * -i, -1}, 3, 1, scaleColor);
        }
    }

    static constexpr int range = 2048;    // +- 2048mg

    static constexpr Rectangle frame{.begin = {0, 0}, .width = WIDTH_V, .height = HEIGHT_V};

    static constexpr int offset = 4;
    static constexpr Rectangle graphArea{.begin = frame.begin + Point{offset, offset},
                                         .width = frame.width - offset * 2,
                                         .height = frame.height - offset * 2};
    // static constexpr Point origin = canvas.begin + Point{canvas.width / 2, canvas.height / 2};
    static constexpr Point origin = Point{graphArea.width / 2, graphArea.height / 2};    // origin is relative to canvas

    Envelope _envelope{};
    Point _prevPos = origin;
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
        // return 1ms;    // TODO for test
    }

  private:
    static constexpr auto accelXPos = Point{93, 32};
    static constexpr auto accelYPos = accelXPos + Point{0, 14};

    AccelGraph<63, 63> _accelGraph;
    Textbox _accelXLabel{accelXPos, smallNumbers};
    Textbox _accelYLabel{accelYPos, smallNumbers};
    const AccelerationProvider& _accel;
};

}    // namespace App

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_STATISTICS_HPP
