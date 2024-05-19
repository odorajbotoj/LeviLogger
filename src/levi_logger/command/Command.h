#pragma once

#include "levi_logger/logger/Logger.h"


namespace levi_logger::command {
void regCmd(std::string_view, levi_logger::logger::Logger&);
}