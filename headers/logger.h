#pragma once

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include "ILogger.h"
#include "LogLevel.h"

class Logger : public ILogger {
   public:
	const char lineSeparator = '\n';
	const char wordSeparator = '|';

	Logger(std::string filename, LogLevel priority);

	bool log(std::string message, LogLevel logLevel);

	void setPriorityLogLevel(LogLevel loglevel);

	LogLevel getPriorityLogLevel();

	~Logger();

   private:
	LogLevel priorityLogLevel;
	std::mutex mtx;
	std::string filename;
	std::ofstream file;
};

