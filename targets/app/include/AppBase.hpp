#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_APPBASE_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_APPBASE_HPP

#include <chrono>
#include <optional>

class AppBase {
  public:
    auto run([[maybe_unused]] auto& display) const noexcept -> std::optional<std::chrono::milliseconds>
    {
        return std::nullopt;
    }
};

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_APPBASE_HPP
