#include "levi_logger/logger/Logger.h"


#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"
#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandPermissionLevel.h"

#include <string>

namespace levi_logger::command {
void regCmd(std::string_view locateName, levi_logger::logger::Logger& logger) {
    auto& cmd = ll::command::CommandRegistrar::getInstance()
                    .getOrCreateCommand("levilogger", "Levi Logger", CommandPermissionLevel::Owner);
    cmd.overload().text("rotate").execute([&logger, locateName](CommandOrigin const&, CommandOutput& output) {
        logger.rotate(locateName);
        output.success("logger rotated");
    });
}
} // namespace levi_logger::command