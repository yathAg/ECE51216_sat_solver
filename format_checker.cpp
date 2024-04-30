#include <iostream>
#include <string>
#include <sstream>
#include <vector>

bool checkAssignmentFormat(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    if (!(iss >> token) || token != "ASSIGNMENT:") {
        return false;
    }
    while (iss >> token) {
        size_t equal_pos = token.find('=');
        if (equal_pos == std::string::npos) {
            return false; // No '=' found in token
        }
        std::string var = token.substr(0, equal_pos);
        std::string value = token.substr(equal_pos + 1);
        for (char c : var) if (!isdigit(c)) return false;
        if (value != "0" && value != "1") return false;
    }
    return true;
}

int main() {
    std::string line;
    // Read the first line which should be "RESULT: SAT" or "RESULT: UNSAT"
    if (!std::getline(std::cin, line)) {
        std::cout << "Error: No output to read." << std::endl;
        return 1;
    }

    if (line == "RESULT: SAT") {
        if (!std::getline(std::cin, line)) {
            std::cout << "Error: Expected ASSIGNMENT after SAT result." << std::endl;
            return 1;
        }
        if (!checkAssignmentFormat(line)) {
            std::cout << "Error: Invalid assignment format." << std::endl;
            return 1;
        }
    } else if (line != "RESULT: UNSAT") {
        std::cout << "Error: Invalid result format. Must be 'RESULT: SAT' or 'RESULT: UNSAT'." << std::endl;
        return 1;
    }

    std::cout << "Format is correct." << std::endl;
    return 0;
}

