#include "logger.h"
#include <vector>
#include <string>
#include <fstream>
#include <cassert>

std::vector<std::string> split(std::string str) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(" ");
    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end-start));
        start = end + 1;
        end = str.find(" ");
    }
    tokens.push_back(str.substr(start, end-start));
    return tokens;
}


void TestWarningPriority() {
    /*
        Тест проверяет корректность обработки уровня важности
    */
    Logger log("log.txt", LogLevel::WARNING);

    log.log("сообщение 1", LogLevel::INFO);
    log.log("сообщение 2", LogLevel::WARNING);
    log.log("сообщение 3", LogLevel::ERROR);

    std::ifstream file("log.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    assert(lines.size() == 2);
    std::vector<std::string> ln1 = split(lines[0]);
    std::vector<std::string> ln2 = split(lines[1]);
    std::vector<std::string> ln3 = split(lines[2]);

    assert(ln1[0] == "сообщение 2");
    assert(ln2[0] == "сообщение 3");


}

void TestInfoPriority() {
    Logger log("log.txt", LogLevel::INFO);

    log.log("сообщение 1", LogLevel::INFO);
    log.log("сообщение 2", LogLevel::WARNING);
    log.log("сообщение 3", LogLevel::ERROR);

    std::ifstream file("log.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    assert(lines.size() == 3);
    std::vector<std::string> ln1 = split(lines[0]);
    std::vector<std::string> ln2 = split(lines[1]);
    std::vector<std::string> ln3 = split(lines[2]);

    assert(ln1[0] == "сообщение 1");
    assert(ln1[0] == "сообщение 2");
    assert(ln2[0] == "сообщение 3");

}

void TestErrorPriority() {
    Logger log("log.txt", LogLevel::ERROR);

    log.log("сообщение 1", LogLevel::INFO);
    log.log("сообщение 2", LogLevel::WARNING);
    log.log("сообщение 3", LogLevel::ERROR);

    std::ifstream file("log.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    assert(lines.size() == 1);
    std::vector<std::string> ln1 = split(lines[0]);
    std::vector<std::string> ln2 = split(lines[1]);
    std::vector<std::string> ln3 = split(lines[2]);

    assert(ln2[0] == "сообщение 3");

}

void TestChangeDefaultLogLevel() {
    
}
