#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/devicetree.h>

class Display {
  public:
    Display(const device* const dev) noexcept
     : _dev(dev)
    { }

    bool init() noexcept
    {
        if (!device_is_ready(_dev)) {
            printk("Display not ready\n");
            // LOG_ERR("Display not ready");
            return false;
        }

        if (display_set_pixel_format(_dev, PIXEL_FORMAT_MONO01) != 0) {
            // printk("Failed to set required pixel format\n");
            // LOG_ERR("Failed to set required pixel format");  // TODO
            return false;
        }

        // display_set_contrast(_dev, 0xff);

        // display_blanking_off(_dev);
        return true;
    }

    void update(const auto& framebuf) noexcept
    {
        // TODO split the framebuffer into tiles and only update the modified tiles

        const display_buffer_descriptor desc{.buf_size = framebuf.size(),
                                             .width = framebuf.width(),
                                             .height = framebuf.height(),
                                             .pitch = framebuf.width()};
        if (const auto err = display_write(_dev, 0, 0, &desc, framebuf.data()); err < 0) {
            printk("failed to write, %d\n", err);
        }
    }

  private:
    const device* const _dev;
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_DISPLAY_HPP
