#ifndef CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_TIMEPROVIDER_H
#define CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_TIMEPROVIDER_H

struct Time { };

class TimeProvider {
  public:
    Time getTime() const noexcept { return {}; }

  private:
};

#endif    // CARINFOTAINMENTSYSTEM_FW_APP_INCLUDE_TIMEPROVIDER_H
