#include "../headers/client.h"

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <thread>

#include "../headers/logger.h"
#include "../headers/utils.h"


void print(std::string msg) { std::cout << msg << std::endl; }

Client::Client(std::string line) {
	logger = loggerInit(line);
	queue_worker = std::thread(&Client::worker, this, logger);
};

Client::Client(std::shared_ptr<ILogger> customLogger) {
	logger = customLogger;
	queue_worker = std::thread(&Client::worker, this, logger);
}

Client::~Client() {
	{
		std::lock_guard lk(mutex);
		running.store(false);
	}
	cv.notify_all();

	if (queue_worker.joinable()) {
		queue_worker.join();
	}
}

std::shared_ptr<Logger> Client::loggerInit(std::string line) {
	std::vector<std::string> argsLoggerInit = split(line, ' ');
	std::string filename = argsLoggerInit[0];

	LogLevel logLevel;
	if (argsLoggerInit.size() != 2) {
		logLevel = LogLevel::INFO;
		print("Выбран уровень логирофания по дефолту INFO");
	} else {
		std::string defaultLogLevel = argsLoggerInit[1];
		try {
			logLevel = strToLogLevel(defaultLogLevel);
		} catch (std::invalid_argument) {
			return nullptr;
		}
	}

	std::shared_ptr<Logger> logger =
		std::make_shared<Logger>(argsLoggerInit[0], logLevel);

	return logger;
}

void Client::worker(std::shared_ptr<ILogger> logger) {
	while (true) {
		std::unique_lock<std::mutex> lk(mutex);
		cv.wait(lk, [this] {
			return running.load() == false ||
				   this->queue_tasks.empty() == false;
		});
		if (running.load() == false) break;
		while (!queue_tasks.empty()) {
			auto [msg, loglevel] = queue_tasks.front();
			queue_tasks.pop();
			if (logger->log(msg, loglevel)) {
				print("Лог успешно записан");
			} else {
				print("Лог не записан");
			}
		}
	}
}

void Client::log(std::vector<std::string> args) {
	std::lock_guard<std::mutex> lk(mutex);

	if (args.size() == 3) {
		LogLevel loglevel;
		try {
			loglevel = strToLogLevel(args[2]);
			queue_tasks.push({args[1], loglevel});
			cv.notify_all();
		} catch (std::invalid_argument err) {
			print(err.what());
		}
	} else if (args.size() == 2) {
		queue_tasks.push({args[1], LogLevel::INFO});

		print("Вы не указали аргумент уровня важности сообщения. Выбран  INFO");
		cv.notify_all();
	} else {
		print("Ошибка. отсутствует сообщение.");
	}
}

bool Client::changePriorityLogLevel(std::vector<std::string> args) {
	std::lock_guard<std::mutex> lk(mutex);
	if (args.size() == 2) {
		LogLevel loglevel;
		try {
			loglevel = strToLogLevel(args[1]);
		} catch (std::invalid_argument) {
			print("Неизвестный тип LogLevel. Текущий приоритет: " +
				  logLevelToStr(logger->getPriorityLogLevel()));
			return false;
		}

		logger->setPriorityLogLevel(loglevel);
		return true;
	}
	return false;
}

