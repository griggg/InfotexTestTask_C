#pragma once

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <thread>

#include "../headers/client.h"
#include "../headers/logger.h"
#include "../headers/utils.h"

class Client {
   public:
	Client(std::string line);

	Client(std::shared_ptr<ILogger> customLogger);

	~Client();

	std::shared_ptr<Logger> loggerInit(std::string line);

	void worker(std::shared_ptr<ILogger> logger);

	void log(std::vector<std::string> args);

	bool changePriorityLogLevel(std::vector<std::string> args);

   private:
	std::shared_ptr<ILogger> logger;
	std::queue<std::pair<std::string, LogLevel>> queue_tasks;

	std::condition_variable cv;
	std::mutex mutex;
	std::atomic<bool> running{true};
	std::thread queue_worker;
};