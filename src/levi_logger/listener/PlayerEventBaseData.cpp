#include "levi_logger/listener/Listener.h"

#include "ll/api/event/player/PlayerEvent.h"
#include "mc/world/level/dimension/Dimension.h"

#include <string>

const PlayerEventBaseData getPlayerEventBaseData(ll::event::PlayerEvent& event) {
    PlayerEventBaseData rst;
    auto&               player = event.self();
    rst.self                   = player.getRealName();
    rst.UUID                   = player.getUuid().asString();
    rst.dim                    = std::to_string(player.getDimension().getDimensionId());
    auto pos                   = player.getFeetPos();
    rst.x                      = std::to_string(pos.x);
    rst.y                      = std::to_string(pos.y);
    rst.z                      = std::to_string(pos.z);
    return rst;
}