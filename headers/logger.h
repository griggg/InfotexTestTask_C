#pragma once

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

enum class LogLevel {
	INFO = 0,
	WARNING = 1,
	ERROR = 2,
};


class Logger {
   public:
	const char lineSeparator = '\n';
	const char wordSeparator = '|';

	Logger(std::string filename, LogLevel priority);

	bool log(std::string message, LogLevel logLevel);

	void setPriorityLogLevel(LogLevel loglevel);

	static std::string logLevelToStr(LogLevel loglevel);

	static LogLevel strToLogLevel(std::string str);

	LogLevel getPriorityLogLevel();

	~Logger();

   private:
	LogLevel priorityLogLevel;
	std::mutex mtx;
	std::string filename;
	std::ofstream file;
};