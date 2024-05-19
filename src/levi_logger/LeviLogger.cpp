#include "levi_logger/LeviLogger.h"
#include "levi_logger/Config.h"
#include "levi_logger/command/Command.h"
#include "levi_logger/listener/Listener.h"
#include "levi_logger/logger/Logger.h"

#include <memory>

#include "ll/api/Config.h"
#include "ll/api/i18n/I18n.h"
#include "ll/api/plugin/NativePlugin.h"
#include "ll/api/plugin/RegisterHelper.h"

Config config;

levi_logger::logger::Logger fileLogger;


namespace levi_logger {

static std::unique_ptr<LeviLogger> instance;

LeviLogger& LeviLogger::getInstance() { return *instance; }

bool LeviLogger::load() {
    const auto& logger = getSelf().getLogger();

    // load config
    logger.info("Loading config");
    const auto& configFilePath = getSelf().getConfigDir() / "config.json";
    if (!ll::config::loadConfig(config, configFilePath)) {
        logger.warn("Cannot load configurations from {}", configFilePath);
        logger.info("Saving default configurations");
        if (!ll::config::saveConfig(config, configFilePath)) {
            logger.error("Cannot save default configurations to {}", configFilePath);
            return false;
        }
    }

    // load i18n
    logger.info("Loading i18n");
    ll::i18n::load(getSelf().getLangDir());

    // load fileLogger
    logger.info("Loading fileLogger");
    fileLogger.setFilePath(getSelf().getDataDir() / "log");
    fileLogger.setMaxLine(config.maxLine);
    if (!fileLogger.rotate(config.locateName)) {
        logger.error(ll::i18n::getInstance()->get("plugin.cannotOpenLogFile", config.locateName));
        return false;
    }

    logger.info(ll::i18n::getInstance()->get("plugin.welcome", config.locateName));
    return true;
}

bool LeviLogger::enable() {
    levi_logger::listener::addListener(config, fileLogger);
    levi_logger::command::regCmd(config.locateName, fileLogger);

    return true;
}

bool LeviLogger::disable() {
    levi_logger::listener::removeListener();

    return true;
}

} // namespace levi_logger

LL_REGISTER_PLUGIN(levi_logger::LeviLogger, levi_logger::instance);
