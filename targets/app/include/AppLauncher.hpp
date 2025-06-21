#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_APPLAUNCHER_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_APPLAUNCHER_HPP

#include "Settings.hpp"    // TEST

#include <pw_sync/binary_semaphore.h>
#include <pw_thread/thread.h>
#include <pw_thread_zephyr/config.h>
#include <pw_thread_zephyr/context.h>
#include <pw_thread_zephyr/options.h>

#include <variant>
#include <tuple>

pw::thread::zephyr::StaticContextWithStack<1024> threadContext;    // TODO define stackSize in kconfig

template <typename, typename, typename>
class AppLauncher;

template <typename... APP_Ts, typename DISPLAY_T, typename PROVIDER_MANAGER_T>
class AppLauncher<std::tuple<APP_Ts...>, DISPLAY_T, PROVIDER_MANAGER_T> {
  public:
    AppLauncher(DISPLAY_T& display, PROVIDER_MANAGER_T& providers) noexcept
     : _display(display)
     , _providers(providers)
    { }

    void onEvent() { }

    template <typename APP_T>
    void switchApp() noexcept
    {
        _stop.release();
        _exited.acquire();
        _apps.template emplace<APP_T>(_display, _providers);
        _start.release();
    }

    void run() noexcept
    {
        static constexpr auto options =
            pw::thread::zephyr::Options(threadContext).set_priority(pw::thread::zephyr::config::kDefaultPriority);

        pw::thread::Thread thread(options, [this]() {
            while (1) {
                std::optional delay = std::chrono::milliseconds{};
                do {
                    delay = std::visit([this](auto& app) { return app.run(_display); }, _apps);
                } while (delay.has_value() && !_stop.try_acquire_for(delay.value()));

                _exited.release();
                _start.acquire();
            }
        });
        thread.detach();
    };

  private:
    DISPLAY_T& _display;
    PROVIDER_MANAGER_T& _providers;
    pw::sync::BinarySemaphore _start{};
    pw::sync::BinarySemaphore _stop{};
    pw::sync::BinarySemaphore _exited{};
    std::variant<APP_Ts...> _apps{std::in_place_type<std::tuple_element_t<0, std::tuple<APP_Ts...>>>, _display,
                                  _providers};
};

template <typename APP_TUPLE_T, typename DISPLAY_T, typename PROVIDER_T>
constexpr auto createAppLauncher(DISPLAY_T& disp, PROVIDER_T& provider) noexcept
    -> AppLauncher<APP_TUPLE_T, DISPLAY_T, PROVIDER_T>
{
    return AppLauncher<APP_TUPLE_T, DISPLAY_T, PROVIDER_T>(disp, provider);
}

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_APPLAUNCHER_HPP
