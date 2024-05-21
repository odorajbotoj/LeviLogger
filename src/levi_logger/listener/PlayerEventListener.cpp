#include "levi_logger/Config.h"
#include "levi_logger/listener/Listener.h"
#include "levi_logger/logger/Logger.h"

#include "ll/api/i18n/I18n.h"
#include "ll/api/utils/WinUtils.h"

#include "ll/api/event/EventBus.h"
#include "ll/api/event/ListenerBase.h"

#include "ll/api/event/player/PlayerAddExperienceEvent.h"
#include "ll/api/event/player/PlayerAttackEvent.h"
#include "ll/api/event/player/PlayerChangePermEvent.h"
#include "ll/api/event/player/PlayerChatEvent.h"
#include "ll/api/event/player/PlayerConnectEvent.h"
#include "ll/api/event/player/PlayerDestroyBlockEvent.h"
#include "ll/api/event/player/PlayerDieEvent.h"
#include "ll/api/event/player/PlayerInteractBlockEvent.h"
#include "ll/api/event/player/PlayerJoinEvent.h"
#include "ll/api/event/player/PlayerJumpEvent.h"
#include "ll/api/event/player/PlayerLeaveEvent.h"
#include "ll/api/event/player/PlayerPickUpItemEvent.h"
#include "ll/api/event/player/PlayerPlaceBlockEvent.h"
#include "ll/api/event/player/PlayerRespawnEvent.h"
#include "ll/api/event/player/PlayerSneakEvent.h"
#include "ll/api/event/player/PlayerSprintEvent.h"
#include "ll/api/event/player/PlayerSwingEvent.h"
#include "ll/api/event/player/PlayerUseItemEvent.h"
#include "ll/api/event/player/PlayerUseItemOnEvent.h"

#include "mc/server/commands/CommandPermissionLevel.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/dimension/Dimension.h"

#include <string>

namespace {
ll::event::ListenerPtr playerAddExperienceEventListener;
ll::event::ListenerPtr playerAttackEventListener;
ll::event::ListenerPtr playerChangePermEventListener;
ll::event::ListenerPtr playerChatEventListener;
ll::event::ListenerPtr playerConnectEventListener;
ll::event::ListenerPtr playerDestroyBlockEventListener;
ll::event::ListenerPtr playerDieEventListener;
ll::event::ListenerPtr playerInteractBlockEventListener;
ll::event::ListenerPtr playerJoinEventListener;
ll::event::ListenerPtr playerJumpEventListener;
ll::event::ListenerPtr playerLeaveEventListener;
ll::event::ListenerPtr playerPickUpItemEventListener;
ll::event::ListenerPtr playerPlacingBlockEventListener;
ll::event::ListenerPtr playerPlacedBlockEventListener;
ll::event::ListenerPtr playerRespawnEventListener;
ll::event::ListenerPtr playerSneakingEventListener;
ll::event::ListenerPtr playerSneakedEventListener;
ll::event::ListenerPtr playerSprintingEventListener;
ll::event::ListenerPtr playerSprintedEventListener;
ll::event::ListenerPtr playerSwingEventListener;
ll::event::ListenerPtr playerUseItemEventListener;
ll::event::ListenerPtr playerUseItemOnEventListener;
} // namespace

namespace levi_logger::listener {

void addPlayerEventListener(Config& config, levi_logger::logger::Logger& logger) {
    auto& eventBus = ll::event::EventBus::getInstance();

    if (config.playerAddExperienceEvent.log) {
        ::playerAddExperienceEventListener = eventBus.emplaceListener<ll::event::PlayerAddExperienceEvent>(
            [&logger, &config](ll::event::PlayerAddExperienceEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerAddExperienceEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerAddExperienceEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            }
        );
    }

    if (config.playerAttackEvent.log) {
        ::playerAttackEventListener =
            eventBus.emplaceListener<ll::event::PlayerAttackEvent>([&logger,
                                                                    &config](ll::event::PlayerAttackEvent& event) {
                std::pair<std::tm, int> ti        = ll::win_utils::getLocalTime();
                const auto              pebd      = getPlayerEventBaseData(event);
                auto&                   target    = event.target();
                auto                    targetPos = target.getFeetPos();
                logger.log(
                    config.locateName,
                    config.playerAttackEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerAttackEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    target.getNameTag() + "(" + target.getTypeName() + ")",
                    std::to_string(targetPos.x),
                    std::to_string(targetPos.y),
                    std::to_string(targetPos.z)
                );
            });
    }

    if (config.playerChangePermEvent.log) {
        ::playerChangePermEventListener = eventBus.emplaceListener<ll::event::PlayerChangePermEvent>(
            [&logger, &config](ll::event::PlayerChangePermEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                int                     np   = 0;
                switch (event.newPerm()) {
                case CommandPermissionLevel::Any:
                    np = 0;
                    break;
                case CommandPermissionLevel::GameDirectors:
                    np = 1;
                    break;
                case CommandPermissionLevel::Admin:
                    np = 2;
                    break;
                case CommandPermissionLevel::Host:
                    np = 3;
                    break;
                case CommandPermissionLevel::Owner:
                    np = 4;
                    break;
                case CommandPermissionLevel::Internal:
                    np = 5;
                    break;
                }
                logger.log(
                    config.locateName,
                    config.playerChangePermEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerChangePermEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    "",
                    "",
                    "",
                    "",
                    std::string{ll::i18n::getInstance()->get("log.info.newPerm", config.locateName)} + ": "
                        + std::to_string(np)
                );
            }
        );
    }

    if (config.playerChatEvent.log) {
        ::playerChatEventListener =
            eventBus.emplaceListener<ll::event::PlayerChatEvent>([&logger, &config](ll::event::PlayerChatEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerChatEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerChatEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    "",
                    "",
                    "",
                    "",
                    std::string{ll::i18n::getInstance()->get("log.info.message", config.locateName)} + ": "
                        + event.message()
                );
            });
    }

    if (config.playerConnectEvent.log) {
        ::playerConnectEventListener =
            eventBus.emplaceListener<ll::event::PlayerConnectEvent>([&logger,
                                                                     &config](ll::event::PlayerConnectEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerConnectEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerConnectEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            });
    }

    if (config.playerDestroyBlockEvent.log) {
        ::playerDestroyBlockEventListener = eventBus.emplaceListener<ll::event::PlayerDestroyBlockEvent>(
            [&logger, &config](ll::event::PlayerDestroyBlockEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                const auto              bpos = event.pos();
                const auto& block = event.self().getDimension().getBlockSourceFromMainChunkSource().getBlock(bpos);
                logger.log(
                    config.locateName,
                    config.playerDestroyBlockEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerDestroyBlockEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                    std::to_string(bpos.x),
                    std::to_string(bpos.y),
                    std::to_string(bpos.z)
                );
            }
        );
    }

    if (config.playerDieEvent.log) {
        ::playerDieEventListener =
            eventBus.emplaceListener<ll::event::PlayerDieEvent>([&logger, &config](ll::event::PlayerDieEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                const auto actor = event.self().getDimension().fetchEntity(event.source().getEntityUniqueID(), false);
                logger.log(
                    config.locateName,
                    config.playerDieEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerDieEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    "",
                    "",
                    "",
                    "",
                    actor ? std::string{ll::i18n::getInstance()->get("log.info.source", config.locateName)} + ": "
                                + actor->getNameTag() + "(" + actor->getTypeName() + ")"
                          : ""
                );
            });
    }

    if (config.playerInteractBlockEvent.log) {
        ::playerInteractBlockEventListener = eventBus.emplaceListener<ll::event::PlayerInteractBlockEvent>(
            [&logger, &config](ll::event::PlayerInteractBlockEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                const auto              bpos = event.pos();
                const auto& block = event.self().getDimension().getBlockSourceFromMainChunkSource().getBlock(bpos);
                logger.log(
                    config.locateName,
                    config.playerInteractBlockEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerInteractBlockEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                    std::to_string(bpos.x),
                    std::to_string(bpos.y),
                    std::to_string(bpos.z)
                );
            }
        );
    }

    if (config.playerJoinEvent.log) {
        ::playerJoinEventListener =
            eventBus.emplaceListener<ll::event::PlayerJoinEvent>([&logger, &config](ll::event::PlayerJoinEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerJoinEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerJoinEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            });
    }

    if (config.playerJumpEvent.log) {
        ::playerJumpEventListener =
            eventBus.emplaceListener<ll::event::PlayerJumpEvent>([&logger, &config](ll::event::PlayerJumpEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerJumpEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerJumpEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            });
    }

    if (config.playerLeaveEvent.log) {
        ::playerLeaveEventListener =
            eventBus.emplaceListener<ll::event::PlayerLeaveEvent>([&logger,
                                                                   &config](ll::event::PlayerLeaveEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerLeaveEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerLeaveEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            });
    }

    if (config.playerPickUpItemEvent.log) {
        ::playerPickUpItemEventListener = eventBus.emplaceListener<ll::event::PlayerPickUpItemEvent>(
            [&logger, &config](ll::event::PlayerPickUpItemEvent& event) {
                std::pair<std::tm, int> ti    = ll::win_utils::getLocalTime();
                const auto              pebd  = getPlayerEventBaseData(event);
                const auto&             ita   = event.itemActor();
                const auto              itafp = ita.getFeetPos();
                const auto              it    = ita.item();
                logger.log(
                    config.locateName,
                    config.playerPickUpItemEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerPickUpItemEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    it.getCustomName().empty() ? it.getName() : it.getCustomName() + "(" + it.getTypeName() + ")",
                    std::to_string(itafp.x),
                    std::to_string(itafp.y),
                    std::to_string(itafp.z)
                );
            }
        );
    }

    if (config.playerPlacingBlockEvent.log) {
        ::playerPlacingBlockEventListener = eventBus.emplaceListener<ll::event::PlayerPlacingBlockEvent>(
            [&logger, &config](ll::event::PlayerPlacingBlockEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                auto                    pos  = event.pos();
                const auto              face = event.face();
                switch (face) {
                case 0:
                    --pos.y;
                    break;
                case 1:
                    ++pos.y;
                    break;
                case 2:
                    --pos.z;
                    break;
                case 3:
                    ++pos.z;
                    break;
                case 4:
                    --pos.x;
                    break;
                case 5:
                    ++pos.x;
                    break;
                }
                const auto& block = event.self().getDimension().getBlockSourceFromMainChunkSource().getBlock(pos);
                logger.log(
                    config.locateName,
                    config.playerPlacingBlockEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerPlacingBlockEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                    std::to_string(pos.x),
                    std::to_string(pos.y),
                    std::to_string(pos.z),
                    std::string{ll::i18n::getInstance()->get("log.info.face", config.locateName)} + ": "
                        + std::string{ll::i18n::getInstance()->get("side." + std::to_string(face), config.locateName)}
                );
            }
        );
    }

    if (config.playerPlacedBlockEvent.log) {
        ::playerPlacedBlockEventListener = eventBus.emplaceListener<ll::event::PlayerPlacedBlockEvent>(
            [&logger, &config](ll::event::PlayerPlacedBlockEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                const auto              bpos = event.pos();
                const auto& block = event.self().getDimension().getBlockSourceFromMainChunkSource().getBlock(bpos);
                logger.log(
                    config.locateName,
                    config.playerPlacedBlockEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerPlacedBlockEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                    std::to_string(bpos.x),
                    std::to_string(bpos.y),
                    std::to_string(bpos.z)
                );
            }
        );
    }

    if (config.playerRespawnEvent.log) {
        ::playerRespawnEventListener =
            eventBus.emplaceListener<ll::event::PlayerRespawnEvent>([&logger,
                                                                     &config](ll::event::PlayerRespawnEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerRespawnEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerRespawnEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            });
    }

    if (config.playerSneakingEvent.log) {
        ::playerSneakingEventListener =
            eventBus.emplaceListener<ll::event::PlayerSneakingEvent>([&logger,
                                                                      &config](ll::event::PlayerSneakingEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerSneakingEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerSneakingEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            });
    }

    if (config.playerSneakedEvent.log) {
        ::playerSneakedEventListener =
            eventBus.emplaceListener<ll::event::PlayerSneakedEvent>([&logger,
                                                                     &config](ll::event::PlayerSneakedEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerSneakedEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerSneakedEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            });
    }

    if (config.playerSprintingEvent.log) {
        ::playerSprintingEventListener = eventBus.emplaceListener<ll::event::PlayerSprintingEvent>(
            [&logger, &config](ll::event::PlayerSprintingEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerSprintingEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerSprintingEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            }
        );
    }

    if (config.playerSprintedEvent.log) {
        ::playerSprintedEventListener =
            eventBus.emplaceListener<ll::event::PlayerSprintedEvent>([&logger,
                                                                      &config](ll::event::PlayerSprintedEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerSprintedEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerSprintedEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            });
    }

    if (config.playerSwingEvent.log) {
        ::playerSwingEventListener =
            eventBus.emplaceListener<ll::event::PlayerSwingEvent>([&logger,
                                                                   &config](ll::event::PlayerSwingEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                logger.log(
                    config.locateName,
                    config.playerSwingEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerSwingEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z
                );
            });
    }

    if (config.playerUseItemEvent.log) {
        ::playerUseItemEventListener =
            eventBus.emplaceListener<ll::event::PlayerUseItemEvent>([&logger,
                                                                     &config](ll::event::PlayerUseItemEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                const auto              it   = event.item();
                logger.log(
                    config.locateName,
                    config.playerUseItemEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerUseItemEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    it.getCustomName().empty() ? it.getName() : it.getCustomName() + "(" + it.getTypeName() + ")"
                );
            });
    }

    if (config.playerUseItemOnEvent.log) {
        ::playerUseItemOnEventListener = eventBus.emplaceListener<ll::event::PlayerUseItemOnEvent>(
            [&logger, &config](ll::event::PlayerUseItemOnEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pebd = getPlayerEventBaseData(event);
                const auto              bpos = event.blockPos();
                const auto& block = event.self().getDimension().getBlockSourceFromMainChunkSource().getBlock(bpos);
                const auto  fpos  = event.clickPos();
                const auto  it    = event.item();
                logger.log(
                    config.locateName,
                    config.playerUseItemOnEvent.noOutputContent,
                    ti,
                    pebd.self,
                    "PlayerUseItemOnEvent",
                    pebd.UUID,
                    pebd.dim,
                    pebd.x,
                    pebd.y,
                    pebd.z,
                    block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                    std::to_string(bpos.x),
                    std::to_string(bpos.y),
                    std::to_string(bpos.z),
                    std::string{ll::i18n::getInstance()->get("log.info.item", config.locateName)} + ": "
                        + (it.getCustomName().empty() ? it.getName() : it.getCustomName()) + "(" + it.getTypeName()
                        + "); " + std::string{ll::i18n::getInstance()->get("log.info.face", config.locateName)} + ": "
                        + std::string{ll::i18n::getInstance()
                                          ->get("side." + std::to_string((schar)event.face()), config.locateName)}
                        + "; " + std::string{ll::i18n::getInstance()->get("log.info.clickPos", config.locateName)}
                        + ": " + std::to_string(fpos.x) + " " + std::to_string(fpos.y) + " " + std::to_string(fpos.z)

                );
            }
        );
    }
}

void removePlayerEventListener() {
    auto& eventBus = ll::event::EventBus::getInstance();
    eventBus.removeListener(::playerAddExperienceEventListener);
    eventBus.removeListener(::playerAttackEventListener);
    eventBus.removeListener(::playerChangePermEventListener);
    eventBus.removeListener(::playerChatEventListener);
    eventBus.removeListener(::playerConnectEventListener);
    eventBus.removeListener(::playerDestroyBlockEventListener);
    eventBus.removeListener(::playerDieEventListener);
    eventBus.removeListener(::playerInteractBlockEventListener);
    eventBus.removeListener(::playerJoinEventListener);
    eventBus.removeListener(::playerJumpEventListener);
    eventBus.removeListener(::playerLeaveEventListener);
    eventBus.removeListener(::playerPickUpItemEventListener);
    eventBus.removeListener(::playerPlacingBlockEventListener);
    eventBus.removeListener(::playerPlacedBlockEventListener);
    eventBus.removeListener(::playerRespawnEventListener);
    eventBus.removeListener(::playerSneakingEventListener);
    eventBus.removeListener(::playerSneakedEventListener);
    eventBus.removeListener(::playerSprintingEventListener);
    eventBus.removeListener(::playerSprintedEventListener);
    eventBus.removeListener(::playerSwingEventListener);
    eventBus.removeListener(::playerUseItemEventListener);
    eventBus.removeListener(::playerUseItemOnEventListener);
}
} // namespace levi_logger::listener