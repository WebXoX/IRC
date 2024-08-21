#include "../inc/utils.hpp"

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::string substr;
    std::istringstream ss(str);
    while (std::getline(ss, substr, delimiter)) {
        tokens.push_back(substr);
    }
    return tokens;
}

std::string trimFunction(std::string str)
{
    size_t first = str.find_first_not_of(" \n\r\t");
    size_t last = str.find_last_not_of(" \n\r\t");

    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }

    return str.substr(first, (last - first + 1));
}