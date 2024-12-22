#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_TIMEPROVIDER_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_TIMEPROVIDER_HPP

struct Time { };

class TimeProvider {
  public:
    Time getTime() const noexcept { return {}; }

  private:
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_TIMEPROVIDER_HPP
