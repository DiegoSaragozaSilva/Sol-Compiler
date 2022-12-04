#include "PrePro.hpp"

std::string PrePro::filter(std::string str) {
    std::string filteredString;
    std::istringstream strStream(str);
    for (std::string line; std::getline(strStream, line); ) {
        int position = 0;
        while (line[position] != '\0') {
            if (line[position] == '/' && line[position + 1] == '/')
                break;
            position++;
        }
        filteredString += line.substr(0, position) + '\n';
    }
    return filteredString;
}
