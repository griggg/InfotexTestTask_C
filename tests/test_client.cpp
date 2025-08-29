#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <mutex>
#include "../headers/logger.h"
#include "../headers/utils.h"
#include "../headers/client.h"
#include "../headers/LogLevel.h"
#include "../headers/MockLogger.h"
#include <memory>




void testValidLogging() {
    auto mockLogger = std::make_shared<MockLogger>();
    Client client(mockLogger);

    client.log({"log", "Hello world", "WARNING"});
    client.log({"log", "Default level message"}); 

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
        std::lock_guard<std::mutex> lock(mockLogger->mtx);
        assert(mockLogger->logs.size() == 2);
        assert(mockLogger->logs[0].first == "Hello world");
        assert(mockLogger->logs[0].second == LogLevel::WARNING);

        assert(mockLogger->logs[1].first == "Default level message");
        assert(mockLogger->logs[1].second == LogLevel::INFO);
    }
    std::cout << "testValidLogging passed\n";
}

void testInvalidLogLevelInLog() {
    auto mockLogger = std::make_shared<MockLogger>();
    Client client(mockLogger);

    try {
        client.log({"log", "Message with bad level", "BADLEVEL"});
    } catch (...) {
        std::cout << "testInvalidLogLevelInLog пройден\n";
        return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
        std::lock_guard<std::mutex> lock(mockLogger->mtx);
        assert(mockLogger->logs.empty());
    }
    std::cout << "testInvalidLogLevelInLog пройден\n";
}

void testChangePrioritySuccessAndFailure() {
    auto mockLogger = std::make_shared<MockLogger>();
    Client client(mockLogger);

    bool res = client.changePriorityLogLevel({"cdp", "ERROR"});
    assert(res);
    assert(mockLogger->getPriorityLogLevel() == LogLevel::ERROR);

    res = client.changePriorityLogLevel({"cdp", "NONEXISTENT"});
    assert(!res);
    // Приоритет не должен был измениться
    assert(mockLogger->getPriorityLogLevel() == LogLevel::ERROR);

    std::cout << "testChangePrioritySuccessAndFailure пройден\n";
}

void testLogWithoutMessage() {
    auto mockLogger = std::make_shared<MockLogger>();
    Client client(mockLogger);

    // Попытка залогировать без сообщения
    client.log({"log"});

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    {
        std::lock_guard<std::mutex> lock(mockLogger->mtx);
        // Логов не должно быть
        assert(mockLogger->logs.empty());
    }

    std::cout << "testLogWithoutMessage пройден\n";
}

void testEmptyCommand() {
    auto mockLogger = std::make_shared<MockLogger>();
    Client client(mockLogger);

    // Пустой ввод
    client.log({});

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    {
        std::lock_guard<std::mutex> lock(mockLogger->mtx);
        assert(mockLogger->logs.empty());
    }

    std::cout << "testEmptyCommand пройден\n";
}

void testFileError() {

    Logger logger("non_existent_dir/test.log", LogLevel::INFO);

    bool result = logger.log("Test message", LogLevel::INFO);

    assert(!result);

    std::cout << "testFileError пройден\n";
}

int main() {
    testValidLogging();
    testInvalidLogLevelInLog();
    testChangePrioritySuccessAndFailure();
    testLogWithoutMessage();
    testEmptyCommand();
    testFileError();

    std::cout << "Все тесты пройдены!\n";
    return 0;
}
