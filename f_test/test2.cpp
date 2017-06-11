#include <fstream>
#include <iostream>
#include <sstream>

#include "avr.h"

int main() {
    std::ifstream comandsFile;
    std::string comandsFileName = "f_test/comands";
    comandsFile.open (comandsFileName); 
    std::string name, variables, actions;

    size_t memorySize;
    float oneTactTime;
    comandsFile >> memorySize >> oneTactTime;

    avr em(memorySize);

    std::unordered_map <std::string, std::pair<std::string, std::string>> comands;
    std::unordered_map <std::string, int> tacts;
    
    while (!comandsFile.eof()) {
        comandsFile >> name >> variables >> actions >> tacts[name];
        comands[name] = {variables, actions};
    }

    std::ifstream codeFile;
    std::string codeFileName = "f_test/code";
    codeFile.open (codeFileName);
    
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

    std::set <uint16_t>changedRegisters;
    int tactsNumber = 0;
    bool quite = 1;

    
    while (currentLine < lineNumber) {
        try {
            currentLine = em.perform(code[currentLine].second, comands[code[currentLine].first], changedRegisters, quite);
            tactsNumber += tacts[code[currentLine].first];
        }
        catch (const avrException exc) {
            std::cout << exc.what() << '\n' << "Error in line " << currentLine + 1 << '\n' << "Test failed.\n";
            return 1;
        }
    }

    std::cout << "Test passed.\n";

    return 0;
}