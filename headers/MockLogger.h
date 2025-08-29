#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "ILogger.h"
#include "LogLevel.h"

class MockLogger : public ILogger {
   public:
	LogLevel priority = LogLevel::INFO;
	std::vector<std::pair<std::string, LogLevel>> logs;
	std::mutex mtx;

	bool log(std::string message, LogLevel logLevel) override {
		std::lock_guard<std::mutex> lock(mtx);
		logs.emplace_back(message, logLevel);
		return true;
	}

	void setPriorityLogLevel(LogLevel loglevel) override {
		std::lock_guard<std::mutex> lock(mtx);
		priority = loglevel;
	}

	LogLevel getPriorityLogLevel() override {
		std::lock_guard<std::mutex> lock(mtx);
		return priority;
	}
};