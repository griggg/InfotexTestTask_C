#include "../headers/Client.h"

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <thread>

#include "../headers/Print.h"

/**
 * @brief Конструктор по умолчанию
 * Если использовать его, то потом нужно вызывать Client::loggerInit(),
 * чтобы инициализировать логгер
 */
Client::Client() {

};

/**
 * @brief Конструктор с логгером.
 * Можно подменить логгер на мок для тестов
 *
 * @param customLogger ILogger
 */
Client::Client(std::shared_ptr<ILogger> customLogger) {
	logger = customLogger;
	queue_worker = std::thread(&Client::worker, this, logger);
}

/**
 * @brief Деструктор
 * Перед тем как удалить condition variable и другие memory location
 * которые использует поток-воркер, он должен уведомить его, что работа
 * прекращается и заджоинить его
 */
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

/**
 * @brief Инициализирует логгер и поток воркер,
 * обрабатывающий команды лог от пользователя
 *
 * Пример: client.loggerInit("log.txt INFO");
 *
 * @param line std::string строка, полученная от пользователя
 * @throw std::invalid_argument Если строка не корректная для команды
 * @throw std::runtime_error Если не удалось создать логгер
 */
void Client::loggerInit(std::string line) {
	std::vector<std::string> argsLoggerInit = split(line, ' ');

	if (argsLoggerInit.size() == 0 || argsLoggerInit.size() > 2) {
		throw std::invalid_argument(
			RED("Ошибка.") +
			"Неправильное количество аргументов для инициализации логгера");
	}

	std::string filename = argsLoggerInit[0];

	LogLevel logLevel;
	if (argsLoggerInit.size() != 2) {
		logLevel = LogLevel::INFO;
		std::cout << "Выбран уровень приоритета по дефолту " << GREEN("INFO")
				  << std::endl;
	} else {
		std::string defaultLogLevel = argsLoggerInit[1];
		try {
			logLevel = strToLogLevel(defaultLogLevel);
		} catch (std::invalid_argument &) {
			throw std::invalid_argument(RED("Ошибка.") +
										"Не существует такого типа loglevel");
		}
	}
	try {
		logger = std::make_shared<Logger>(argsLoggerInit[0], logLevel);
		queue_worker = std::thread(&Client::worker, this, logger);
	} catch (std::runtime_error &e) {
		throw e;
	}
}

/**
 * @brief Обработчик команд от пользователя
 * Получает уведомление, когда добавляется сообщение,
 * забирает сообщения из очереди и обрабатывает их
 *
 * @param logger std::shared_ptr<ILogger>
 */
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
			try {
				if (logger->log(msg, loglevel)) {
					std::cout << GREEN("Лог успешно записан") << std::endl;
				} else {
					std::cout << YELLOW("Лог не записан.") << "Важность "
							  << logLevelToStr(loglevel)
							  << "меньше приоритета логгера "
							  << logLevelToStr(logger->getPriorityLogLevel())
							  << std::endl;
				}
			} catch (const std::runtime_error &e) {
				print(e.what());
			}
		}
	}
}

/**
 * @brief Обработчик пользовательского ввода команды log
 *
 * @param args Вектор строк. Аргументы пользователя
 * Например {"log", "message", "INFO"}
 * @throw std::invalid_argument если некорректные аргументы
 */
void Client::log(std::vector<std::string> args) {
	std::lock_guard<std::mutex> lk(mutex);

	if (args.size() == 3) {
		LogLevel loglevel;
		loglevel = strToLogLevel(args[2]);
		queue_tasks.push({args[1], loglevel});
		cv.notify_all();
	} else if (args.size() == 2) {
		queue_tasks.push({args[1], LogLevel::INFO});

		std::cout
			<< "Вы не указали аргумент уровня важности сообщения. Выбран  "
			<< GREEN("INFO") << std::endl;

		cv.notify_all();
	} else {
		throw std::invalid_argument(RED("Ошибка.") +
									" Некорректное число аргументов");
	}
}

/**
 * @brief Обработчик пользовательского ввода команды cdp
 * Меняет приоритет сообщений у логгера.
 *
 * @param args Вектор строк. Аргументы пользователя
 * Например {"cdp", "INFO"}
 * @throw std::invalid_argument если некорректные аргументы
 */
void Client::changePriorityLogLevel(std::vector<std::string> args) {
	std::lock_guard<std::mutex> lk(mutex);
	if (args.size() == 2) {
		LogLevel loglevel;
		try {
			loglevel = strToLogLevel(args[1]);
		} catch (std::invalid_argument &) {
			throw std::invalid_argument(
				RED("Ошибка.") +
				"Неизвестный тип LogLevel. Текущий приоритет: " +
				logLevelToStr(logger->getPriorityLogLevel()));
		}

		logger->setPriorityLogLevel(loglevel);
	} else {
		throw std::invalid_argument(
			RED("Ошибка.") + " Не хватает аргументов для команды cdp" +
			" Текущий приоритет: " +
			logLevelToStr(logger->getPriorityLogLevel()));
	}
}
