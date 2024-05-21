#pragma once

#include "levi_logger/Config.h"
#include "levi_logger/logger/Logger.h"

#include "ll/api/event/player/PlayerEvent.h"

#include <string>

struct PlayerEventBaseData {
    std::string self; // self
    std::string UUID; // selfUUID
    std::string dim;  // dim
    std::string x;    // x
    std::string y;    // y
    std::string z;    // z
};
const PlayerEventBaseData getPlayerEventBaseData(ll::event::PlayerEvent&);

namespace levi_logger::listener {

void addPlayerEventListener(Config&, levi_logger::logger::Logger&);
void removePlayerEventListener();

} // namespace levi_logger::listener