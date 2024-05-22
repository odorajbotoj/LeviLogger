#pragma once

#include "levi_logger/Config.h"
#include "levi_logger/logger/Logger.h"

#include "ll/api/plugin/NativePlugin.h"

extern Config                      config;
extern levi_logger::logger::Logger fileLogger;

namespace levi_logger {

class LeviLogger {

public:
    static LeviLogger& getInstance();

    LeviLogger(ll::plugin::NativePlugin& self) : mSelf(self) {}

    [[nodiscard]] ll::plugin::NativePlugin& getSelf() const { return mSelf; }

    /// @return True if the plugin is loaded successfully.
    bool load();

    /// @return True if the plugin is enabled successfully.
    bool enable();

    /// @return True if the plugin is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the plugin.
    // /// @return True if the plugin is unloaded successfully.
    // bool unload();

private:
    ll::plugin::NativePlugin& mSelf;
};

} // namespace levi_logger
