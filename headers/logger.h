#pragma once

#include <string>
#include <mutex>

enum class LogLevel {
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
};

std::string logLevelToStr(LogLevel loglevel) {
    switch(loglevel) {
        case LogLevel::INFO:
            return "INFO";
            break;  
        case LogLevel::WARNING:
            return "WARNING";
            break;
        case LogLevel::ERROR:
            return "ERROR";
            break;
        default:
            return "";
    }
}

LogLevel strToLogLevel(std::string str) {
    if (str == "INFO") return LogLevel::INFO;
    if (str == "WARNING") return LogLevel::WARNING;
    if (str == "ERROR") return LogLevel::ERROR;
    return LogLevel::INFO;
}

class Logger {
public:
    Logger(std::string filename, LogLevel defaultLogLevel) {
        this->filename = filename;
    }

    bool log(std::string message, LogLevel logLevel);

    void setDefaultLogLevel(LogLevel loglevel);

private:
    LogLevel defaultLogLevel;
    std::mutex mtx;
    std::string filename;
};