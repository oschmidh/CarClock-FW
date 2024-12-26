#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP

#include <zephyr/device.h>

class Display {
  public:
    Display(const device* const dev) noexcept
     : _dev(dev)
    { }

    bool init() noexcept
    {
        if (!device_is_ready(_dev)) {
            // LOG_ERR("Display not ready");
            return false;
        }
        return true;
    }

    void update() noexcept { }

  private:
    const device* const _dev;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP
