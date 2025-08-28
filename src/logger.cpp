#include "../headers/logger.h"

#include <fstream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include "logger.h"

Logger::Logger(std::string filename, LogLevel defaultLogLevel) {
    this->filename = filename;
    this->defaultLogLevel = defaultLogLevel;
    file = std::ofstream(this->filename);
}

std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool Logger::log(std::string message, LogLevel logLevel)
{
    std::lock_guard<std::mutex> lk(this->mtx);

    if (logLevel >= this->defaultLogLevel) {
        
        file << message << wordSeparator << logLevelToStr(logLevel) << wordSeparator 
            << getCurrentTime() << lineSeparator;
        file.flush();
        return true;
    }
    return false;
}

void Logger::setDefaultLogLevel(LogLevel loglevel)
{
    this->defaultLogLevel = loglevel;
}

LogLevel Logger::getDefaultLogLevel()
{
    return defaultLogLevel;
}
Logger::~Logger()
{
    file.close();
}
