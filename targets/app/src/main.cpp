#include "AppLauncher.hpp"
#include "HomeScreen.hpp"
#include "Settings.hpp"

#include "ProviderManager.hpp"
#include "SettingsProvider.hpp"
#include "TemperatureProvider.hpp"
#include "TimeProvider.hpp"

#include "Display.hpp"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>

#include <tuple>

LOG_MODULE_REGISTER(app, CONFIG_LOG_DEFAULT_LEVEL);

DT_DISPLAY_FRAMEBUF_DEFINE(frameBuf, DT_CHOSEN(display));

int main()
{
    using ProviderList = std::tuple<SettingsProvider, TemperatureProvider, TimeProvider>;
    ProviderManager<ProviderList> providers;

    providers.init();

    static constexpr const device* const dispDev = DEVICE_DT_GET(DT_CHOSEN(display));
    Display display(dispDev, frameBuf);
    display.init();

    using AppList = std::tuple<App::HomeScreen, App::Settings>;
    auto launcher = createAppLauncher<AppList>(display, providers);

    // bootAnimation(display); // TODO implement

    launcher.run();

    while (1) {
        display.update();    // TODO move to UI thread
        k_sleep(K_MSEC(10));
    }

    return 0;
}
