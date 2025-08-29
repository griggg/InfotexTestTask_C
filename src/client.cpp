#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <thread>
#include <atomic>

#include "../headers/logger.h"
#include "../headers/utils.h"



void print(std::string msg) { std::cout << msg << std::endl; }

class Client {
public:
	Client(std::string line) {
        logger = loggerInit(line);
        queue_worker = std::thread(&Client::worker, this, logger);
	};

	~Client() {
		{
			std::lock_guard lk(mutex);
			running.store(false);
		}
		cv.notify_all();

		if (queue_worker.joinable()) {
			queue_worker.join();
		}
	}

	

	std::shared_ptr<Logger> loggerInit(std::string line) {
		std::vector<std::string> argsLoggerInit = split(line, ' ');
		std::string filename = argsLoggerInit[0];

		LogLevel logLevel;
		if (argsLoggerInit.size() != 2) {
			logLevel = LogLevel::INFO;
			print("Выбран уровень логирофания по дефолту INFO");
		} else {
			std::string defaultLogLevel = argsLoggerInit[1];
			try {
				logLevel = Logger::strToLogLevel(defaultLogLevel);
			} catch (std::invalid_argument) {
				return nullptr;
			}
		}

		std::shared_ptr<Logger> logger =
			std::make_shared<Logger>(argsLoggerInit[0], logLevel);

		return logger;
	}

	void worker(std::shared_ptr<Logger> logger) {
		while (true) {
			std::unique_lock<std::mutex> lk(mutex);
			cv.wait(lk, [this] {
				return running.load() == false || this->queue_tasks.empty() == false;
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

    void log(std::vector<std::string> args) {
		std::lock_guard<std::mutex> lk(mutex);

        if (args.size() == 3) {
				queue_tasks.push({args[1], Logger::strToLogLevel(args[2])});
				cv.notify_all();
			} else if (args.size() == 2) {
				queue_tasks.push({args[1], LogLevel::INFO});

				print("Вы не указали аргумент уровня важности сообщения. Выбран  INFO");
				cv.notify_all();
			} else {
				print("Ошибка. отсутствует сообщение.");
			}
    }

    bool changePriorityLogLevel(std::vector<std::string> args) {
		std::lock_guard<std::mutex> lk(mutex);
        if (args.size() == 2) {
				LogLevel loglevel;
				try {
					loglevel = Logger::strToLogLevel(args[1]);
				} catch (std::invalid_argument) {
					print(
						"Неизвестный тип LogLevel. Текущий приоритет: " +
						Logger::logLevelToStr(logger->getPriorityLogLevel()));
					return false;
				}
				
				logger->setPriorityLogLevel(loglevel);
                return true;
			}
        return false;
    }

private:
    std::shared_ptr<Logger> logger;
    std::queue<std::pair<std::string, LogLevel>> queue_tasks;

	std::condition_variable cv;
	std::mutex mutex;
	std::atomic<bool> running{true};
	std::thread queue_worker;
};

int main() {

	print("Инициализация Logger");
	print("Напишите <имя файла логгера> <уровень приоритета> (по умолч. INFO)");
	std::string line;
	std::getline(std::cin, line);
    Client client(line);

	print("------------");
	print("Меню логгера");
	print("1) log <сообщение> <приоритет=INFO/WARNING/ERROR> (по умолч. INFO) - Записать сообщение");
	print("2) cdp <INFO/WARNING/ERROR> - Сменить уровень приоритета у логгера");
	print("3) exit - завершить работу");
	print("------------");

	while (1) {
		std::getline(std::cin, line);
		std::vector<std::string> args = split(line, ' ');
		if (args.size() == 0) {
			// print("не получено аргументов");
			continue;
		}
		if (args[0] == "log") {
			client.log(args);
		} else if (args[0] == "cdp") {
			client.changePriorityLogLevel(args);
		} else if (args[0] == "exit") {
			if (args.size() == 1) {
				break;
			}
		} else {
			print("Неизвестная команда: " + args[0]);
		}
	}
}