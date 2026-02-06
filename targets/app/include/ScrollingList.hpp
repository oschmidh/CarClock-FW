#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_SCROLLINGLIST_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_SCROLLINGLIST_HPP

#include <Primitives.hpp>
#include <Font.hpp>

#include <string_view>
#include <cstdint>

template <typename... ITEM_Ts>
class ScrollingList {
    static constexpr unsigned int sideOffset = 6;
    static constexpr unsigned int sideItemOffset = 1;
    static constexpr unsigned int topOffset = 2;
    static constexpr unsigned int itemHeight = 14;
    // static constexpr unsigned int height = 62;                      // TODO should be settable
    // static constexpr unsigned int width = 213;                      // TODO should be settable
    // static constexpr unsigned int numRows = height / itemHeight;    // visible rows

  public:
    template <typename... Ts>
    constexpr ScrollingList(Point pos, unsigned int width, unsigned int height, Ts&&... args) noexcept
     : _pos(pos)
     , _items(std::forward<Ts>(args)...)
    {
        resize(width, height);
    }

    static constexpr std::size_t numItems() noexcept { return sizeof...(ITEM_Ts); }

    void draw(auto& display) noexcept
    {

        display.clear({_pos, _width, _height});
        const Point rowOffset = {0, itemHeight};

        for (unsigned int i = 0; i < numRows(); ++i) {
            display.drawHLine(_pos + rowOffset * i, _width);

            const auto itemIdx = _scrlIndex + i;
            // setItemPos(itemIdx, _pos + rowOffset * i + Point{0, 1});
            const auto itPos = _pos + rowOffset * i + Point{sideItemOffset, 1};
            setItemPos(itemIdx, itPos);
            drawItem(itemIdx, display);

            if (itemIdx == _selIndex) {
                // TODO adjust how selection is highlighted
                display.invert({_pos + rowOffset * i, _width, itemHeight});
            }
        }
    }

    void selDown() noexcept
    {
        if (_selIndex >= numItems() - 1) {
            return;
        }
        ++_selIndex;

        if (_selIndex >= _scrlIndex + numRows()) {
            ++_scrlIndex;
        }
    }

    void selUp() noexcept
    {
        if (_selIndex <= 0) {
            return;
        }
        --_selIndex;

        if (_selIndex < _scrlIndex) {
            --_scrlIndex;
        }
    }

    constexpr void resize(unsigned int width, unsigned int height) noexcept
    {
        _width = width;
        _height = height;
        const unsigned int widgetWidth = width - (sideItemOffset * 2);
        const unsigned int widgetHeight = itemHeight;    // TODO hardcoded for now
        // std::apply([widgetWidth, widgetHeight](auto& widget) { widget.resize(widgetWidth, widgetHeight); }, _items);
        std::apply([widgetWidth, widgetHeight](auto&... widgets) { (widgets.resize(widgetWidth, widgetHeight), ...); },
                   _items);
    }

  private:
    // template

    constexpr unsigned int numRows() const noexcept { return _height / itemHeight; }    // visible rows

    template <unsigned int N = numItems() - 1>
    void setItemPos(unsigned int idx, Point pos) noexcept
    {
        if (idx == N) {
            return std::get<N>(_items).setPos(pos);
        }

        if constexpr (N != 0) {
            return setItemPos<N - 1>(idx, pos);
        }

        // TODO assert? should never happen
    }

    template <unsigned int N = numItems() - 1>
    void drawItem(unsigned int idx, auto& display) const noexcept
    {
        if (idx == N) {
            return std::get<N>(_items).draw(display);
        }

        if constexpr (N != 0) {
            return drawItem<N - 1>(idx, display);
        }

        // TODO assert? should never happen
    }

    int _width{};
    int _height{};
    unsigned int _selIndex{};
    unsigned int _scrlIndex{};
    Point _pos;
    std::tuple<ITEM_Ts...> _items;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_SCROLLINGLIST_HPP
