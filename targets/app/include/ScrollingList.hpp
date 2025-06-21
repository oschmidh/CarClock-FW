#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_SCROLLINGLIST_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_SCROLLINGLIST_HPP

#include <Primitives.hpp>
#include <Font.hpp>

#include <string_view>
#include <cstdint>

template <typename... ITEM_Ts>
class ScrollingList {
    static constexpr unsigned int sideOffset = 6;
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

    // template <typename... Ts>
    // ScrollingList(Point pos, Ts&... args) noexcept
    //  : _pos(pos)
    //  , _items(args...)
    // { }

    static constexpr std::size_t numItems() noexcept { return sizeof...(ITEM_Ts); }

    void draw(auto& display) noexcept
    {
        printk("scrl: widget 0 addr: %p\n", &std::get<0>(_items));

        display.clear({_pos, width, height});
        // printk("scrollingList draw\n");
        const Point rowOffset = {0, itemHeight};

        for (unsigned int i = 0; i < numRows; ++i) {
            display.drawHLine(_pos + rowOffset * i, width);

            const auto itemIdx = _scrlIndex + i;
            // setItemPos(itemIdx, _pos + rowOffset * i + Point{0, 1});
            const auto itPos = _pos + rowOffset * i + Point{0, 1};
            // printk("itPos: (%d|%d)\n", itPos.x, itPos.y);
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

        if (_selIndex > _scrlIndex + numRows) {
            ++_scrlIndex;
        }
        // TODO implement
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
        // TODO implement
    }

  private:
    // template <unsigned int N = numItems() - 1>
    // struct ItemAccessor {
    //     static void setPos(unsigned int idx, Point pos) noexcept
    //     {
    //         if (idx == N) {
    //             std::get<N>(_items).setPos(pos);
    //             return;
    //         }

    //         invoke<N - 1>(idx, pos);
    //     }

    //     static void draw(unsigned int idx, auto& display) noexcept
    //     {
    //         if (idx == N) {
    //             std::get<N>(_items).draw(display);
    //             return;
    //         }

    //         invoke<N - 1>(idx, pos);
    //     }
    // };

    // template <>
    // struct ItemAccessor<0> {
    //     static void setPos(unsigned int idx, Point pos) noexcept
    //     {
    //         if (idx == 0) {
    //             std::get<0>(_items).draw(pos);
    //         }
    //         return;    // TODO assert? should never happen
    //     }

    //     static void draw(unsigned int idx, auto& display) noexcept
    //     {
    //         if (idx == 0) {
    //             std::get<0>(_items).draw(display);
    //         }
    //         return;    // TODO assert? should never happen
    //     }
    // };

    // void setItemPos(unsigned int idx, Point pos) const noexcept    // TODO wrappers necessary?
    // {
    //     ItemAccessor::setPos(idx, pos);
    // }

    // void drawItem(unsigned int idx, auto& display) const noexcept    // TODO wrappers necessary?
    // {
    //     ItemAccessor::draw(idx, display);
    // }

    template <unsigned int N = numItems() - 1>
    void setItemPos(unsigned int idx, Point pos) noexcept
    {
        printk("scrl: setItemPos<%d>(%d, (%d|%d))\n", N, idx, pos.x, pos.y);
        if (idx == N) {
            return std::get<N>(_items).setPos(pos);
        }

        if constexpr (N != 0) {
            return setItemPos<N - 1>(idx, pos);
        }

        // TODO assert? should never happen
    }

    // template <>
    // void setItemPos<0>(unsigned int idx, Point pos) const noexcept
    // {
    //     static_assert(N < numItems());
    //     if (idx == N) {
    //         return std::get<N>(_items).setPos(pos);
    //     }

    //     return;    // TODO assert? should never happen
    // }

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

    // template <>
    // void drawItem<0>(unsigned int idx, auto& display) const noexcept
    // {
    //     static_assert(N < numItems());
    //     if (idx == N) {
    //         return std::get<N>(_items).draw(display);
    //     }

    //     return;    // TODO assert? should never happen
    // }

    unsigned int _selIndex{};
    unsigned int _scrlIndex{};
    Point _pos;
    std::tuple<ITEM_Ts...> _items;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_SCROLLINGLIST_HPP
