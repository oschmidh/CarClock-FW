#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_PROVIDERMANAGER_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_PROVIDERMANAGER_HPP

#include <tuple>

template <typename>
class ProviderManager;

template <typename... PROVIDER_Ts>
class ProviderManager<std::tuple<PROVIDER_Ts...>> {
  public:
    bool init() noexcept
    {
        return std::apply([](PROVIDER_Ts&... p) { return (p.init() && ...); }, _providers);
    };

    template <typename T>
    constexpr T& get() noexcept
    {
        return std::get<T>(_providers);
    }

    template <typename T>
    constexpr const T& get() const noexcept
    {
        return const_cast<const T&>(get<T>());
    }

  private:
    std::tuple<PROVIDER_Ts...> _providers{};
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_PROVIDERMANAGER_HPP
