#ifndef CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_APPLAUNCHER_H
#define CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_APPLAUNCHER_H

#include <variant>

template <typename... APP_Ts>
class AppLauncher {
  public:
    void onEvent();

    void run() noexcept {
        // TODO make thread and run app in thread
        std::visit([](auto&& app) { app.run(); }, _apps)
    };

  private:
    ProviderList& _providers;
    std::variant<APP_Ts...> _apps;
};

#endif    // CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_APPLAUNCHER_H
