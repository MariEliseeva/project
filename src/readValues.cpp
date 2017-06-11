#include "avr.h"

void makeOperandMap(std::string args, 
                        std::unordered_map<std::string, int> &operandValues,
                        std::string *operandNumber) {
    size_t argsQuantity = 0;
    std::string name = "";

    for (size_t i = 0; i < args.length(); i++) {
        if (args[i] == ',') {
            if (!(name[0] == 'R' && name.size() == 2 
                  && name[1] >= 'a' && name[1] <= 'z') 
                && name != "K8" && name != "K6"
                && name != "k" && name != "q"
                && name != "s" && name != "P") {
                throw avrException("Wrong name of argument in comands description file.");
            }
            operandValues[name] = 0;
            operandNumber[argsQuantity] = name;
            name = "";
            argsQuantity++;
        } else {
            name += args[i];
        }
    }
    if (name != "") {
        if (!(name[0] == 'R' && name.size() == 2 
              && name[1] >= 'a' && name[1] <= 'z') 
          	&& name != "K8" && name != "K6"
            && name != "k" && name != "q"
            && name != "s" && name != "P") {
            throw avrException("Wrong name of argument in comands description file.");
        }
        operandValues[name] = 0;
        operandNumber[argsQuantity] = name;     
    }     
}

void avr::fillOperandMap(std::string values, 
                        std::unordered_map<std::string, int> &operandValues,
                        std::string *operandNumber, std::set<uint16_t> &changedRegisters) {
    size_t operNumber = 0;
    std::string operand = "";
    int value, regNumber = 0;
    std::unordered_map<std::string, int> emptyMap; 
    //На даном этапе при вычислении значени выражения нельзя использовать что-то, кроме констант.
                        
    for (size_t i = 0; i < values.length(); i++) {
        if (values[i] == ',') {

            if (!(operand[0] == 'R' && operand[1] >= '0' && operand[1] <= '9' &&
            	(operand.size() == 2 || (operand.size() == 3 
            		&& operand[2] >= '0' && operand[2] <= '9')))) {
                value = operEvaluate(operand, emptyMap, changedRegisters); 
                //Если передавали константу/константное выражение, то посчитаем значение (expression.cpp)
            } else {
                value = regNumber;
                //Если передавали регистр - получаем номер регистра.
            }
                regNumber = 0;
                operandValues[operandNumber[operNumber]] = value;
                operNumber++;
                operand = "";
        } else {
            operand += values[i];
            if (values[i] >= '0' && values[i] <= '9') {
                regNumber = 10 * regNumber + values[i] - '0';
            } //если передаётся номер регистра, то тут этот номер посчитается.
        }
    }

    if (operand[0] != 'R' ) {
        value = operEvaluate(operand, operandValues, changedRegisters);
    } else {
        value = regNumber;
    }

    operandValues[operandNumber[operNumber]] = value;    
}

void fillActionsVector(std::string comands, std::vector<std::string> &actions) {
    std::string action = "";

    for (size_t i = 0; i < comands.length(); i++) {
        if (comands[i] == ',') {
            actions.push_back(action);
            action = "";
    } else {
            action += comands[i];
        }
    }
    actions.push_back(action);
}
