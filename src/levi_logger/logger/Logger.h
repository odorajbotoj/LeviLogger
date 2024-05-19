#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <utility>


namespace levi_logger::logger {
class Logger {
private:
    // 重载构造函数和赋值运算符，禁止复制
    Logger(const Logger&);
    Logger& operator=(const Logger&);

    std::filesystem::path  filePath;
    unsigned long long int currentLine;
    unsigned long long int maxLine;
    std::ofstream          file;

public:
    Logger();
    ~Logger();
    bool rotate(std::string_view);
    void setFilePath(std::filesystem::path);
    void setMaxLine(unsigned long long int);
    void
    log(std::string_view,          // config
        std::vector<std::string>&, // noOutputContent
        std::pair<std::tm, int>,   // time
        std::string,               // self
        std::string,               // event
        std::string = "",          // self UUID
        std::string = "",          // dim
        std::string = "",          // x
        std::string = "",          // y
        std::string = "",          // z
        std::string = "",          // target
        std::string = "",          // tx
        std::string = "",          // ty
        std::string = "",          // tz
        std::string = "");         // info
};
} // namespace levi_logger::logger