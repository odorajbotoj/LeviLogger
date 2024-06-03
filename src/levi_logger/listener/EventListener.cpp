#include "levi_logger/Config.h"
#include "levi_logger/LeviLogger.h"
#include "levi_logger/listener/Listener.h"
#include "levi_logger/logger/Logger.h"

#include "ll/api/i18n/I18n.h"
#include "ll/api/utils/WinUtils.h"

#include "ll/api/event/EventBus.h"
#include "ll/api/event/ListenerBase.h"

#include "ll/api/event/command/ExecuteCommandEvent.h"
#include "ll/api/event/entity/ActorHurtEvent.h"
#include "ll/api/event/entity/MobDieEvent.h"
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

#include "magic_enum.hpp"
#include "mc/world/actor/Actor.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/dimension/Dimension.h"

#include <format>
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
ll::event::ListenerPtr mobDieEventListener;
ll::event::ListenerPtr actorHurtEventListener;
ll::event::ListenerPtr executingCommandEventListener;
} // namespace

namespace levi_logger::listener {

void addEventListener() {
    auto& eventBus = ll::event::EventBus::getInstance();

    if (config.playerAddExperienceEvent.log) {
        ::playerAddExperienceEventListener =
            eventBus.emplaceListener<ll::event::PlayerAddExperienceEvent>([](ll::event::PlayerAddExperienceEvent& event
                                                                          ) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerAddExperienceEvent,
                    ti,
                    pbd.self,
                    "PlayerAddExperienceEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z
                );
            });
    }

    if (config.playerAttackEvent.log) {
        ::playerAttackEventListener =
            eventBus.emplaceListener<ll::event::PlayerAttackEvent>([](ll::event::PlayerAttackEvent& event) {
                std::pair<std::tm, int> ti        = ll::win_utils::getLocalTime();
                const auto              pbd       = getPlayerBaseData(event.self());
                auto&                   target    = event.target();
                auto                    targetPos = target.getFeetPos();
                fileLogger.log(
                    config.playerAttackEvent,
                    ti,
                    pbd.self,
                    "PlayerAttackEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    target.getNameTag() + "(" + target.getTypeName() + ")",
                    std::to_string(targetPos.x),
                    std::to_string(targetPos.y),
                    std::to_string(targetPos.z)
                );
            });
    }

    if (config.playerChangePermEvent.log) {
        ::playerChangePermEventListener =
            eventBus.emplaceListener<ll::event::PlayerChangePermEvent>([](ll::event::PlayerChangePermEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerChangePermEvent,
                    ti,
                    pbd.self,
                    "PlayerChangePermEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    "",
                    "",
                    "",
                    "",
                    std::format(
                        "{}: {}",
                        ll::i18n::getInstance()->get("log.info.newPerm", config.locateName),
                        magic_enum::enum_name(event.newPerm())
                    )
                );
            });
    }

    if (config.playerChatEvent.log) {
        ::playerChatEventListener =
            eventBus.emplaceListener<ll::event::PlayerChatEvent>([](ll::event::PlayerChatEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerChatEvent,
                    ti,
                    pbd.self,
                    "PlayerChatEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    "",
                    "",
                    "",
                    "",
                    std::format(
                        "{}: {}",
                        ll::i18n::getInstance()->get("log.info.message", config.locateName),
                        event.message()
                    )
                );
            });
    }

    if (config.playerConnectEvent.log) {
        ::playerConnectEventListener =
            eventBus.emplaceListener<ll::event::PlayerConnectEvent>([](ll::event::PlayerConnectEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerConnectEvent,
                    ti,
                    pbd.self,
                    "PlayerConnectEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z
                );
            });
    }

    if (config.playerDestroyBlockEvent.log) {
        ::playerDestroyBlockEventListener =
            eventBus.emplaceListener<ll::event::PlayerDestroyBlockEvent>([](ll::event::PlayerDestroyBlockEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pbd  = getPlayerBaseData(event.self());
                const auto              bpos = event.pos();
                const auto& block = event.self().getDimension().getBlockSourceFromMainChunkSource().getBlock(bpos);
                fileLogger.log(
                    config.playerDestroyBlockEvent,
                    ti,
                    pbd.self,
                    "PlayerDestroyBlockEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                    std::to_string(bpos.x),
                    std::to_string(bpos.y),
                    std::to_string(bpos.z)
                );
            });
    }

    if (config.playerDieEvent.log) {
        ::playerDieEventListener =
            eventBus.emplaceListener<ll::event::PlayerDieEvent>([](ll::event::PlayerDieEvent& event) {
                std::pair<std::tm, int> ti     = ll::win_utils::getLocalTime();
                const auto              pbd    = getPlayerBaseData(event.self());
                Actor*                  source = nullptr;
                if (event.source().isEntitySource()) {
                    source = event.self().getLevel().fetchEntity(event.source().getDamagingEntityUniqueID());
                    if (source)
                        if (event.source().isChildEntitySource()) source = source->getOwner();
                }
                fileLogger.log(
                    config.playerDieEvent,
                    ti,
                    pbd.self,
                    "PlayerDieEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    "",
                    "",
                    "",
                    "",
                    source ? std::format(
                        "{}: {}({})",
                        ll::i18n::getInstance()->get("log.info.source", config.locateName),
                        source->getNameTag(),
                        source->getTypeName()
                    )
                           : ""
                );
            });
    }

    if (config.playerInteractBlockEvent.log) {
        ::playerInteractBlockEventListener =
            eventBus.emplaceListener<ll::event::PlayerInteractBlockEvent>([](ll::event::PlayerInteractBlockEvent& event
                                                                          ) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pbd  = getPlayerBaseData(event.self());
                const auto              bpos = event.pos();
                const auto& block = event.self().getDimension().getBlockSourceFromMainChunkSource().getBlock(bpos);
                fileLogger.log(
                    config.playerInteractBlockEvent,
                    ti,
                    pbd.self,
                    "PlayerInteractBlockEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                    std::to_string(bpos.x),
                    std::to_string(bpos.y),
                    std::to_string(bpos.z)
                );
            });
    }

    if (config.playerJoinEvent.log) {
        ::playerJoinEventListener = eventBus.emplaceListener<ll::event::PlayerJoinEvent>([](ll::event::PlayerJoinEvent&
                                                                                                event) {
            std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
            const auto              pbd = getPlayerBaseData(event.self());
            fileLogger
                .log(config.playerJoinEvent, ti, pbd.self, "PlayerJoinEvent", pbd.UUID, pbd.dim, pbd.x, pbd.y, pbd.z);
        });
    }

    if (config.playerJumpEvent.log) {
        ::playerJumpEventListener = eventBus.emplaceListener<ll::event::PlayerJumpEvent>([](ll::event::PlayerJumpEvent&
                                                                                                event) {
            std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
            const auto              pbd = getPlayerBaseData(event.self());
            fileLogger
                .log(config.playerJumpEvent, ti, pbd.self, "PlayerJumpEvent", pbd.UUID, pbd.dim, pbd.x, pbd.y, pbd.z);
        });
    }

    if (config.playerLeaveEvent.log) {
        ::playerLeaveEventListener =
            eventBus.emplaceListener<ll::event::PlayerLeaveEvent>([](ll::event::PlayerLeaveEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerLeaveEvent,
                    ti,
                    pbd.self,
                    "PlayerLeaveEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z
                );
            });
    }

    if (config.playerPickUpItemEvent.log) {
        ::playerPickUpItemEventListener =
            eventBus.emplaceListener<ll::event::PlayerPickUpItemEvent>([](ll::event::PlayerPickUpItemEvent& event) {
                std::pair<std::tm, int> ti    = ll::win_utils::getLocalTime();
                const auto              pbd   = getPlayerBaseData(event.self());
                const auto&             ita   = event.itemActor();
                const auto              itafp = ita.getFeetPos();
                const auto              it    = ita.item();
                fileLogger.log(
                    config.playerPickUpItemEvent,
                    ti,
                    pbd.self,
                    "PlayerPickUpItemEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    (it.getCustomName().empty() ? it.getName() : it.getCustomName()) + "(" + it.getTypeName() + ")",
                    std::to_string(itafp.x),
                    std::to_string(itafp.y),
                    std::to_string(itafp.z),
                    std::format(
                        "{}: {}; {}: {}",
                        ll::i18n::getInstance()->get("log.info.count", config.locateName),
                        it.mCount,
                        ll::i18n::getInstance()->get("log.info.aux", config.locateName),
                        it.getAuxValue()
                    )
                );
            });
    }

    if (config.playerPlacingBlockEvent.log) {
        ::playerPlacingBlockEventListener =
            eventBus.emplaceListener<ll::event::PlayerPlacingBlockEvent>([](ll::event::PlayerPlacingBlockEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pbd  = getPlayerBaseData(event.self());
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
                fileLogger.log(
                    config.playerPlacingBlockEvent,
                    ti,
                    pbd.self,
                    "PlayerPlacingBlockEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                    std::to_string(pos.x),
                    std::to_string(pos.y),
                    std::to_string(pos.z),
                    std::format(
                        "{}: {}",
                        ll::i18n::getInstance()->get("log.info.face", config.locateName),
                        ll::i18n::getInstance()->get("side." + std::to_string(face), config.locateName)
                    )
                );
            });
    }

    if (config.playerPlacedBlockEvent.log) {
        ::playerPlacedBlockEventListener =
            eventBus.emplaceListener<ll::event::PlayerPlacedBlockEvent>([](ll::event::PlayerPlacedBlockEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pbd  = getPlayerBaseData(event.self());
                const auto              bpos = event.pos();
                const auto& block = event.self().getDimension().getBlockSourceFromMainChunkSource().getBlock(bpos);
                fileLogger.log(
                    config.playerPlacedBlockEvent,
                    ti,
                    pbd.self,
                    "PlayerPlacedBlockEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                    std::to_string(bpos.x),
                    std::to_string(bpos.y),
                    std::to_string(bpos.z)
                );
            });
    }

    if (config.playerRespawnEvent.log) {
        ::playerRespawnEventListener =
            eventBus.emplaceListener<ll::event::PlayerRespawnEvent>([](ll::event::PlayerRespawnEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerRespawnEvent,
                    ti,
                    pbd.self,
                    "PlayerRespawnEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z
                );
            });
    }

    if (config.playerSneakingEvent.log) {
        ::playerSneakingEventListener =
            eventBus.emplaceListener<ll::event::PlayerSneakingEvent>([](ll::event::PlayerSneakingEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerSneakingEvent,
                    ti,
                    pbd.self,
                    "PlayerSneakingEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z
                );
            });
    }

    if (config.playerSneakedEvent.log) {
        ::playerSneakedEventListener =
            eventBus.emplaceListener<ll::event::PlayerSneakedEvent>([](ll::event::PlayerSneakedEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerSneakedEvent,
                    ti,
                    pbd.self,
                    "PlayerSneakedEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z
                );
            });
    }

    if (config.playerSprintingEvent.log) {
        ::playerSprintingEventListener =
            eventBus.emplaceListener<ll::event::PlayerSprintingEvent>([](ll::event::PlayerSprintingEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerSprintingEvent,
                    ti,
                    pbd.self,
                    "PlayerSprintingEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z
                );
            });
    }

    if (config.playerSprintedEvent.log) {
        ::playerSprintedEventListener =
            eventBus.emplaceListener<ll::event::PlayerSprintedEvent>([](ll::event::PlayerSprintedEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerSprintedEvent,
                    ti,
                    pbd.self,
                    "PlayerSprintedEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z
                );
            });
    }

    if (config.playerSwingEvent.log) {
        ::playerSwingEventListener =
            eventBus.emplaceListener<ll::event::PlayerSwingEvent>([](ll::event::PlayerSwingEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                fileLogger.log(
                    config.playerSwingEvent,
                    ti,
                    pbd.self,
                    "PlayerSwingEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z
                );
            });
    }

    if (config.playerUseItemEvent.log) {
        ::playerUseItemEventListener =
            eventBus.emplaceListener<ll::event::PlayerUseItemEvent>([](ll::event::PlayerUseItemEvent& event) {
                std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
                const auto              pbd = getPlayerBaseData(event.self());
                const auto              it  = event.item();
                fileLogger.log(
                    config.playerUseItemEvent,
                    ti,
                    pbd.self,
                    "PlayerUseItemEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    (it.getCustomName().empty() ? it.getName() : it.getCustomName()) + "(" + it.getTypeName() + ")",
                    "",
                    "",
                    "",
                    std::format(
                        "{}: {}; {}: {}",
                        ll::i18n::getInstance()->get("log.info.count", config.locateName),
                        it.mCount,
                        ll::i18n::getInstance()->get("log.info.aux", config.locateName),
                        it.getAuxValue()
                    )
                );
            });
    }

    if (config.playerUseItemOnEvent.log) {
        ::playerUseItemOnEventListener =
            eventBus.emplaceListener<ll::event::PlayerUseItemOnEvent>([](ll::event::PlayerUseItemOnEvent& event) {
                std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
                const auto              pbd  = getPlayerBaseData(event.self());
                const auto              bpos = event.blockPos();
                const auto& block = event.self().getDimension().getBlockSourceFromMainChunkSource().getBlock(bpos);
                const auto  fpos  = event.clickPos();
                const auto  it    = event.item();
                fileLogger.log(
                    config.playerUseItemOnEvent,
                    ti,
                    pbd.self,
                    "PlayerUseItemOnEvent",
                    pbd.UUID,
                    pbd.dim,
                    pbd.x,
                    pbd.y,
                    pbd.z,
                    block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                    std::to_string(bpos.x),
                    std::to_string(bpos.y),
                    std::to_string(bpos.z),
                    std::format(
                        "{}: {}({}); {}: {}; {}: {}; {}: {}; {}: {} {} {}",
                        ll::i18n::getInstance()->get("log.info.item", config.locateName),
                        it.getCustomName().empty() ? it.getName() : it.getCustomName(),
                        it.getTypeName(),
                        ll::i18n::getInstance()->get("log.info.count", config.locateName),
                        it.mCount,
                        ll::i18n::getInstance()->get("log.info.aux", config.locateName),
                        it.getAuxValue(),
                        ll::i18n::getInstance()->get("log.info.face", config.locateName),
                        ll::i18n::getInstance()->get("side." + std::to_string((schar)event.face()), config.locateName),
                        ll::i18n::getInstance()->get("log.info.clickPos", config.locateName),
                        fpos.x,
                        fpos.y,
                        fpos.z
                    )
                );
            });
    }

    if (config.mobDieEvent.log) {
        ::mobDieEventListener = eventBus.emplaceListener<ll::event::MobDieEvent>([](ll::event::MobDieEvent& event) {
            std::pair<std::tm, int> ti     = ll::win_utils::getLocalTime();
            const auto&             mob    = event.self();
            const auto              pos    = mob.getFeetPos();
            Actor*                  source = nullptr;
            if (event.source().isEntitySource()) {
                source = event.self().getLevel().fetchEntity(event.source().getDamagingEntityUniqueID());
                if (source)
                    if (event.source().isChildEntitySource()) source = source->getOwner();
            }
            fileLogger.log(
                config.mobDieEvent,
                ti,
                mob.getNameTag() + "(" + mob.getTypeName() + ")",
                "MobDieEvent",
                "",
                std::to_string(mob.getDimensionId()),
                std::to_string(pos.x),
                std::to_string(pos.y),
                std::to_string(pos.z),
                "",
                "",
                "",
                "",
                (source ? std::format(
                     "{}: {}({}); ",
                     ll::i18n::getInstance()->get("log.info.source", config.locateName),
                     source->getNameTag(),
                     source->getTypeName()
                 )
                        : "")
                    + std::format(
                        "{}: {}",
                        ll::i18n::getInstance()->get("log.info.damageCause", config.locateName),
                        magic_enum::enum_name(event.source().getCause())
                    )
            );
        });
    }

    if (config.actorHurtEvent.log) {
        ::actorHurtEventListener =
            eventBus.emplaceListener<ll::event::ActorHurtEvent>([](ll::event::ActorHurtEvent& event) {
                std::pair<std::tm, int> ti     = ll::win_utils::getLocalTime();
                const auto&             actor  = event.self();
                const auto              pos    = actor.getFeetPos();
                Actor*                  source = nullptr;
                if (event.source().isEntitySource()) {
                    source = event.self().getLevel().fetchEntity(event.source().getDamagingEntityUniqueID());
                    if (source)
                        if (event.source().isChildEntitySource()) source = source->getOwner();
                }
                fileLogger.log(
                    config.actorHurtEvent,
                    ti,
                    actor.getNameTag() + "(" + actor.getTypeName() + ")",
                    "ActorHurtEvent",
                    "",
                    std::to_string(actor.getDimensionId()),
                    std::to_string(pos.x),
                    std::to_string(pos.y),
                    std::to_string(pos.z),
                    "",
                    "",
                    "",
                    "",
                    (source ? std::format(
                         "{}: {}({}); ",
                         ll::i18n::getInstance()->get("log.info.source", config.locateName),
                         source->getNameTag(),
                         source->getTypeName()
                     )
                            : "")
                        + std::format(
                            "{}: {}; {}: {}",
                            ll::i18n::getInstance()->get("log.info.damage", config.locateName),
                            event.damage(),
                            ll::i18n::getInstance()->get("log.info.damageCause", config.locateName),
                            magic_enum::enum_name(event.source().getCause())
                        )
                );
            });
    }

    if (config.executingCommandEvent.log) {
        ::executingCommandEventListener =
            eventBus.emplaceListener<ll::event::ExecutingCommandEvent>([](ll::event::ExecutingCommandEvent& event) {
                std::pair<std::tm, int> ti     = ll::win_utils::getLocalTime();
                const Player*           player = static_cast<Player*>(event.commandContext().mOrigin->getEntity());
                if (player) {
                    const auto pbd = getPlayerBaseData(*player);
                    fileLogger.log(
                        config.executingCommandEvent,
                        ti,
                        pbd.self,
                        "ExecutingCommandEvent",
                        pbd.UUID,
                        pbd.dim,
                        pbd.x,
                        pbd.y,
                        pbd.z,
                        "",
                        "",
                        "",
                        "",
                        std::format(
                            "{}: {}",
                            ll::i18n::getInstance()->get("log.info.command", config.locateName),
                            event.commandContext().mCommand
                        )
                    );
                }
            });
    }
}

void removeEventListener() {
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
    eventBus.removeListener(::mobDieEventListener);
    eventBus.removeListener(::actorHurtEventListener);
    eventBus.removeListener(::executingCommandEventListener);
}
} // namespace levi_logger::listener