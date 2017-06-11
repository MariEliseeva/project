#pragma once

#include <iostream>
#include <unordered_map>
#include <stdint.h>
#include <vector>
#include <set>

const size_t MAX_LEN = 1000;
const size_t MAX_ARGS = 25;
class avr {
  public:
    avr(size_t _memorySize);
    size_t perform(std::string &values, std::pair<std::string, std::string> &comand, std::set<uint16_t> &changedRegisters, bool quite);
    int8_t get(int t);
    size_t memorySize;
    friend class AvrTest;
  private:
    void doAction(std::string action, std::unordered_map<std::string, int> &operandValues, std::set<uint16_t> &changedRegisters);
    int getValue(std::string argument, std::unordered_map<std::string, int> &operandValues);

    int stackGetValue();
    int SREGGetValue (std::string argument, std::unordered_map<std::string, int> &operandValues);
    int regBitGetValue(std::string argument, std::unordered_map<std::string, int> &operandValues);
    int defSREGGetValue(std::string argument);
    int regGetValue (std::string argument);
    int constGetValue (std::string argument, std::unordered_map<std::string, int> &operandValues);

    void setValue(std::string left, int value, std::unordered_map<std::string, int> &operandValues,
                  std::set<uint16_t> &changedRegisters);
    void memSetValue(std::string left, int value, std::unordered_map<std::string, int> &operandValues, 
                     std::set<uint16_t> &changedRegisters);
    void SREGSetValue(std::string left, int value, std::unordered_map<std::string, int> &operandValues);
    void setRegValue(std::string left, int value, 
                     std::set<uint16_t> &changedRegisters);
    void defSREGSetValue(std::string left, int value);
    void setRegBitValue(std::string left, int value, std::unordered_map<std::string, int> &operandValues, 
                        std::set<uint16_t> &changedRegisters);
    int operEvaluate(std::string expr, std::unordered_map<std::string, int> &operandValues, std::set<uint16_t> &changedRegisters);

    void fillOperandMap(std::string values, 
                        std::unordered_map<std::string, int> &operandValues,
                        std::string *operandNumber, std::set<uint16_t> &changedRegisters);

  	std::vector <int8_t> memory = std::vector<int8_t>(64, 0);
    size_t PC = 0;
    size_t SP = 63;
};

void makeOperandMap(std::string args, 
                        std::unordered_map<std::string, int> &operandValues,
                        std::string *operandNumber);


void fillActionsVector(std::string comands, std::vector<std::string> &actions);


int operationPriority(char c);

class avrException {
 public:
  avrException(const std::string problem) : _problem(problem){};

  std::string what() const{
    return _problem;
  }

 private:
  const std::string _problem;
};


