#include "levi_logger/LeviLogger.h"
#include "levi_logger/logger/Logger.h"

#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"
#include "ll/api/i18n/I18n.h"

#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandPermissionLevel.h"

#include <string>

namespace levi_logger::command {
void regCmd() {
    auto& cmd = ll::command::CommandRegistrar::getInstance()
                    .getOrCreateCommand("levilogger", "Levi Logger", CommandPermissionLevel::Owner);
    cmd.overload().text("rotate").execute([](CommandOrigin const&, CommandOutput& output) {
        fileLogger.rotate();
        output.success(ll::i18n::getInstance()->get("command.rotated", config.locateName));
    });
}
} // namespace levi_logger::command