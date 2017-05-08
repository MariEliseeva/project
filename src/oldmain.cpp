#include <fstream>

#include "avr.h"

int main(int argc, char** argv) {
	if (argc == 0) return 0;
    std::unordered_map <std::string, std::pair<std::string, std::string>> comands;

    std::ifstream comandsFile;
    comandsFile.open (argv[1]);

    std::string name, variables, actions;

    size_t memorySize;
    comandsFile >> memorySize;

    avr em(memorySize);

    while (!comandsFile.eof()) {
        comandsFile >> name >> variables >> actions;
        comands[name] = {variables, actions};
    }

    std::ifstream codeFile;
    codeFile.open (argv[2]);

    std::string values;
    std::unordered_map <size_t, std::pair<std::string, std::string>> code;
    size_t lineNumber = 0;

    while (!codeFile.eof()) {
        codeFile >> name;
        if (comands[name].first != "-") codeFile >> values;
        code[lineNumber] = {name, values};
        lineNumber++;
    }

    size_t currentLine = 0;

    while (currentLine < lineNumber) {
        currentLine = em.perform(code[currentLine].second, comands[code[currentLine].first]);
    }

    return 0;
}