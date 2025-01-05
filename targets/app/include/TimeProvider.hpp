#ifndef CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_TIMEPROVIDER_H
#define CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_TIMEPROVIDER_H

struct Time {
    unsigned int hours{};
    unsigned int minutes{};
    unsigned int seconds{};
};

class TimeProvider {
  public:
    bool init() const noexcept { return true; }
    Time getTime() const noexcept { return {}; }

  private:
};

#endif    // CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_TIMEPROVIDER_H
