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
    static constexpr unsigned int height = 62;                      // TODO should be settable
    static constexpr unsigned int width = 213;                      // TODO should be settable
    static constexpr unsigned int numRows = height / itemHeight;    // visible rows

  public:
    template <typename... Ts>
    ScrollingList(Point pos, Ts&&... args) noexcept
     : _pos(pos)
     , _items(std::forward<Ts>(args)...)
    { }

    static constexpr std::size_t numItems() noexcept { return sizeof...(ITEM_Ts); }

    void draw(auto& display) noexcept
    {

        display.clear({_pos, width, height});
        const Point rowOffset = {0, itemHeight};

        for (unsigned int i = 0; i < numRows; ++i) {
            display.drawHLine(_pos + rowOffset * i, width);

            const auto itemIdx = _scrlIndex + i;
            // setItemPos(itemIdx, _pos + rowOffset * i + Point{0, 1});
            const auto itPos = _pos + rowOffset * i + Point{sideItemOffset, 1};
            setItemPos(itemIdx, itPos);
            drawItem(itemIdx, display);

            if (itemIdx == _selIndex) {
                // TODO adjust how selection is highlighted
                display.invert({_pos + rowOffset * i, width, itemHeight});
            }
        }
    }

    void selDown() noexcept
    {
        if (_selIndex >= numItems() - 1) {
            return;
        }
        ++_selIndex;

        if (_selIndex >= _scrlIndex + numRows) {
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

  private:
    // template

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

    unsigned int _selIndex{};
    unsigned int _scrlIndex{};
    Point _pos;
    std::tuple<ITEM_Ts...> _items;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_SCROLLINGLIST_HPP
