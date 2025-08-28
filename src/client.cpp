#include <iostream>
#include <queue>
#include <string>
#include "../headers/logger.h"
#include "../headers/utils.h"
#include <memory>
#include <fstream>
#include <condition_variable>
#include <thread>

std::condition_variable cv;
std::mutex queue_mutex;
bool running = true;
std::queue<std::pair<std::string, LogLevel>> queue_tasks;

void print(std::string msg) {
    std::cout << msg << std::endl;
}

void worker(std::shared_ptr<Logger> logger) {
    while (running) {
        std::unique_lock<std::mutex> lk(queue_mutex);
        cv.wait(lk, []{
            return running == false || queue_tasks.empty() == false;
        });
        while (!queue_tasks.empty()) {
            auto [msg, loglevel] = queue_tasks.front();
            queue_tasks.pop();
            if (logger->log(msg, loglevel)) {
                print("Лог записан");
            } else {
                print("Лог не записан"); 
            }
        }
    }
}



std::shared_ptr<Logger> loggerInit(std::string line) {
    std::vector<std::string> argsLoggerInit = split(line, ' ');
    
    // std::ofstream file(argsLoggerInit[0]);
    // if (!file.is_open()) return nullptr;
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

int main() {

    print("Logger");
    print("Напишите <logFileName> <defaultLogLevel> (по умолч. INFO)");
    std::string line;
    std::getline(std::cin, line);
    std::shared_ptr<Logger> logger = loggerInit(line);
    
    std::thread queue_worker(worker, logger);
    queue_worker.detach();
    print("Меню логгера");
    print("log <message> <INFO/WARNING/ERROR>");
    print("cdl <INFO/WARNING/ERROR>");
    print("exit");
    print("");
    
    while (1) {
        std::getline(std::cin, line);
        std::vector<std::string> args = split(line, ' ');
        if (args.size() == 0) {
            // print("не получено аргументов");
            continue;
        }
        if (args[0] == "log") {
            if (args.size() == 3) {
                queue_tasks.push({args[1], strToLogLevel(args[2])});
                cv.notify_all();
            } else if (args.size() == 2) {
                queue_tasks.push({args[1], logger->getDefaultLogLevel()});
                print("Вы не указали аргумент уровня важности сообщения. Выбран" 
                    + logLevelToStr(logger->getDefaultLogLevel()));
                cv.notify_all();
            } else {
                print("Ошибка; отсутствует аргумент. log <message> <INFO/WARNING/ERROR>");
            }
        } else if (args[0] == "cdl") {
            if (args.size() == 2) {
                LogLevel loglevel;
                try
                {
                    loglevel = strToLogLevel(args[1]);
                }
                catch(std::invalid_argument)
                {
                    print("Неизвестный тип LogLevel. logger defaultLogLevel = "
                        + logLevelToStr(logger->getDefaultLogLevel()));
                    continue;
                }
                
                logger->setDefaultLogLevel(loglevel);
            }
        } else if (args[0] == "exit") {
            if (args.size() == 1) {
                running = false;
                cv.notify_all();
                break;
            }
        } else {
            continue;
        }
    }

}