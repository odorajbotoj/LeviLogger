#pragma once

#include "mc/world/actor/player/Player.h"

#include <string>

struct PlayerBaseData {
    std::string self; // self
    std::string UUID; // selfUUID
    std::string dim;  // dim
    std::string x;    // x
    std::string y;    // y
    std::string z;    // z
};
const PlayerBaseData getPlayerBaseData(Player&);

namespace levi_logger::listener {

void addEventListener();
void removeEventListener();

void addEventHookListener();
void removeEventHookListener();

} // namespace levi_logger::listener