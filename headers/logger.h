#pragma once

#include <string>

enum class LogLevel {
    INFO = 0,
    WARNING = 1,
    ERRROR = 2,
};

class Logger {
public:

    Logger(std::string filename, LogLevel defaultLogLevel) {

    }

    bool log(std::string message, LogLevel logLevel);

    void setDefaultLogLevel(LogLevel loglevel);

private:
    LogLevel defaultLogLevel;
};