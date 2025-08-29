#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "../headers/Client.h"
#include "../headers/LogLevel.h"
#include "../headers/Logger.h"
#include "../headers/MockLogger.h"
#include "../headers/Utils.h"

void testValidLogging() {
	auto mockLogger = std::make_shared<MockLogger>();
	Client client(mockLogger);

	try {
		client.log({"log", "Hello world", "WARNING"});
		client.log({"log", "Default level message"});
	} catch (...) {
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	{
		std::lock_guard<std::mutex> lock(mockLogger->mtx);
		assert(mockLogger->logs.size() == 2);
		assert(mockLogger->logs[0].first == "Hello world");
		assert(mockLogger->logs[0].second == LogLevel::WARNING);
		assert(mockLogger->logs[1].first == "Default level message");
		assert(mockLogger->logs[1].second == LogLevel::INFO);
	}

	std::cout << "testValidLogging пройден\n";
}

void testInvalidLogLevelInLog() {
	auto mockLogger = std::make_shared<MockLogger>();
	Client client(mockLogger);

	try {
		client.log({"log", "Message with bad level", "BADLEVEL"});
	} catch (...) {
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	{
		std::lock_guard<std::mutex> lock(mockLogger->mtx);
		assert(mockLogger->logs.empty());
	}

	std::cout << "testInvalidLogLevelInLog пройден\n";
}

void testChangePrioritySuccess() {
	auto mockLogger = std::make_shared<MockLogger>();
	Client client(mockLogger);

	client.changePriorityLogLevel({"cdp", "ERROR"});

	assert(mockLogger->getPriorityLogLevel() == LogLevel::ERROR);

	std::cout << "testChangePrioritySuccess пройден\n";
}

void testChangePriorityThrowsOnInvalid() {
	auto mockLogger = std::make_shared<MockLogger>();
	Client client(mockLogger);

	try {
		client.changePriorityLogLevel({"cdp", "NONEXISTENT"});
		assert(false);
	} catch (const std::invalid_argument& e) {
		std::cout << "testChangePriorityThrowsOnInvalid пройден: " << "\n";
	}
}

void testChangePriorityThrowsOnMissingArgs() {
	auto mockLogger = std::make_shared<MockLogger>();
	Client client(mockLogger);

	try {
		client.changePriorityLogLevel({"cdp"});
		assert(false);
	} catch (const std::invalid_argument& e) {
		std::cout << "testChangePriorityThrowsOnMissingArgs пройден: " << "\n";
	}
}

void testLogWithoutMessage() {
	auto mockLogger = std::make_shared<MockLogger>();
	Client client(mockLogger);

	try {
		client.log({"log"});
	} catch (...) {
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	{
		std::lock_guard<std::mutex> lock(mockLogger->mtx);
		assert(mockLogger->logs.empty());
	}

	std::cout << "testLogWithoutMessage пройден\n";
}

void testEmptyCommand() {
	auto mockLogger = std::make_shared<MockLogger>();
	Client client(mockLogger);

	try {
		client.log({});
		assert(false);
	} catch (std::invalid_argument& e) {
		std::cout << "testEmptyCommand пройден\n";
	}
}

void testLoggerInit() {
	Client client;
	try {
		client.loggerInit("testlog.log INFO");
		std::cout << "testLoggerInit пройден\n";
	} catch (...) {
		assert(false);
	}
}

void testLoggerInitInvalidLevel() {
	Client client;
	try {
		client.loggerInit("testlog.log BADLEVEL");
		assert(false);
	} catch (const std::invalid_argument&) {
		std::cout << "testLoggerInitInvalidLevel пройден\n";
	}
}

void testLoggerLogInvalidSizeArgs() {
	Client client;
	client.loggerInit("testlog.log INFO");
	try {
		client.log({"testlog.log", "hello", "ERROR", "123"});
		assert(false);
	} catch (const std::invalid_argument&) {
		std::cout << "testLoggerLogInvalidSizeArgs пройден\n";
	}
}

int main() {
	testValidLogging();
	testInvalidLogLevelInLog();
	testChangePrioritySuccess();
	testChangePriorityThrowsOnInvalid();
	testChangePriorityThrowsOnMissingArgs();
	testLogWithoutMessage();
	testEmptyCommand();
	testLoggerInit();
	testLoggerInitInvalidLevel();
	testLoggerLogInvalidSizeArgs();

	std::cout << "Все тесты пройдены!\n";
	return 0;
}
