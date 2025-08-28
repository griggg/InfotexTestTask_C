#include "../headers/logger.h"
#include "../headers/utils.h"
#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <iostream>




void TestWarningPriority() {
    /*
        Тест проверяет корректность обработки уровня важности
    */
    std::cout << "TestWarningPriority" << std::endl;

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
    std::vector<std::string> ln1 = split(lines[0], log.wordSeparator);
    std::vector<std::string> ln2 = split(lines[1], log.wordSeparator);

    assert(ln1[0] == "сообщение 2");
    assert(ln2[0] == "сообщение 3");


}

void TestInfoPriority() {
    std::cout << "TestInfoPriority" << std::endl;

    Logger log("log2.txt", LogLevel::INFO);

    log.log("сообщение 1", LogLevel::INFO);
    log.log("сообщение 2", LogLevel::WARNING);
    log.log("сообщение 3", LogLevel::ERROR);

    std::ifstream file("log2.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    assert(lines.size() == 3);
    std::vector<std::string> ln1 = split(lines[0], log.wordSeparator);
    std::vector<std::string> ln2 = split(lines[1], log.wordSeparator);
    std::vector<std::string> ln3 = split(lines[2], log.wordSeparator);

    std::cout << ln1[0] << "V" << std::endl;

    assert(ln1[0] == "сообщение 1");
    assert(ln2[0] == "сообщение 2");
    assert(ln3[0] == "сообщение 3");

}

void TestErrorPriority() {
    std::cout << "TestErrorPriority" << std::endl;


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
    std::vector<std::string> ln1 = split(lines[0], log.wordSeparator);

    assert(ln1[0] == "сообщение 3");

}

void TestChangeDefaultLogLevel() {
    
}

int main() {
    TestWarningPriority();
    TestInfoPriority();
    TestErrorPriority();
}