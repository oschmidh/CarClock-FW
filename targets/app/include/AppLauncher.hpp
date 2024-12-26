#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_APPLAUNCHER_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_APPLAUNCHER_HPP

#include <pw_sync/binary_semaphore.h>
#include <pw_thread/thread.h>
#include <pw_thread_zephyr/config.h>
#include <pw_thread_zephyr/context.h>
#include <pw_thread_zephyr/options.h>

#include <variant>
#include <tuple>

pw::thread::zephyr::StaticContextWithStack<512> threadContext;    // TODO define stackSize in kconfig

template <typename DISPLAY_T, typename PROVIDER_MANAGER_T, template <typename> typename... APP_Ts>
class AppLauncher {
  public:
    AppLauncher(DISPLAY_T& display, PROVIDER_MANAGER_T& providers) noexcept
     : _display(display)
     , _providers(providers)
    { }

    void onEvent() { }

    template <typename APP_T>
    void swichApp() noexcept
    {
        _stop.release();
        _exited.acquire();
        _apps.emplace(APP_T::create(_display, _providers));
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
                    delay = std::visit([](auto& app) { return app.run(); }, _apps);
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
    std::variant<APP_Ts<DISPLAY_T>...> _apps{
        std::tuple_element_t<0, std::tuple<APP_Ts<DISPLAY_T>...>>::create(_display, _providers)};
};

#endif    // CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_APPLAUNCHER_H
