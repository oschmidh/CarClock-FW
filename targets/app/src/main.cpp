
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>

#include <tuple>

LOG_MODULE_REGISTER(app, CONFIG_LOG_DEFAULT_LEVEL);

int main()
{

    using ProviderList = std::tuple<SettingsProvider, TemperatureProvider, TimeProvider>;
    ProviderList providers;

    providers.init();

    using AppList = std::tuple<Homescreen, Settings>;
    AppLauncher<AppList> launcher(providers);

    static constexpr const device* const dispDev = DEVICE_DT_GET(DT_ALIAS(display));
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Display not ready");
        return -ENODEV;
    }
    Display display(dispDev);
    display.init();

    bootAnimation(display);

    launcher.run(display);

    while (1) {
        display.update();
        k_msleep(K_MSEC(10));
    }

    return 0;
}
