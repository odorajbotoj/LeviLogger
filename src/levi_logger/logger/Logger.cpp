#include "levi_logger/logger/Logger.h"

#include "ll/api/i18n/I18n.h"
#include "ll/api/utils/WinUtils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>


namespace levi_logger::logger {

bool Logger::rotate(std::string_view locateName) {
    if (file.is_open()) file.close();
    std::pair<std::tm, int> ti = ll::win_utils::getLocalTime();
    file.open(
        filePath
            / std::format(
                "{:0>4}-{:0>2}-{:0>2}_{:0>2}-{:0>2}-{:0>2}_{:0>3}.csv",
                ti.first.tm_year + 1900,
                ti.first.tm_mon + 1,
                ti.first.tm_mday,
                ti.first.tm_hour,
                ti.first.tm_min,
                ti.first.tm_sec,
                ti.second
            ),
        std::ofstream::out | std::ofstream::trunc
    );
    if (file.is_open()) {
        auto& i18nInstance = ll::i18n::getInstance();
        file << i18nInstance->get("log.title.year", locateName) << ","
             << i18nInstance->get("log.title.month", locateName) << ","
             << i18nInstance->get("log.title.day", locateName) << "," << i18nInstance->get("log.title.hour", locateName)
             << "," << i18nInstance->get("log.title.minute", locateName) << ","
             << i18nInstance->get("log.title.second", locateName) << ","
             << i18nInstance->get("log.title.millisecond", locateName) << ","
             << i18nInstance->get("log.title.self", locateName) << ","
             << i18nInstance->get("log.title.event", locateName) << ","
             << i18nInstance->get("log.title.selfUUID", locateName) << ","
             << i18nInstance->get("log.title.dim", locateName) << "," << i18nInstance->get("log.title.x", locateName)
             << "," << i18nInstance->get("log.title.y", locateName) << ","
             << i18nInstance->get("log.title.z", locateName) << "," << i18nInstance->get("log.title.target", locateName)
             << "," << i18nInstance->get("log.title.tx", locateName) << ","
             << i18nInstance->get("log.title.ty", locateName) << "," << i18nInstance->get("log.title.tz", locateName)
             << "," << i18nInstance->get("log.title.info", locateName) << std::endl;
        currentLine = 0;
        return true;
    } else {
        return false;
    }
}

Logger::Logger() {}
Logger::~Logger() { file.close(); }

void Logger::setFilePath(std::filesystem::path fp) {
    std::filesystem::create_directories(fp);
    filePath = fp;
}

void Logger::setMaxLine(unsigned long long int ml) { maxLine = ml; }

void Logger::log(
    std::string_view          locateName,      // config
    std::vector<std::string>& noOutputContent, // noOutputContent
    std::pair<std::tm, int>   ti,              // time
    std::string               self,            // self
    std::string               event,           // event
    std::string               selfUUID,        // self UUID
    std::string               dim,             // dim
    std::string               x,               // x
    std::string               y,               // y
    std::string               z,               // z
    std::string               target,          // target
    std::string               tx,              // tx
    std::string               ty,              // ty
    std::string               tz,              // tz
    std::string               info             // info
) {
    if (currentLine >= maxLine) rotate(locateName);
    std::string content = std::to_string(ti.first.tm_year + 1900) + "," + std::to_string(ti.first.tm_mon + 1) + ","
                        + std::to_string(ti.first.tm_mday) + "," + std::to_string(ti.first.tm_hour) + ","
                        + std::to_string(ti.first.tm_min) + "," + std::to_string(ti.first.tm_sec) + ","
                        + std::to_string(ti.second) + "," + self + ","
                        + std::string{ll::i18n::getInstance()->get("event." + event, locateName)} + "," + selfUUID + ","
                        + std::string{ll::i18n::getInstance()->get("dim." + dim, locateName)} + "," + x + "," + y + ","
                        + z + "," + target + "," + tx + "," + ty + "," + tz + "," + info;
    bool output = true;
    for (auto iter = noOutputContent.begin(); iter != noOutputContent.end(); ++iter) {
        if (auto pos = content.find(*iter); pos != std::string::npos) {
            output = false;
            break;
        }
    }
    if (file.is_open() && output) {
        file << content << std::endl;
        currentLine++;
    }
}

} // namespace levi_logger::logger