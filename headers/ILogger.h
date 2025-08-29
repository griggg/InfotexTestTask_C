#pragma once

#include <string>
#include "LogLevel.h"

class ILogger {
   public:
	virtual bool log(std::string message, LogLevel logLevel) = 0;
	virtual void setPriorityLogLevel(LogLevel loglevel) = 0;
	virtual LogLevel getPriorityLogLevel() = 0;
	virtual ~ILogger() {};
};
