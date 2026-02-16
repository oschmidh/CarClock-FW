#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_ACCELERATIONPROVIDER_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_ACCELERATIONPROVIDER_HPP

#include <expected>

class AccelerationProvider {
  public:
    struct Acceleration {
        int x;
        int y;
        int z;
    };

    bool init() const noexcept { return true; }

    // TODO add actual error codes?
    std::expected<Acceleration, int> get() const noexcept
    {
        static unsigned int pos = 0;

        return testPattern[pos++];    // TODO test values
        // return {0, 0, 1};
    }

    static constexpr Acceleration testPattern[] = {
        {0, 0, 0},         {500, 0, 0},      {700, 200, 0},    {1300, -20, 0}, {1900, 0, 0},   {-1500, -200, 0},
        {-1600, -1300, 0}, {-900, -1500, 0}, {-300, -1650, 0}, {200, -850, 0}, {300, -200, 0}, {0, 0, 0},
    };    // TODO test values
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_ACCELERATIONPROVIDER_HPP
