#include "levi_logger/Config.h"
#include "levi_logger/LeviLogger.h"
#include "levi_logger/listener/Listener.h"
#include "levi_logger/logger/Logger.h"

#include "ll/api/i18n/I18n.h"
#include "ll/api/utils/WinUtils.h"

#include "ll/api/memory/Hook.h"

#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOriginType.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/ItemFrameBlock.h"
#include "mc/world/level/block/RespawnAnchorBlock.h"
#include "mc/world/level/block/actor/BaseCommandBlock.h"
#include "mc/world/level/dimension/Dimension.h"

#include <format>
#include <memory>
#include <string>

namespace levi_logger::listener {

LL_TYPE_INSTANCE_HOOK(
    UseFrameBlockHook1,
    HookPriority::Normal,
    ItemFrameBlock,
    "?use@ItemFrameBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
    bool,
    Player&         player,
    BlockPos const& pos,
    uchar           face
) {
    if (config.useFrameBlockEvent.log) {
        std::pair<std::tm, int> ti    = ll::win_utils::getLocalTime();
        const auto              pbd   = getPlayerBaseData(player);
        const auto&             block = player.getDimension().getBlockSourceFromMainChunkSource().getBlock(pos);
        fileLogger.log(
            config.useFrameBlockEvent.noOutputContent,
            ti,
            pbd.self,
            "UseFrameBlockEvent",
            pbd.UUID,
            pbd.dim,
            pbd.x,
            pbd.y,
            pbd.z,
            block.buildDescriptionName() + "(" + block.getTypeName() + ")",
            std::to_string(pos.x),
            std::to_string(pos.y),
            std::to_string(pos.z),
            std::string{ll::i18n::getInstance()->get("log.info.face", config.locateName)} + ": "
                + std::string{ll::i18n::getInstance()->get("side." + std::to_string(face), config.locateName)}
        );
    }
    return origin(player, pos, face);
}

LL_TYPE_INSTANCE_HOOK(
    UseFrameBlockHook2,
    HookPriority::Normal,
    ItemFrameBlock,
    "?attack@ItemFrameBlock@@UEBA_NPEAVPlayer@@AEBVBlockPos@@@Z",
    bool,
    Player*         player,
    BlockPos const& pos
) {
    if (config.useFrameBlockEvent.log) {
        std::pair<std::tm, int> ti    = ll::win_utils::getLocalTime();
        const auto              pbd   = getPlayerBaseData(*player);
        const auto&             block = player->getDimension().getBlockSourceFromMainChunkSource().getBlock(pos);
        fileLogger.log(
            config.useFrameBlockEvent.noOutputContent,
            ti,
            pbd.self,
            "UseFrameBlockEvent",
            pbd.UUID,
            pbd.dim,
            pbd.x,
            pbd.y,
            pbd.z,
            block.buildDescriptionName() + "(" + block.getTypeName() + ")",
            std::to_string(pos.x),
            std::to_string(pos.y),
            std::to_string(pos.z)
        );
    }
    return origin(player, pos);
}

LL_TYPE_INSTANCE_HOOK(
    EntityExplodeHook,
    HookPriority::Normal,
    Level,
    &Level::explode,
    bool,
    BlockSource& region,
    Actor*       source,
    Vec3 const&  pos,
    float        explosionRadius,
    bool         fire,
    bool         breaksBlocks,
    float        maxResistance,
    bool         allowUnderwater
) {
    if (config.entityExplodeEvent.log && source) {
        std::pair<std::tm, int> ti = ll::win_utils::getLocalTime();
        fileLogger.log(
            config.entityExplodeEvent.noOutputContent,
            ti,
            source->getNameTag() + "(" + source->getTypeName() + ")",
            "EntityExplodeEvent",
            "",
            std::to_string(source->getDimensionId()),
            std::to_string(source->getFeetPos().x),
            std::to_string(source->getFeetPos().y),
            std::to_string(source->getFeetPos().z),
            "",
            std::to_string(pos.x),
            std::to_string(pos.y),
            std::to_string(pos.z),
            std::format(
                "{}: {}; {}: {}; {}: {}; {}: {}",
                ll::i18n::getInstance()->get("log.info.explode.radius", config.locateName),
                explosionRadius,
                ll::i18n::getInstance()->get("log.info.explode.maxResistance", config.locateName),
                maxResistance,
                ll::i18n::getInstance()->get("log.info.explode.isDestroy", config.locateName),
                ll::i18n::getInstance()->get("bool." + std::to_string(breaksBlocks), config.locateName),
                ll::i18n::getInstance()->get("log.info.explode.isFire", config.locateName),
                ll::i18n::getInstance()->get("bool." + std::to_string(fire), config.locateName)
            )
        );
    }
    return origin(region, source, pos, explosionRadius, fire, breaksBlocks, maxResistance, allowUnderwater);
}

LL_TYPE_INSTANCE_HOOK(
    BlockExplodeHook,
    HookPriority::Normal,
    Level,
    &Level::explode,
    bool,
    BlockSource& region,
    Actor*       source,
    Vec3 const&  pos,
    float        explosionRadius,
    bool         fire,
    bool         breaksBlocks,
    float        maxResistance,
    bool         allowUnderwater
) {
    if (config.blockExplodeEvent.log) {
        std::pair<std::tm, int> ti    = ll::win_utils::getLocalTime();
        const auto&             block = region.getBlock(pos);
        fileLogger.log(
            config.blockExplodeEvent.noOutputContent,
            ti,
            block.buildDescriptionName() + "(" + block.getTypeName() + ")",
            "BlockExplodeEvent",
            "",
            std::to_string(region.getDimensionId()),
            std::to_string(pos.x),
            std::to_string(pos.y),
            std::to_string(pos.z),
            "",
            std::to_string(pos.x),
            std::to_string(pos.y),
            std::to_string(pos.z),
            std::format(
                "{}: {}; {}: {}; {}: {}; {}: {}",
                ll::i18n::getInstance()->get("log.info.explode.radius", config.locateName),
                explosionRadius,
                ll::i18n::getInstance()->get("log.info.explode.maxResistance", config.locateName),
                maxResistance,
                ll::i18n::getInstance()->get("log.info.explode.isDestroy", config.locateName),
                ll::i18n::getInstance()->get("bool." + std::to_string(breaksBlocks), config.locateName),
                ll::i18n::getInstance()->get("log.info.explode.isFire", config.locateName),
                ll::i18n::getInstance()->get("bool." + std::to_string(fire), config.locateName)
            )
        );
    }
    return origin(region, source, pos, explosionRadius, fire, breaksBlocks, maxResistance, allowUnderwater);
}

LL_TYPE_STATIC_HOOK(
    RespawnAnchorExplodeHook,
    HookPriority::Normal,
    RespawnAnchorBlock,
    &RespawnAnchorBlock::_explode,
    void,
    Player&         player,
    BlockPos const& pos,
    BlockSource&    region,
    Level&          level
) {
    if (config.respawnAnchorExplodeEvent.log) {
        std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
        const auto              pbd = getPlayerBaseData(player);
        fileLogger.log(
            config.respawnAnchorExplodeEvent.noOutputContent,
            ti,
            pbd.self,
            "RespawnAnchorExplodeEvent",
            pbd.UUID,
            pbd.dim,
            pbd.x,
            pbd.y,
            pbd.z,
            "",
            std::to_string(pos.x),
            std::to_string(pos.y),
            std::to_string(pos.z)
        );
    }
    origin(player, pos, region, level);
}

LL_TYPE_INSTANCE_HOOK(
    BlockExplodedHook,
    HookPriority::Normal,
    Block,
    &Block::onExploded,
    void,
    BlockSource&    region,
    BlockPos const& pos,
    Actor*          entitySource
) {
    if (config.blockExplodedEvent.log) {
        std::pair<std::tm, int> ti    = ll::win_utils::getLocalTime();
        const auto&             block = region.getBlock(pos);
        fileLogger.log(
            config.blockExplodedEvent.noOutputContent,
            ti,
            block.buildDescriptionName() + "(" + block.getTypeName() + ")",
            "BlockExplodedEvent",
            "",
            std::to_string(region.getDimensionId()),
            std::to_string(pos.x),
            std::to_string(pos.y),
            std::to_string(pos.z),
            "",
            "",
            "",
            "",
            entitySource ? std::format(
                "{}: {}({})",
                ll::i18n::getInstance()->get("log.info.source", config.locateName),
                entitySource->getNameTag(),
                entitySource->getTypeName()
            )
                         : ""
        );
    }
    origin(region, pos, entitySource);
}

LL_TYPE_INSTANCE_HOOK(
    CommandBlockExecuteHook,
    HookPriority::Normal,
    BaseCommandBlock,
    &BaseCommandBlock::_performCommand,
    bool,
    BlockSource&         region,
    CommandOrigin const& commandOrigin,
    bool&                markForSaving
) {
    if (config.commandBlockExecuteEvent.log) {
        if (commandOrigin.getOriginType() == CommandOriginType::MinecartCommandBlock) {
            std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
            const auto&             pos = commandOrigin.getEntity()->getPosition();
            fileLogger.log(
                config.commandBlockExecuteEvent.noOutputContent,
                ti,
                std::string{ll::i18n::getInstance()->get("commandOriginType.minecart", config.locateName)},
                "CommandBlockExecuteEvent",
                "",
                std::to_string(region.getDimensionId()),
                std::to_string(pos.x),
                std::to_string(pos.y),
                std::to_string(pos.z),
                "",
                "",
                "",
                "",
                std::format(
                    "{}: {}",
                    ll::i18n::getInstance()->get("log.info.command", config.locateName),
                    this->getCommand()
                )
            );
        } else {
            std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
            const auto              pos = commandOrigin.getBlockPosition();
            fileLogger.log(
                config.commandBlockExecuteEvent.noOutputContent,
                ti,
                std::string{ll::i18n::getInstance()->get("commandOriginType.block", config.locateName)},
                "CommandBlockExecuteEvent",
                "",
                std::to_string(region.getDimensionId()),
                std::to_string(pos.x),
                std::to_string(pos.y),
                std::to_string(pos.z),
                "",
                "",
                "",
                "",
                std::format(
                    "{}: {}",
                    ll::i18n::getInstance()->get("log.info.command", config.locateName),
                    this->getCommand()
                )
            );
        }
    }
    return origin(region, commandOrigin, markForSaving);
}

LL_TYPE_INSTANCE_HOOK(
    ProjectileHitBlockHook,
    HookPriority::Normal,
    Block,
    &Block::onProjectileHit,
    void,
    BlockSource&    region,
    BlockPos const& pos,
    Actor const&    projectile
) {
    if (config.projectileHitBlockEvent.log) {
        if (pos != BlockPos::ZERO && !this->isAir()) {
            std::pair<std::tm, int> ti             = ll::win_utils::getLocalTime();
            const auto&             block          = region.getBlock(pos);
            const auto              projectile_pos = projectile.getFeetPos();
            fileLogger.log(
                config.projectileHitBlockEvent.noOutputContent,
                ti,
                projectile.getNameTag() + "(" + projectile.getTypeName() + ")",
                "ProjectileHitBlockEvent",
                "",
                std::to_string(region.getDimensionId()),
                std::to_string(projectile_pos.x),
                std::to_string(projectile_pos.y),
                std::to_string(projectile_pos.z),
                block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                std::to_string(pos.x),
                std::to_string(pos.y),
                std::to_string(pos.z)
            );
        }
    }
    origin(region, pos, projectile);
}


void addEventHookListener() {
    if (config.useFrameBlockEvent.log) {
        UseFrameBlockHook1::hook();
        UseFrameBlockHook2::hook();
    }
    if (config.entityExplodeEvent.log) {
        EntityExplodeHook::hook();
    }
    if (config.blockExplodeEvent.log) {
        BlockExplodeHook::hook();
    }
    if (config.respawnAnchorExplodeEvent.log) {
        RespawnAnchorExplodeHook::hook();
    }
    if (config.blockExplodedEvent.log) {
        BlockExplodedHook::hook();
    }
    if (config.commandBlockExecuteEvent.log) {
        CommandBlockExecuteHook::hook();
    }
    if (config.projectileHitBlockEvent.log) {
        ProjectileHitBlockHook::hook();
    }
}

void removeEventHookListener() {
    if (config.useFrameBlockEvent.log) {
        UseFrameBlockHook1::unhook();
        UseFrameBlockHook2::unhook();
    }
    if (config.entityExplodeEvent.log) {
        EntityExplodeHook::unhook();
    }
    if (config.blockExplodeEvent.log) {
        BlockExplodeHook::unhook();
    }
    if (config.respawnAnchorExplodeEvent.log) {
        RespawnAnchorExplodeHook::unhook();
    }
    if (config.blockExplodedEvent.log) {
        BlockExplodedHook::unhook();
    }
    if (config.commandBlockExecuteEvent.log) {
        CommandBlockExecuteHook::unhook();
    }
    if (config.projectileHitBlockEvent.log) {
        ProjectileHitBlockHook::unhook();
    }
}

} // namespace levi_logger::listener