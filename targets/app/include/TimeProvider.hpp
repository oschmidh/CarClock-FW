#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_TIMEPROVIDER_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_TIMEPROVIDER_HPP

#include <zephyr/drivers/rtc.h>
#include <zephyr/devicetree.h>

struct Time {
    unsigned int hours{};
    unsigned int minutes{};
    unsigned int seconds{};
};

struct Date {
    unsigned int wday = 1;    // TODO??
    unsigned int day = 1;
    unsigned int month = 1;
    unsigned int year = 1970;
};

class TimeProvider {
  public:
    bool init() const noexcept
    {
        // const rtc_time now{.tm_sec = 0,
        //                    .tm_min = 11,
        //                    .tm_hour = 0,
        //                    .tm_mday = 31,
        //                    .tm_mon = 1,
        //                    .tm_year = 2026 - 1900,
        //                    .tm_wday = 6,
        //                    .tm_yday = 31,
        //                    .tm_isdst = false};
        // rtc_set_time(_dev, &now);

        return true;
    }

    Time getTime() const noexcept
    {
        const auto tm = getDataTime();
        return {.hours = tm.tm_hour, .minutes = tm.tm_min, .seconds = tm.tm_sec};
    }

    Date getDate() const noexcept
    {
        const auto tm = getDataTime();
        return {.wday = tm.tm_wday, .day = tm.tm_mday, .month = tm.tm_mon, .year = tm.tm_year};
    }

  private:
    rtc_time getDataTime() const noexcept
    {
        rtc_time tm;
        rtc_get_time(_dev, &tm);
        return tm;
    }

    static constexpr const device* const _dev = DEVICE_DT_GET(DT_CHOSEN(rtc));
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_TIMEPROVIDER_HPP
