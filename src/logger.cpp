#include "../headers/Logger.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "../headers/Print.h"

/**
 * @brief Устанавливает приоритет сообщений логгера
 * Сообщения с приоритетом ниже не будут записываться
 *
 * @param filename Строка название файла в который будут записываться логи
 * @param priorityLogLevel LogLevel приоритет сообщений. Сообщения с важностью
 * LogLevel меньше чем приоритет будут игнорироваться.
 * 
 * @throw std::runtime_error Если не удастся открыть файл для записи по 
 * переданному пути filename
 */
Logger::Logger(std::string filename, LogLevel priorityLogLevel) {
	this->filename = filename;
	this->priorityLogLevel = priorityLogLevel;
	file = std::ofstream(this->filename);
	if (!file.is_open()) {
		throw std::runtime_error(RED("Ошибка.") +
								 "Не удалось открыть файл по адресу" +
								 this->filename);
	}
}


/**
 * @brief Функция для получения текущего времени
 *
 * @return std::string строка, текущее время в
 * 					формате "%Y-%m-%d %H:%M:%S"
 */
std::string getCurrentTime() {
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	std::tm tm = *std::localtime(&time);

	std::stringstream ss;
	ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

/**
 * @brief Записывает лог в файл
 *
 * @param message сообщение для логирования
 * @param logLevel важность сообщения
 */
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

/**
 * @brief Устанавливает приоритет сообщений логгера
 * Сообщения с приоритетом ниже не будут записываться
 *
 * @param loglevel важность сообщения
 */
void Logger::setPriorityLogLevel(LogLevel loglevel) {
	this->priorityLogLevel = loglevel;
}

LogLevel Logger::getPriorityLogLevel() { return priorityLogLevel; }
Logger::~Logger() { file.close(); }
