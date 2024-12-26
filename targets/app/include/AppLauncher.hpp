#ifndef CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_APPLAUNCHER_H
#define CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_APPLAUNCHER_H

#include <variant>
#include <tuple>

template <typename DISPLAY_T, typename PROVIDER_MANAGER_T, template <typename> typename... APP_Ts>
class AppLauncher {
  public:
    AppLauncher(DISPLAY_T& display, PROVIDER_MANAGER_T& providers) noexcept
     : _display(display)
     , _providers(providers)
    { }

    void onEvent() { }

    void run() noexcept
    {
        // TODO make thread and run app in thread

        std::visit([](auto&& app) { app.run(); }, _apps);
    }

  private:
    DISPLAY_T& _display;
    PROVIDER_MANAGER_T& _providers;
    std::variant<APP_Ts<DISPLAY_T>...> _apps{
        std::tuple_element_t<0, std::tuple<APP_Ts<DISPLAY_T>...>>::create(_display, _providers)};
};

#endif    // CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_APPLAUNCHER_H
