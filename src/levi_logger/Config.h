#pragma once

#include <string>
#include <vector>

struct EventConfigStruct {
    bool                     log             = true;
    bool                     print           = false;
    std::vector<std::string> noOutputContent = {};
    std::string              printFormat     = "";
};

struct Config {
    int                    version                  = 2;
    std::string            locateName               = "zh_CN";
    unsigned long long int maxLine                  = 50000;
    EventConfigStruct      playerAddExperienceEvent = {};
    EventConfigStruct      playerAttackEvent        = {};
    EventConfigStruct      playerChangePermEvent    = {};
    EventConfigStruct playerChatEvent    = {.print = true, .printFormat = "<{{self}}> ({{x}}, {{y}}, {{z}}) {{info}}"};
    EventConfigStruct playerConnectEvent = {};
    EventConfigStruct playerDestroyBlockEvent  = {};
    EventConfigStruct playerDieEvent           = {};
    EventConfigStruct playerInteractBlockEvent = {};
    EventConfigStruct playerJoinEvent  = {.print = true, .printFormat = "<{{self}}> ({{x}}, {{y}}, {{z}}) {{event}}"};
    EventConfigStruct playerJumpEvent  = {.log = false};
    EventConfigStruct playerLeaveEvent = {};
    EventConfigStruct playerPickUpItemEvent     = {};
    EventConfigStruct playerPlacingBlockEvent   = {};
    EventConfigStruct playerPlacedBlockEvent    = {};
    EventConfigStruct playerRespawnEvent        = {};
    EventConfigStruct playerSneakingEvent       = {.log = false};
    EventConfigStruct playerSneakedEvent        = {.log = false};
    EventConfigStruct playerSprintingEvent      = {.log = false};
    EventConfigStruct playerSprintedEvent       = {.log = false};
    EventConfigStruct playerSwingEvent          = {.log = false};
    EventConfigStruct playerUseItemEvent        = {};
    EventConfigStruct playerUseItemOnEvent      = {};
    EventConfigStruct useFrameBlockEvent        = {};
    EventConfigStruct entityExplodeEvent        = {};
    EventConfigStruct blockExplodeEvent         = {};
    EventConfigStruct respawnAnchorExplodeEvent = {};
    EventConfigStruct blockExplodedEvent        = {};
    EventConfigStruct commandBlockExecuteEvent  = {
         .print       = true,
         .printFormat = "<{{self}}> ({{x}}, {{y}}, {{z}}) {{info}}"
    };
    EventConfigStruct projectileHitBlockEvent     = {.log = false};
    EventConfigStruct mobDieEvent                 = {.log = false};
    EventConfigStruct actorHurtEvent              = {.log = false};
    EventConfigStruct projectileHitEntityEvent    = {.log = false};
    EventConfigStruct witherDestroyEvent          = {.log = false};
    EventConfigStruct pistonPushEvent             = {.log = false};
    EventConfigStruct farmDecayEvent              = {};
    EventConfigStruct playerDropItemEvent         = {};
    EventConfigStruct playerConsumeTotemEvent     = {};
    EventConfigStruct playerChangeSlotEvent       = {};
    EventConfigStruct playerSetArmorEvent         = {};
    EventConfigStruct playerUseRespawnAnchorEvent = {};
    EventConfigStruct playerPullFishingHookEvent  = {};
    EventConfigStruct playerSleepEvent            = {};
    EventConfigStruct executingCommandEvent       = {
              .print       = true,
              .printFormat = "<{{self}}> ({{x}}, {{y}}, {{z}}) {{info}}"
    };
    EventConfigStruct playerChangeDimension = {
        .print       = true,
        .printFormat = "<{{self}}> ({{x}}, {{y}}, {{z}}) {{event}} {{info}}"
    };
};