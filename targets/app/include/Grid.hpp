#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_GRID_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_GRID_HPP

#include <tuple>

template <unsigned int COLS_V, unsigned int ROWS_V, typename... WIDGET_Ts>
class Grid {
    static constexpr unsigned int width = 200;    // TODO hardcoded
    static constexpr unsigned int height = 13;    // TODO hardcoded

  public:
    template <typename... Ts>
    Grid(Ts&&... args) noexcept
     : _widgets(std::forward<Ts>(args)...)
    { }

    constexpr void setPos(Point pos) noexcept
    {

        for (unsigned int c = 0; c < COLS_V; ++c) {
            for (unsigned int r = 0; r < ROWS_V; ++r) {
                const auto widgetIdx = c * ROWS_V + r;
                if (widgetIdx >= sizeof...(WIDGET_Ts)) {
                    return;
                }
                setWidgetPos(widgetIdx, pos + Point{r * width / ROWS_V, c * height / COLS_V});    // TODO col and
                                                                                                  // row-width
                                                                                                  // hardcoded
            }
        }
    }

    void draw(auto& display) noexcept
    {

        std::apply([&display](auto&... w) { (w.draw(display), ...); }, _widgets);
    }

  private:
    static_assert(sizeof...(WIDGET_Ts) <= COLS_V * ROWS_V);

    template <unsigned int N = sizeof...(WIDGET_Ts) - 1>
    void setWidgetPos(unsigned int idx, Point pos) const noexcept
    {
        if (idx == N) {
            return std::get<N>(_widgets).setPos(pos);
        }

        if constexpr (N != 0) {
            return setWidgetPos<N - 1>(idx, pos);
        }

        // TODO assert? should never happen
    }

    std::tuple<WIDGET_Ts...> _widgets;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_GRID_HPP
