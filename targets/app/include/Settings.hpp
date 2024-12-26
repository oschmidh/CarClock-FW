#ifndef CARCLOCK_FW_TARGETS_APP_INCLUDE_SETTINGS_HPP
#define CARCLOCK_FW_TARGETS_APP_INCLUDE_SETTINGS_HPP

#include <chrono>
#include <optional>

namespace App {

template <typename DISPLAY_T>
class Settings {
  public:
    auto run() const noexcept -> std::optional<std::chrono::milliseconds> { return std::nullopt; }
};

}    // namespace App

#endif    // CARCLOCK_FW_TARGETS_APP_INCLUDE_SETTINGS_HPP
