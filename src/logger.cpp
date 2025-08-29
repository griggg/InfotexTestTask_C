#include "../headers/logger.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

Logger::Logger(std::string filename, LogLevel priorityLogLevel) {
	this->filename = filename;
	this->priorityLogLevel = priorityLogLevel;
	file = std::ofstream("build/"+this->filename);
}

std::string getCurrentTime() {
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	std::tm tm = *std::localtime(&time);

	std::stringstream ss;
	ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

bool Logger::log(std::string message, LogLevel logLevel) {
	std::lock_guard<std::mutex> lk(this->mtx);

	if (logLevel >= this->priorityLogLevel) {
		file << message << wordSeparator << logLevelToStr(logLevel)
			 << wordSeparator << getCurrentTime() << lineSeparator;
		file.flush();
		return true;
	}
	return false;
}

void Logger::setPriorityLogLevel(LogLevel loglevel) {
	this->priorityLogLevel = loglevel;
}

LogLevel Logger::getPriorityLogLevel() { return priorityLogLevel; }
Logger::~Logger() { file.close(); }
