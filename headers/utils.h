#pragma once

#include <vector>
#include <string>

std::vector<std::string> split(std::string str, const char& separator) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(separator, start);
    while (end != std::string::npos) {
        if (end > start) { 
            tokens.push_back(str.substr(start, end - start));
        }
        start = end + 1;
        end = str.find(separator, start);
    }
    if (start < str.size()) {
        tokens.push_back(str.substr(start));
    }
    return tokens;
}
