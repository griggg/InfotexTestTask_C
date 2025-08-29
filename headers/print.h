#pragma once
#include <string>
#include <iostream>

// #define RED(text)    ("\033[31m" text "\033[0m")
// #define GREEN(text)  ("\033[32m" text "\033[0m")
// #define BLUE(text)   ("\033[34m" text "\033[0m")
// #define YELLOW(text) ("\033[33m" text "\033[0m")

inline std::string RED(const std::string& text) {
    return "\033[31m" + text + "\033[0m";
}

inline std::string GREEN(const std::string& text) {
    return "\033[32m" + text + "\033[0m";
}

inline std::string BLUE(const std::string& text) {
    return "\033[34m" + text + "\033[0m";
}

inline std::string YELLOW(const std::string& text) {
    return "\033[33m" + text + "\033[0m";
}

static void print(std::string m) {
    std::cout << m << std::endl;
}