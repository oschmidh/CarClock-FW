#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_GRID_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_GRID_HPP

#include <tuple>

template <unsigned int COLS_V, unsigned int ROWS_V, typename... WIDGET_Ts>
class Grid {
    static constexpr unsigned int width = 200;    // TODO hardcoded
    static constexpr unsigned int height = 13;    // TODO hardcoded

  public:
    // Grid(Point pos, auto&&... args) noexcept
    //  : _pos(pos)
    //  , _widgets(args...)
    // { }

    template <typename... Ts>
    Grid(Ts&&... args) noexcept
     : _widgets(std::forward<Ts>(args)...)
    {
        printk("widget 0 addr: %p\n", &std::get<0>(_widgets));
        printk("widget 1 addr: %p\n", &std::get<1>(_widgets));
        printk("widget 2 addr: %p\n", &std::get<2>(_widgets));
    }

    // template <typename... Ts>
    // Grid(Ts&... args) noexcept
    //  : _widgets(args...)
    // {
    //     printk("widget 0 addr: %p\n", &std::get<0>(_widgets));
    //     printk("widget 1 addr: %p\n", &std::get<1>(_widgets));
    //     printk("widget 2 addr: %p\n", &std::get<2>(_widgets));
    // }

    constexpr void setPos(Point pos) noexcept
    {
        printk("widget 0 addr: %p\n", &std::get<0>(_widgets));
        printk("widget 1 addr: %p\n", &std::get<1>(_widgets));
        printk("widget 2 addr: %p\n", &std::get<2>(_widgets));

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
        // [this]<std::size_t N>(std::index_sequence<N>) noexcept { (std::get<I>(_widgets).draw(), ...); }(
        //     std::make_index_sequence<sizeof...(WIDGET_Ts)>);

        std::apply([&display](auto&... w) { (w.draw(display), ...); }, _widgets);
    }

  private:
    static_assert(sizeof...(WIDGET_Ts) <= COLS_V * ROWS_V);

    template <unsigned int N = sizeof...(WIDGET_Ts) - 1>
    void setWidgetPos(unsigned int idx, Point pos) const noexcept
    {
        printk("grid: setWidgetPos<%d>(%d, (%d|%d))\n", N, idx, pos.x, pos.y);
        if (idx == N) {
            // printk("setting widget %d pos: (%d|%d)\n", idx, pos.x, pos.y);
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
