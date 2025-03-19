#include "AppLauncher.hpp"
#include "HomeScreen.hpp"
#include "Settings.hpp"

#include "ProviderManager.hpp"
#include "SettingsProvider.hpp"
#include "TemperatureProvider.hpp"
#include "TimeProvider.hpp"

#include "Framebuffer.hpp"
#include "Display.hpp"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>

#include <tuple>

LOG_MODULE_REGISTER(app, CONFIG_LOG_DEFAULT_LEVEL);

// TODO color depth hardcoded:
using FramebufferType = Framebuffer<DT_PROP(DT_CHOSEN(display), width), DT_PROP(DT_CHOSEN(display), height), 4>;
FramebufferType fb;

int main()
{
    using ProviderList = std::tuple<SettingsProvider, TemperatureProvider, TimeProvider>;
    ProviderManager<ProviderList> providers;

    providers.init();

    static constexpr const device* const dispDev = DEVICE_DT_GET(DT_CHOSEN(display));
    Display display(dispDev);
    display.init();

    using AppList = std::tuple<App::HomeScreen, App::Settings>;
    auto launcher = createAppLauncher<AppList>(fb, providers);

    // bootAnimation(display); // TODO implement

    launcher.run();

    while (1) {
        display.update(fb);    // TODO move to UI thread
        k_sleep(K_MSEC(10));
    }

    return 0;
}
