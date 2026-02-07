#ifndef CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_TEMPERATUREPROVIDER_H
#define CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_TEMPERATUREPROVIDER_H

#include <expected>

class TemperatureProvider {
  public:
    enum class Sensors { Inside, Outside };

    bool init() const noexcept { return true; }

    // TODO add actual error codes?
    std::expected<int, int> get(Sensors sensor) const noexcept
    {
        switch (sensor) {
            case Sensors::Inside: return 215;
            case Sensors::Outside: [[fallthrough]];
            default: return std::unexpected(-1);
        }
    }    // deci-degree celsius
};

#endif    // CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_TEMPERATUREPROVIDER_H
