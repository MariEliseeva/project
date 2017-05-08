#include "avr.h"

int avr::stackGetValue() {
    SP++;
    if (SP >= memorySize) throw avrException("Reference to a non-existent memory.");
    return memory[SP];
}

int avr::SREGGetValue (std::string argument, std::unordered_map<std::string, int> &operandValues) {
    std::string bitNumber;

    size_t i = 0;

    for (i = 5; argument[i] != ')'; i++) {
        bitNumber += argument[i];
    }

    if (i != argument.size() - 1) throw avrException("Unknown argument in comands description file.");

    if (operandValues.count(bitNumber) == 0)  {
    	try {
            return (memory[63] >> (stoi(bitNumber) & ((1 << 8) - 1))) & 1;
    	} catch (std::invalid_argument) {
            throw avrException("Unknown argument in comands description file.");
    	}    	
    }
    
    return (memory[63] >> (operandValues[bitNumber] & ((1 << 8) - 1))) & 1;
}

int avr::regBitGetValue(std::string argument, std::unordered_map<std::string, int> &operandValues) {
    size_t i = 0;

    if (argument[2] == '(')  i = 3;
    else i = 4;

    std::string regNumber = "";
    for (size_t j = 0; j < i - 1; j++) regNumber += argument[i];

    std::string bitNumber = "";
    size_t brackets = 1;
    for (; brackets != 0; i++) {
        if (argument[i] == ')') brackets--;
        if (argument[i] == '(') brackets++;
        if (brackets != 0) bitNumber += argument[i];
    }
    if (i != argument.size()) throw avrException("Unknown argument in comands description file.");


    if (operandValues.count(bitNumber) == 0) {
        if (operandValues.count(regNumber) == 0) {
            try {
        	    regNumber = "";
                for (size_t j = 1; j < i - 1; j++) regNumber += argument[i];
                if ((size_t)stoi(regNumber) >= memorySize) throw avrException("Reference to a non-existent memory.");
                return (memory[stoi(regNumber)] >> (stoi(bitNumber) & ((1 << 8) - 1))) & 1;
    	    } catch (std::invalid_argument) {
                throw avrException("Unknown argument in comands description file.");
    	    }
    	}

        if ((size_t)operandValues[regNumber] >= memorySize) throw avrException("Reference to a non-existent memory.");
    	try {
        	    return (memory[operandValues[regNumber]] >> (stoi(bitNumber) & ((1 << 8) - 1))) & 1;
    	    } catch (std::invalid_argument) {
                throw avrException("Unknown argument in comands description file.");
    	    }
    }   

    if (operandValues.count(regNumber) == 0) {
        try {
      	    regNumber = "";
            for (size_t j = 1; j < i - 1; j++) regNumber += argument[i];
            if ((size_t)stoi(regNumber) >= memorySize) throw avrException("Reference to a non-existent memory.");
            return (memory[stoi(regNumber)] >> (operandValues[bitNumber] & ((1 << 8) - 1))) & 1;
        } catch (std::invalid_argument) {
            throw avrException("Unknown argument in comands description file.");
        }
    }

    if ((size_t)operandValues[regNumber] >= memorySize) throw avrException("Reference to a non-existent memory.");
    try {
            return (memory[operandValues[regNumber]] >> (operandValues[bitNumber] & ((1 << 8) - 1))) & 1;
    	} catch (std::invalid_argument) {
            throw avrException("Unknown argument in comands description file.");
    	}
}

int avr::defSREGGetValue(std::string argument) {
    if (argument == "C") {
        return memory[63] & 1;
    }

    if (argument == "Z") {
        return (memory[63] >> 1) & 1;
    }


    if (argument == "N") {
        return (memory[63] >> 2) & 1;
    }

    if (argument == "V") {
        return (memory[63] >> 3) & 1;
    }

    if (argument == "S") {
        return (memory[63] >> 4) & 1;
    }

    if (argument == "H") {
        return (memory[63] >> 5) & 1;
    }

    if (argument == "T") {
        return (memory[63] >> 6) & 1;
    }

    if (argument == "I") {
        return (memory[63] >> 7) & 1;
    }

    return 0;
}

int avr::regGetValue (std::string argument) {
    size_t Number = 0;

    for (size_t i = 1; i < argument.size(); i++) {
        Number += argument[i] - '0';
        if (argument[i] < '0' || argument[i] > '9') throw avrException("Unknown argument in comands description file.");
    }

    if (Number >= memorySize) throw avrException("Reference to a non-existent memory.");

    return memory[Number];
}

int avr::constGetValue (std::string argument, std::unordered_map<std::string, int> &operandValues) {
	if (operandValues.count(argument) == 0) throw avrException("Unknown argument in comands description file.");

    if (argument == "K8") return operandValues[argument] & ((1 << 8) - 1);

    else if (argument == "K6" || argument == "q" || argument == "P")
         return operandValues[argument] & ((1 << 6) - 1);

    else if (argument == "s" || argument == "b") return operandValues[argument] & ((1 << 3) - 1);

    else return operandValues[argument];
}
int avr::getValue(std::string argument, std::unordered_map<std::string, int> &operandValues) {
    //если просто число, то оно и возвращается. 

	bool isNumberExpr = true;    

    for (size_t i = 0; i < argument.size(); i++) {
        if (argument[i] < '0' || argument[i] > '9') {
            isNumberExpr = false;
        }
    }

    if  (isNumberExpr && argument != "") {
        return stoi(argument);
    }

    //Может быть значение на вершине стека, из регистра с флагами, 
    //из регистров общего назначения, из памяти, значение констант и т.д.

    if (argument == "STACK") return stackGetValue();


    if (argument[0] == 'S' && argument[1] == 'R' && 
        argument[2] == 'E' && argument[3] == 'G' && argument[4] == '(') 
        return SREGGetValue(argument, operandValues); //обращение к флагам через SREG

    if (argument[0] == 'R' && argument[1] >= 'a' && argument[1] <= 'z' ) {
    	if (operandValues.count(argument) == 0) throw avrException("Unknown argument in comands description file.");
        return memory[operandValues[argument]]; //Регистры - аргументы команды
    }

    if (argument[0] == 'R' && ((argument.size() >= 3 && argument[2] == '(') 
                            || (argument.size() >= 4 && argument[3] == '('))) {
    	
        return regBitGetValue(argument, operandValues);//определённый бит регистра
	    
    }

    if (argument[0] == 'R' && argument[1] >= '0' && argument[1] <= '9') {
    	return regGetValue(argument);//регистр по номеру
    }

    if (argument == "K8" || argument == "k" || 
        argument == "K6" || argument == "q" || 
        argument == "s" || argument == "P" || 
        argument == "b") {
        return constGetValue(argument, operandValues); //константы - аргументы команды
    } 

    if (argument == "C" || argument == "Z" || argument == "N" || argument == "V" ||
        argument == "S" || argument == "H" || argument == "T" || argument == "I")
        return defSREGGetValue(argument); //флаги по имени

    if (argument == "PC") {
        return PC; //программный счётчик
    }

    if (argument == "RX") {
        return (memory[26] << 8) + memory[27];
    }

    if (argument == "RY") {
        return (memory[28] << 8) + memory[29];
    }

    if (argument == "RZ") {
        return (memory[30] << 8) + memory[31];
    }
    //пары регистров - X,Y,Z  

    throw avrException("Unknown argument in comands description file.");
    return 0;
}