#pragma once

#include <string>
#include <vector>

namespace {
struct EventStruct {
    bool                     log             = true;
    std::vector<std::string> noOutputContent = {};
};
} // namespace

struct Config {
    int                    version                  = 1;
    std::string            locateName               = "zh_CN";
    unsigned long long int maxLine                  = 2000;
    ::EventStruct          playerAddExperienceEvent = {};
    ::EventStruct          playerAttackEvent        = {};
    ::EventStruct          playerChangePermEvent    = {};
    ::EventStruct          playerChatEvent          = {};
    ::EventStruct          playerConnectEvent       = {};
    ::EventStruct          playerDestroyBlockEvent  = {};
    ::EventStruct          playerDieEvent           = {};
    ::EventStruct          playerInteractBlockEvent = {};
    ::EventStruct          playerJoinEvent          = {};
    ::EventStruct          playerJumpEvent          = {false};
    ::EventStruct          playerLeaveEvent         = {};
    ::EventStruct          playerPickUpItemEvent    = {};
    ::EventStruct          playerPlacingBlockEvent  = {};
    ::EventStruct          playerPlacedBlockEvent   = {};
    ::EventStruct          playerRespawnEvent       = {};
    ::EventStruct          playerSneakingEvent      = {false};
    ::EventStruct          playerSneakedEvent       = {false};
    ::EventStruct          playerSprintingEvent     = {false};
    ::EventStruct          playerSprintedEvent      = {false};
    ::EventStruct          playerSwingEvent         = {false};
    ::EventStruct          playerUseItemEvent       = {};
    ::EventStruct          playerUseItemOnEvent     = {};
};