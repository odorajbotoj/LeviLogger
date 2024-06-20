#include "levi_logger/Config.h"
#include "levi_logger/LeviLogger.h"
#include "levi_logger/listener/Listener.h"
#include "levi_logger/logger/Logger.h"

#include "ll/api/i18n/I18n.h"
#include "ll/api/utils/WinUtils.h"

#include "ll/api/memory/Hook.h"

#include "magic_enum.hpp"

#include "mc/entity/components/ProjectileComponent.h"
#include "mc/entity/utilities/ActorType.h"
#include "mc/server/ServerPlayer.h"
#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOriginType.h"
#include "mc/world/actor/FishingHook.h"
#include "mc/world/actor/boss/WitherBoss.h"
#include "mc/world/actor/item/ItemActor.h"
#include "mc/world/inventory/transaction/ComplexInventoryTransaction.h"
#include "mc/world/item/ItemStackBase.h"
#include "mc/world/item/registry/ItemStack.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/ChangeDimensionRequest.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/FarmBlock.h"
#include "mc/world/level/block/ItemFrameBlock.h"
#include "mc/world/level/block/RespawnAnchorBlock.h"
#include "mc/world/level/block/actor/BaseCommandBlock.h"
#include "mc/world/level/block/actor/PistonBlockActor.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/phys/AABB.h"

#include <ctime>
#include <format>
#include <memory>
#include <string>
#include <utility>

namespace {
std::string inventoryChangeInfo(const ItemStack& old_item, const ItemStack new_item) {
    if (old_item.isNull() && new_item.isNull()) return "";
    if (old_item.isNull() && !new_item.isNull()) {
        return std::format(
            "{}: {}({}); {}: {}; {}: {}",
            ll::i18n::getInstance()->get("log.info.put", config.locateName),
            new_item.getCustomName().empty() ? new_item.getName() : new_item.getCustomName(),
            new_item.getTypeName(),
            ll::i18n::getInstance()->get("log.info.count", config.locateName),
            new_item.mCount,
            ll::i18n::getInstance()->get("log.info.aux", config.locateName),
            new_item.getAuxValue()
        );
    }
    if (!old_item.isNull() && new_item.isNull()) {
        return std::format(
            "{}: {}({}); {}: {}; {}: {}",
            ll::i18n::getInstance()->get("log.info.take", config.locateName),
            old_item.getCustomName().empty() ? old_item.getName() : old_item.getCustomName(),
            old_item.getTypeName(),
            ll::i18n::getInstance()->get("log.info.count", config.locateName),
            old_item.mCount,
            ll::i18n::getInstance()->get("log.info.aux", config.locateName),
            old_item.getAuxValue()
        );
    }
    if (old_item.getTypeName() == new_item.getTypeName()) {
        if (old_item.mCount < new_item.mCount) {
            return std::format(
                "{}: {}({}); {}: {}; {}: {}",
                ll::i18n::getInstance()->get("log.info.increase", config.locateName),
                old_item.getCustomName().empty() ? old_item.getName() : old_item.getCustomName(),
                old_item.getTypeName(),
                ll::i18n::getInstance()->get("log.info.count", config.locateName),
                new_item.mCount - old_item.mCount,
                ll::i18n::getInstance()->get("log.info.aux", config.locateName),
                old_item.getAuxValue()
            );
        }
        if (old_item.mCount > new_item.mCount) {
            return std::format(
                "{}: {}({}); {}: {}; {}: {}",
                ll::i18n::getInstance()->get("log.info.reduce", config.locateName),
                old_item.getCustomName().empty() ? old_item.getName() : old_item.getCustomName(),
                old_item.getTypeName(),
                ll::i18n::getInstance()->get("log.info.count", config.locateName),
                old_item.mCount - new_item.mCount,
                ll::i18n::getInstance()->get("log.info.aux", config.locateName),
                old_item.getAuxValue()
            );
        }
    }
    return std::format(
        "{}({}); {}: {}; {}: {} {} {}({}); {}: {}; {}: {}",
        new_item.getCustomName().empty() ? new_item.getName() : new_item.getCustomName(),
        new_item.getTypeName(),
        ll::i18n::getInstance()->get("log.info.count", config.locateName),
        new_item.mCount - new_item.mCount,
        ll::i18n::getInstance()->get("log.info.aux", config.locateName),
        new_item.getAuxValue(),
        ll::i18n::getInstance()->get("log.info.replace", config.locateName),
        old_item.getCustomName().empty() ? old_item.getName() : old_item.getCustomName(),
        old_item.getTypeName(),
        ll::i18n::getInstance()->get("log.info.count", config.locateName),
        old_item.mCount - new_item.mCount,
        ll::i18n::getInstance()->get("log.info.aux", config.locateName),
        old_item.getAuxValue()
    );
}
} // namespace

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
            config.useFrameBlockEvent,
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
            config.useFrameBlockEvent,
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
            config.entityExplodeEvent,
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
            config.blockExplodeEvent,
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
            config.respawnAnchorExplodeEvent,
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
            config.blockExplodedEvent,
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
                config.commandBlockExecuteEvent,
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
                config.commandBlockExecuteEvent,
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
                config.projectileHitBlockEvent,
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

LL_TYPE_INSTANCE_HOOK(
    ProjectileHitEntityHook,
    HookPriority::Normal,
    ProjectileComponent,
    &ProjectileComponent::onHit,
    void,
    Actor&           owner,
    HitResult const& res
) {
    if (config.projectileHitEntityEvent.log) {
        const auto* target = res.getEntity();
        if (target) {
            std::pair<std::tm, int> ti         = ll::win_utils::getLocalTime();
            const auto              owner_pos  = owner.getFeetPos();
            const auto              target_pos = target->getFeetPos();
            fileLogger.log(
                config.projectileHitEntityEvent,
                ti,
                owner.getNameTag() + "(" + owner.getTypeName() + ")",
                "ProjectileHitEntityEvent",
                "",
                std::to_string(owner.getDimensionId()),
                std::to_string(owner_pos.x),
                std::to_string(owner_pos.y),
                std::to_string(owner_pos.z),
                target->getNameTag() + "(" + target->getTypeName() + ")",
                std::to_string(target_pos.x),
                std::to_string(target_pos.y),
                std::to_string(target_pos.z)
            );
        }
    }
    origin(owner, res);
}

LL_TYPE_INSTANCE_HOOK(
    WitherDestroyHook,
    HookPriority::Normal,
    WitherBoss,
    &WitherBoss::_destroyBlocks,
    void,
    Level&                       level,
    AABB const&                  bb,
    BlockSource&                 region,
    int                          range,
    WitherBoss::WitherAttackType type
) {
    if (config.witherDestroyEvent.log) {
        std::pair<std::tm, int> ti       = ll::win_utils::getLocalTime();
        const auto              min_pos  = bb.min;
        const auto              max_pos  = bb.max;
        const auto              boss_pos = this->getFeetPos();
        fileLogger.log(
            config.witherDestroyEvent,
            ti,
            this->getNameTag() + "(" + this->getTypeName() + ")",
            "WitherDestroyEvent",
            "",
            std::to_string(region.getDimensionId()),
            std::to_string(boss_pos.x),
            std::to_string(boss_pos.y),
            std::to_string(boss_pos.z),
            "",
            "",
            "",
            "",
            std::format(
                "{}: {} {} {}; {}: {} {} {}",
                ll::i18n::getInstance()->get("log.info.from", config.locateName),
                min_pos.x,
                min_pos.y,
                min_pos.z,
                ll::i18n::getInstance()->get("log.info.to", config.locateName),
                max_pos.x,
                max_pos.y,
                max_pos.z
            )
        );
    }
    origin(level, bb, region, range, type);
}

LL_TYPE_INSTANCE_HOOK(
    PistonPushHook,
    HookPriority::Normal,
    PistonBlockActor,
    &PistonBlockActor::_attachedBlockWalker,
    bool,
    BlockSource&    region,
    BlockPos const& curPos,
    uchar           curBranchFacing,
    uchar           pistonMoveFacing
) {
    bool shouldPush = origin(region, curPos, curBranchFacing, pistonMoveFacing);
    if (config.pistonPushEvent.log) {
        if (shouldPush) {
            std::pair<std::tm, int> ti    = ll::win_utils::getLocalTime();
            const auto              pos   = this->getPosition();
            const auto&             block = region.getLevel()
                                    .getOrCreateDimension(region.getDimensionId())
                                    ->getBlockSourceFromMainChunkSource()
                                    .getBlock(curPos);
            fileLogger.log(
                config.pistonPushEvent,
                ti,
                "",
                "PistonPushEvent",
                "",
                std::to_string(region.getDimensionId()),
                std::to_string(pos.x),
                std::to_string(pos.y),
                std::to_string(pos.z),
                block.buildDescriptionName() + "(" + block.getTypeName() + ")",
                std::to_string(curPos.x),
                std::to_string(curPos.y),
                std::to_string(curPos.z)
            );
        }
    }
    return shouldPush;
}

LL_TYPE_INSTANCE_HOOK(
    FarmDecayHook,
    HookPriority::Normal,
    FarmBlock,
    "?transformOnFall@FarmBlock@@UEBAXAEAVBlockSource@@AEBVBlockPos@@PEAVActor@@M@Z",
    void,
    BlockSource&    region,
    BlockPos const& pos,
    Actor*          actor,
    float           fallDistance
) {
    if (config.farmDecayEvent.log) {
        std::pair<std::tm, int> ti = ll::win_utils::getLocalTime();
        fileLogger.log(
            config.farmDecayEvent,
            ti,
            actor ? actor->getNameTag() + "(" + actor->getTypeName() + ")" : "",
            "FarmDecayEvent",
            "",
            std::to_string(region.getDimensionId()),
            actor ? std::to_string(actor->getFeetPos().x) : "",
            actor ? std::to_string(actor->getFeetPos().y) : "",
            actor ? std::to_string(actor->getFeetPos().z) : "",
            "",
            std::to_string(pos.x),
            std::to_string(pos.y),
            std::to_string(pos.z)
        );
    }
    origin(region, pos, actor, fallDistance);
}

LL_TYPE_INSTANCE_HOOK(
    PlayerDropItemHook1,
    HookPriority::Normal,
    Player,
    "?drop@Player@@UEAA_NAEBVItemStack@@_N@Z",
    bool,
    ItemStack const& item,
    bool             randomly
) {
    if (config.playerDropItemEvent.log) {
        std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
        const auto              pbd = getPlayerBaseData(*this);
        fileLogger.log(
            config.playerDropItemEvent,
            ti,
            pbd.self,
            "PlayerDropItemEvent",
            pbd.UUID,
            pbd.dim,
            pbd.x,
            pbd.y,
            pbd.z,
            (item.getCustomName().empty() ? item.getName() : item.getCustomName()) + "(" + item.getTypeName() + ")",
            "",
            "",
            "",
            std::format(
                "{}: {}; {}: {}",
                ll::i18n::getInstance()->get("log.info.count", config.locateName),
                item.mCount,
                ll::i18n::getInstance()->get("log.info.aux", config.locateName),
                item.getAuxValue()
            )
        );
    }
    return origin(item, randomly);
}

LL_TYPE_INSTANCE_HOOK(
    PlayerDropItemHook2,
    HookPriority::Normal,
    ComplexInventoryTransaction,
    "?handle@ComplexInventoryTransaction@@UEBA?AW4InventoryTransactionError@@AEAVPlayer@@_N@Z",
    InventoryTransactionError,
    Player& player,
    bool    isSenderAuthority
) {
    if (config.playerDropItemEvent.log && type == ComplexInventoryTransaction::Type::NormalTransaction) {
        InventorySource source(InventorySourceType::ContainerInventory, ContainerID::Inventory);
        auto&           actions = data.getActions(source);
        if (actions.size() == 1) {
            std::pair<std::tm, int> ti   = ll::win_utils::getLocalTime();
            const auto              pbd  = getPlayerBaseData(player);
            const auto&             item = player.getInventory().getItem(actions[0].mSlot);
            fileLogger.log(
                config.playerDropItemEvent,
                ti,
                pbd.self,
                "PlayerDropItemEvent",
                pbd.UUID,
                pbd.dim,
                pbd.x,
                pbd.y,
                pbd.z,
                (item.getCustomName().empty() ? item.getName() : item.getCustomName()) + "(" + item.getTypeName() + ")",
                "",
                "",
                "",
                std::format(
                    "{}: {}; {}: {}",
                    ll::i18n::getInstance()->get("log.info.count", config.locateName),
                    item.mCount,
                    ll::i18n::getInstance()->get("log.info.aux", config.locateName),
                    item.getAuxValue()
                )
            );
        }
    }
    return origin(player, isSenderAuthority);
}

LL_TYPE_INSTANCE_HOOK(PlayerConsumeTotemHook, HookPriority::Normal, Player, "?consumeTotem@Player@@UEAA_NXZ", bool) {
    if (config.playerConsumeTotemEvent.log) {
        std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
        const auto              pbd = getPlayerBaseData(*this);
        fileLogger.log(
            config.playerConsumeTotemEvent,
            ti,
            pbd.self,
            "PlayerConsumeTotemEvent",
            pbd.UUID,
            pbd.dim,
            pbd.x,
            pbd.y,
            pbd.z
        );
    }
    return origin();
}

LL_TYPE_INSTANCE_HOOK(
    PlayerChangeSlotHook,
    HookPriority::Normal,
    Player,
    &Player::inventoryChanged,
    void,
    Container&       container,
    int              slot,
    ItemStack const& oldItem,
    ItemStack const& newItem,
    bool             idk
) {
    if (config.playerChangeSlotEvent.log) {
        std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
        const auto              pbd = getPlayerBaseData(*this);
        fileLogger.log(
            config.playerChangeSlotEvent,
            ti,
            pbd.self,
            "PlayerChangeSlotEvent",
            pbd.UUID,
            pbd.dim,
            pbd.x,
            pbd.y,
            pbd.z,
            "",
            "",
            "",
            "",
            std::format("{}:{}; ", ll::i18n::getInstance()->get("log.info.slot", config.locateName), slot)
                + ::inventoryChangeInfo(oldItem, newItem)
        );
    }
    origin(container, slot, oldItem, newItem, idk);
}

LL_TYPE_INSTANCE_HOOK(
    PlayerSetArmorHook,
    HookPriority::Normal,
    ServerPlayer,
    "?setArmor@ServerPlayer@@UEAAXW4ArmorSlot@@AEBVItemStack@@@Z",
    void,
    ArmorSlot        armorSlot,
    ItemStack const& item
) {
    if (config.playerSetArmorEvent.log && !item.isNull()) {
        std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
        const auto              pbd = getPlayerBaseData(*this);
        fileLogger.log(
            config.playerSetArmorEvent,
            ti,
            pbd.self,
            "PlayerSetArmorEvent",
            pbd.UUID,
            pbd.dim,
            pbd.x,
            pbd.y,
            pbd.z,
            std::string{magic_enum::enum_name(armorSlot)},
            "",
            "",
            "",
            std::format(
                "{}({}); {}: {}; {}: {}",
                item.getCustomName().empty() ? item.getName() : item.getCustomName(),
                item.getTypeName(),
                ll::i18n::getInstance()->get("log.info.count", config.locateName),
                item.mCount,
                ll::i18n::getInstance()->get("log.info.aux", config.locateName),
                item.getAuxValue()
            )
        );
    }
    origin(std::move(armorSlot), item);
}

LL_TYPE_STATIC_HOOK(
    PlayerUseRespawnAnchorHook,
    HookPriority::Normal,
    RespawnAnchorBlock,
    &RespawnAnchorBlock::_trySetSpawn,
    bool,
    Player&         player,
    BlockPos const& pos,
    BlockSource&    region,
    class Level&    level
) {
    if (config.playerUseRespawnAnchorEvent.log) {
        std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
        const auto              pbd = getPlayerBaseData(player);
        fileLogger.log(
            config.playerUseRespawnAnchorEvent,
            ti,
            pbd.self,
            "PlayerUseRespawnAnchorEvent",
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
    return origin(player, pos, region, level);
}

LL_TYPE_INSTANCE_HOOK(
    PlayerPullFishingHookHook,
    HookPriority::Normal,
    FishingHook,
    &FishingHook::_pullCloser,
    void,
    Actor& inEntity,
    float  inSpeed
) {
    if (config.playerPullFishingHookEvent.log && this->getPlayerOwner()) {
        std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
        const auto              pbd = getPlayerBaseData(*(this->getPlayerOwner()));
        const auto              pos = inEntity.getFeetPos();
        ItemStack               item;
        if (inEntity.isType(ActorType::ItemEntity)) item = static_cast<ItemActor&>(inEntity).item();
        fileLogger.log(
            config.playerPullFishingHookEvent,
            ti,
            pbd.self,
            "PlayerPullFishingHookEvent",
            pbd.UUID,
            pbd.dim,
            pbd.x,
            pbd.y,
            pbd.z,
            inEntity.getNameTag() + "(" + inEntity.getTypeName() + ")",
            std::to_string(pos.x),
            std::to_string(pos.y),
            std::to_string(pos.z),
            inEntity.isType(ActorType::ItemEntity) ? std::format(
                "{}({}); {}: {}; {}: {}",
                item.getCustomName().empty() ? item.getName() : item.getCustomName(),
                item.getTypeName(),
                ll::i18n::getInstance()->get("log.info.count", config.locateName),
                item.mCount,
                ll::i18n::getInstance()->get("log.info.aux", config.locateName),
                item.getAuxValue()
            )
                                                   : ""
        );
    }
    origin(inEntity, inSpeed);
}

LL_TYPE_INSTANCE_HOOK(
    PlayerSleepHook,
    HookPriority::Normal,
    Player,
    "?startSleepInBed@Player@@UEAA?AW4BedSleepingResult@@AEBVBlockPos@@@Z",
    BedSleepingResult,
    BlockPos const& pos
) {
    if (config.playerSleepEvent.log) {
        std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
        const auto              pbd = getPlayerBaseData(*this);
        fileLogger.log(
            config.playerSleepEvent,
            ti,
            pbd.self,
            "PlayerSleepEvent",
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
    return origin(pos);
}

LL_TYPE_INSTANCE_HOOK(
    PlayerChangeDimensionHook,
    HookPriority::Normal,
    Level,
    &Level::requestPlayerChangeDimension,
    void,
    Player&                  player,
    ChangeDimensionRequest&& changeRequest
) {
    std::pair<std::tm, int> ti  = ll::win_utils::getLocalTime();
    const auto              pbd = getPlayerBaseData(player);
    fileLogger.log(
        config.playerChangeDimensionEvent,
        ti,
        pbd.self,
        "PlayerChangeDimensionEvent",
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
            "{}: {}; {}: {}",
            ll::i18n::getInstance()->get("log.info.from", config.locateName),
            ll::i18n::getInstance()
                ->get("dim." + std::to_string(static_cast<int>(changeRequest.mFromDimensionId)), config.locateName),
            ll::i18n::getInstance()->get("log.info.to", config.locateName),
            ll::i18n::getInstance()
                ->get("dim." + std::to_string(static_cast<int>(changeRequest.mToDimensionId)), config.locateName)
        )
    );
    origin(player, std::move(changeRequest));
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
    if (config.projectileHitEntityEvent.log) {
        ProjectileHitEntityHook::hook();
    }
    if (config.witherDestroyEvent.log) {
        WitherDestroyHook::hook();
    }
    if (config.pistonPushEvent.log) {
        PistonPushHook::hook();
    }
    if (config.farmDecayEvent.log) {
        FarmDecayHook::hook();
    }
    if (config.playerDropItemEvent.log) {
        PlayerDropItemHook1::hook();
        PlayerDropItemHook2::hook();
    }
    if (config.playerConsumeTotemEvent.log) {
        PlayerConsumeTotemHook::hook();
    }
    if (config.playerChangeSlotEvent.log) {
        PlayerChangeSlotHook::hook();
    }
    if (config.playerSetArmorEvent.log) {
        PlayerSetArmorHook::hook();
    }
    if (config.playerUseRespawnAnchorEvent.log) {
        PlayerUseRespawnAnchorHook::hook();
    }
    if (config.playerPullFishingHookEvent.log) {
        PlayerPullFishingHookHook::hook();
    }
    if (config.playerSleepEvent.log) {
        PlayerSleepHook::hook();
    }
    if (config.playerChangeDimensionEvent.log) {
        PlayerChangeDimensionHook::hook();
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
    if (config.projectileHitEntityEvent.log) {
        ProjectileHitEntityHook::unhook();
    }
    if (config.witherDestroyEvent.log) {
        WitherDestroyHook::unhook();
    }
    if (config.pistonPushEvent.log) {
        PistonPushHook::unhook();
    }
    if (config.farmDecayEvent.log) {
        FarmDecayHook::unhook();
    }
    if (config.playerDropItemEvent.log) {
        PlayerDropItemHook1::unhook();
        PlayerDropItemHook2::unhook();
    }
    if (config.playerConsumeTotemEvent.log) {
        PlayerConsumeTotemHook::unhook();
    }
    if (config.playerChangeSlotEvent.log) {
        PlayerChangeSlotHook::unhook();
    }
    if (config.playerSetArmorEvent.log) {
        PlayerSetArmorHook::unhook();
    }
    if (config.playerUseRespawnAnchorEvent.log) {
        PlayerUseRespawnAnchorHook::unhook();
    }
    if (config.playerPullFishingHookEvent.log) {
        PlayerPullFishingHookHook::unhook();
    }
    if (config.playerSleepEvent.log) {
        PlayerSleepHook::unhook();
    }
    if (config.playerChangeDimensionEvent.log) {
        PlayerChangeDimensionHook::unhook();
    }
}

} // namespace levi_logger::listener