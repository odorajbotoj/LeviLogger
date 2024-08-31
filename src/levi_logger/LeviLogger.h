#pragma once

#include "levi_logger/Config.h"
#include "levi_logger/logger/Logger.h"

#include "ll/api/mod/NativeMod.h"

extern Config                      config;
extern levi_logger::logger::Logger fileLogger;

namespace levi_logger {

class LeviLogger {

public:
    static LeviLogger& getInstance();

    LeviLogger(ll::mod::NativeMod& self) : mSelf(self) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the mod.
    // /// @return True if the mod is unloaded successfully.
    // bool unload();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace levi_logger
