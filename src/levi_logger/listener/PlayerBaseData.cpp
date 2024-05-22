#include "levi_logger/listener/Listener.h"

#include "mc/world/actor/player/Player.h"
#include "mc/world/level/dimension/Dimension.h"

#include <string>

const PlayerBaseData getPlayerBaseData(Player& player) {
    PlayerBaseData rst;
    rst.self = player.getRealName();
    rst.UUID = player.getUuid().asString();
    rst.dim  = std::to_string(player.getDimension().getDimensionId());
    auto pos = player.getFeetPos();
    rst.x    = std::to_string(pos.x);
    rst.y    = std::to_string(pos.y);
    rst.z    = std::to_string(pos.z);
    return rst;
}