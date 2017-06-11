#include <fstream>
#include <iostream>
#include <sstream>

#include "avr.h"

void writeInfo(std::set <uint16_t>changedRegisters, float time, avr &em) {
    std::cout << "\n\nINFO:\nChanged registers: ";
    for(auto it : changedRegisters) {
        std::cout << it << ' ';
    }
    std::cout << "\nTotal time (in microseconds): " << time << "\n-----\n\n";
    std::cout << "\n\n-----\n\nDo you want to see some registers? Print 'Y' if yes," 
                 <<"'n' if no or 'a' if you want to see all first 32 registers.\n\n";
    char ifInfo;
    bool end = false;   
    while (!end) {
        std::cin >> ifInfo;
        if (ifInfo == 'Y') {
            std::cout << "Print numbers of registers which you want to see: ";
            std::set <int> lines;

            int lineNumb = 0;    
            std::string lineWithNumbers;
            std::getline(std::cin, lineWithNumbers);
            std::getline(std::cin, lineWithNumbers);
        
            for (size_t i = 0; i < lineWithNumbers.size(); i++) {
                if (lineWithNumbers[i] >= '0' && lineWithNumbers[i] <= '9') {
                    lineNumb = lineNumb * 10 + lineWithNumbers[i] - '0';
                } else if (lineWithNumbers[i] == ' ' || lineWithNumbers[i] == '\n') {
                    lines.insert(lineNumb);
                    lineNumb = 0;
                } else {
                    std::cout << "Wrong symbol.\n";
                }
            }  
            lines.insert(lineNumb);

            for (auto t : lines) {
                if ((size_t)t > em.memorySize) std::cout << t << ": unknown register.\n";
                else std::cout << t << ": " << (int)em.get(t) << '\n';
            }
            std::cout <<  "\n-----\n\n";
            end = true;
        } else if (ifInfo == 'a') {
            for (size_t i = 0; i < 32; i++) {
                std::cout << i << ": " << (int)em.get(i) << '\n';    
            }
            std::cout <<  "\n-----\n\n";
            end = true;
        } else if (ifInfo == 'n') {
            end = true;
        } else {
            std::cout << "Unknown option\n";
        }
    }
}

int main() {
    std::cout << "\n\nThis is an AVR emulator.";

    
    bool endRead = false;
    std::ifstream comandsFile;
        
    while (!endRead) {
        std::cout << "\n\nEnter name of file with comands description: ";
        std::string comandsFileName;
        std::cin >> comandsFileName;

        comandsFile.open (comandsFileName);
        if (comandsFile.fail()) {
            std::cout << "Input file opening error. Try again";
        } else {
            endRead = true;
        }
    }

    


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

    endRead = false;
    std::ifstream codeFile;
        
    while (!endRead) {
        std::cout << "\nEnter name of file with code: ";

	    std::string codeFileName;
	    std::cin >> codeFileName;

        codeFile.open (codeFileName);
        if (codeFile.fail()) {
            std::cout << "Input file opening error. Try again";
        } else {
            endRead = true;
        }
    }

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
    char mode;

    std::cout << "\n\n--Modes--\n\n0 --run all comands from code-file\n\n" << 
                  "1 --run and stop after each line, until ENTER pressed\n\n" << 
                  "2 --run and stop after definite lines\n\n-----\n\n" << 
                  "Chose mode: ";

    std::cin >> mode;

    std::cout << "\n\nIf you want to see first 32 registers after every line - print 0, else - print 1: ";

    bool quite;
    std::cin >> quite;


    std::set <uint16_t>changedRegisters;
    float time = 0;
    int tactsNumber = 0;
    

    if (mode == '0') {
        while (currentLine < lineNumber) {
            try {
                currentLine = em.perform(code[currentLine].second, comands[code[currentLine].first], changedRegisters, quite);
                tactsNumber += tacts[code[currentLine].first];
            }
            catch (const avrException exc) {
                std::cout << exc.what() << '\n' << "Error in line " << currentLine + 1;
                return 1;
            }
        }
    } else if (mode == '1') {
        std::cout << "If you will want to stop performing line by line - print 'help'\n";
        char symbol;
        while (currentLine < lineNumber) {
            symbol = std::cin.get();
            if (symbol == 'h' && std::cin.get() == 'e' && 
                std::cin.get() == 'l' && std::cin.get() == 'p' && std::cin.get() == '\n') {
                bool end = false;
                std::cout << "\n\n--What you can do:--\n\n0 --continue\n\n" << 
                      "1 --continue without stopping\n\n" << 
                      "2 --ask some info\n\n" << 
                      "3 --stop everything and exit\n\n-----\n\n";
                      
                while (!end) {
                    std::cout << "Chose what to do: ";
                    char option;
                    std::cin >> option;
                    if (option == '0' && std::cin.get() == '\n') {
                        end = true;
                    } else if (option == '1' && std::cin.get() == '\n') {
                        while (currentLine < lineNumber) {
                            try {
                                currentLine = em.perform(code[currentLine].second, 
                                    comands[code[currentLine].first], changedRegisters, quite);
                                tactsNumber += tacts[code[currentLine].first];
                            }
                            catch (const avrException exc) {
                                std::cout << exc.what() << '\n' << "Error in line " << currentLine + 1;
                                return 1;
                            } 
                        }
                        end = true;
                    } else if (option == '2' && std::cin.get() == '\n') {
                        time = oneTactTime * tactsNumber;
                        writeInfo(changedRegisters, time, em);
                    } else if (option == '3' && std::cin.get() == '\n') {
                        std::cout << "\nend.\n";
                        return 0;
                    } else {
                       std::cout << "Unknown option\n";
                    }
                }
            } else if (symbol == '\n') {
                try {
                    currentLine = em.perform(code[currentLine].second, comands[code[currentLine].first], changedRegisters, quite);
                    tactsNumber += tacts[code[currentLine].first];
                }
                catch (const avrException exc) {
                    std::cout << exc.what() << '\n' << "Error in line " << currentLine + 1;
                    return 1;
                }
            } else {
                std::cout << "\npress ENTER if you want to countinue or print 'help' if you want to do sth else.\n";
                while (std::cin.get() != '\n');
            }
            
        }
    } else {
        std::cout << "Print lines after which you want to stop: ";
        std::set <int> lines;

        int lineNumb = 0;    
        std::string lineWithNumbers;
        std::getline(std::cin, lineWithNumbers);
        std::getline(std::cin, lineWithNumbers);
        
        for (size_t i = 0; i < lineWithNumbers.size(); i++) {
            if (lineWithNumbers[i] >= '0' && lineWithNumbers[i] <= '9') {
                lineNumb = lineNumb * 10 + lineWithNumbers[i] - '0';
            } else if (lineWithNumbers[i] == ' ' || lineWithNumbers[i] == '\n') {
                lines.insert(lineNumb);
                lineNumb = 0;
            } else {
                std::cout << "Wrong symbol.\n";
            }
        }  
        lines.insert(lineNumb);

        while (currentLine < lineNumber) {           
            try {
                currentLine = em.perform(code[currentLine].second, comands[code[currentLine].first], changedRegisters, quite);
                tactsNumber += tacts[code[currentLine].first];
            }
            catch (const avrException exc) {
                std::cout << exc.what() << '\n' << "Error in line " << currentLine + 1;
                return 1;
            }
             if (lines.count(currentLine)) {
                bool end = false;
                std::cout << "\n\n--What you can do:--\n\n0 --continue\n\n" << 
                      "1 --continue without stopping\n\n" << 
                      "2 --ask some info\n\n" << 
                      "3 --stop everything and exit\n\n-----\n\n";
                      
                while (!end) {
                    std::cout << "Chose what to do: ";
                    char option;
                    std::cin >> option;
                    if (option == '0' && std::cin.get() == '\n') {
                        end = true;
                    } else if (option == '1' && std::cin.get() == '\n') {
                        while (currentLine < lineNumber) {
                            try {
                                currentLine = em.perform(code[currentLine].second, 
                                    comands[code[currentLine].first], changedRegisters, quite);
                                tactsNumber += tacts[code[currentLine].first];
                            }
                            catch (const avrException exc) {
                                std::cout << exc.what() << '\n' << "Error in line " << currentLine + 1;
                            }
                        }
                        end = true;
                    } else if (option == '2' && std::cin.get() == '\n') {
                        time = oneTactTime * tactsNumber;
                        writeInfo(changedRegisters, time, em);
                        std::cout << "\n\n--What you can do:--\n\n0 --continue\n\n" << 
                      "1 --continue without stopping\n\n" << 
                      "2 --ask some info\n\n" << 
                      "3 --stop everything and exit\n\n-----\n\n";
                    } else if (option == '3' && std::cin.get() == '\n') {
                        std::cout << "\nend.\n";
                        return 0;
                    } else {
                       std::cout << "Unknown option\n";
                    }
                }   
            }        
        }
    }

    std::cout << "\n\n-----\n\nDo you want info? Print 'Y' if yes and 'n if no.\n\n";
    char ifInfo;
    std::cin >> ifInfo;
    if (ifInfo == 'Y') {
        time = oneTactTime * tactsNumber;
        writeInfo(changedRegisters, time, em);
    }

    std::cout << "\nend.\n";
    return 0;
}