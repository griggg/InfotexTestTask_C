#pragma once

#include <string>
#include <mutex>
#include <iostream>
#include <fstream>

enum class LogLevel {
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
};

static std::string logLevelToStr(LogLevel loglevel) {
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

static LogLevel strToLogLevel(std::string str) {
    if (str == "INFO") return LogLevel::INFO;
    if (str == "WARNING") return LogLevel::WARNING;
    if (str == "ERROR") return LogLevel::ERROR;
    throw std::invalid_argument("Незивестный LogLevel");
}

class Logger {
public:

    const char lineSeparator = '\n';
    const char wordSeparator = '|';

    Logger(std::string filename, LogLevel defaultLogLevel);

    bool log(std::string message, LogLevel logLevel);

    void setDefaultLogLevel(LogLevel loglevel);
    
    LogLevel getDefaultLogLevel();

    ~Logger();
private:
    LogLevel defaultLogLevel;
    std::mutex mtx;
    std::string filename;
    std::ofstream file;

    
};