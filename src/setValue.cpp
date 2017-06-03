#include "avr.h"

void avr::memSetValue(std::string left, int value, std::unordered_map<std::string, int> &operandValues, std::set<uint16_t> &changedRegisters) {
    std::string expr;
    size_t brackets = 1;
    size_t i = 1;

    for (; brackets != 0; i++) {
        if (left[i] == ')') brackets--;
        if (left[i] == '(') brackets++;
        if (brackets != 0) expr += left[i];
    }

    if (i != left.size()) throw avrException("Unknown argument in comands description file.");

    memory[operEvaluate(expr, operandValues, changedRegisters)] = value;
}

void avr::SREGSetValue(std::string left, int value, std::unordered_map<std::string, int> &operandValues) {
    std::string bitNumber;

    size_t i = 0;

    for (i = 5; left[i] != ')'; i++) {
        bitNumber += left[i];
    }

    if (i != left.size() - 1) throw avrException("Unknown argument in comands description file.");

    if (operandValues.count(bitNumber) == 0)  {
    	try {
            if (value) memory[63] |= (1 << stoi(bitNumber));
            else memory[63] = memory[63] & ~(1 << stoi(bitNumber));
    	} catch (std::invalid_argument) {
            throw avrException("Unknown argument in comands description file.");
    	}    	
    }
         
    if (value) memory[63] |= (1 << operandValues[bitNumber]);
    else memory[63] = memory[63] & ~(1 << operandValues[bitNumber]);
}

void avr::defSREGSetValue(std::string left, int value) {
    size_t bitMove;

    if (left == "C") bitMove = 0;
    else if (left == "Z") bitMove = 1;
    else if (left == "N") bitMove = 2;
    else if (left == "V") bitMove = 3;
    else if (left == "S") bitMove = 4;
    else if (left == "H") bitMove = 5;
    else if (left == "T") bitMove = 6;
    else if (left == "I") bitMove = 7;

    if (value) memory[63] |= (1 << bitMove);
    else memory[63] = memory[63] & ~(1 << bitMove);   
}

void avr::setRegValue(std::string left, int value, std::set<uint16_t> &changedRegisters) {
    std::string regNumber = "";
    for (size_t i = 1; i < left.size(); i++) {
        regNumber += left[i];
    }
    try {
    	if ((size_t)stoi(regNumber) >= memorySize) throw avrException("Reference to a non-existent memory."); 
        memory[stoi(regNumber)] = value;
    } catch (std::invalid_argument) {
        throw avrException("Unknown argument in comands description file.");
    }  
    changedRegisters.insert(stoi(regNumber));
}

void avr::setRegBitValue(std::string left, int value, 
	std::unordered_map<std::string, int> &operandValues, std::set<uint16_t> &changedRegisters) {
    size_t i = 0;

    if (left[2] == '(')  i = 3;
    else i = 4;

    std::string regNumber = "";
    for (size_t j = 0; j < i - 1; j++) regNumber += left[j];

    std::string bitNumber = "";
    for (size_t j = i; j < left.size() - 1; j++) bitNumber += left[j];
                
    if (operandValues.count(bitNumber) == 0) {
        if (operandValues.count(regNumber) == 0) {
        	try {
                regNumber[0] = '0';
                changedRegisters.insert(stoi(regNumber));    
        	    regNumber = "";
                for (size_t j = 1; j < i - 1; j++) regNumber += left[j];
                if ((size_t)stoi(regNumber) >= memorySize) throw avrException("Reference to a non-existent memory.");
                if (value) memory[stoi(regNumber)] |= (1 << stoi(bitNumber));
                else memory[stoi(regNumber)] = 
                    memory[stoi(regNumber)] & ~(1 << stoi(bitNumber));
                return;
    	    } catch (std::invalid_argument) {
                throw avrException("Unknown argument in comands description file.");
    	    }
    	}

    	changedRegisters.insert(operandValues[regNumber]);    

        if ((size_t)operandValues[regNumber] >= memorySize) throw avrException("Reference to a non-existent memory.");
    	try {
        	if (value) memory[operandValues[regNumber]] |= (1 << stoi(bitNumber));
            else memory[operandValues[regNumber]] = 
                memory[operandValues[regNumber]] & ~(1 << stoi(bitNumber));
            return;
    	} catch (std::invalid_argument) {
            throw avrException("Unknown argument in comands description file.");
    	}
    }   

    if (operandValues.count(regNumber) == 0) {
    	try {
            regNumber[0] = '0';
            changedRegisters.insert(stoi(regNumber));    
      	    regNumber = "";
            for (size_t j = 1; j < i - 1; j++) regNumber += left[j];
            if ((size_t)stoi(regNumber) >= memorySize) throw avrException("Reference to a non-existent memory.");
            if (value) memory[stoi(regNumber)] |= (1 << operandValues[bitNumber]);
            else memory[stoi(regNumber)] = 
                memory[stoi(regNumber)] & ~(1 << operandValues[bitNumber]);
            return;
        } catch (std::invalid_argument) {
            throw avrException("Unknown argument in comands description file.");
        }
    }

    changedRegisters.insert(operandValues[regNumber]);    

    if ((size_t)operandValues[regNumber] >= memorySize) throw avrException("Reference to a non-existent memory.");
    try {
        if (value) memory[operandValues[regNumber]] |= (1 << operandValues[bitNumber]);
        else memory[operandValues[regNumber]] = 
            memory[operandValues[regNumber]] & ~(1 << operandValues[bitNumber]);
    } catch (std::invalid_argument) {
        throw avrException("Unknown argument in comands description file.");
    }

    
}

void avr::setValue(std::string left, int value, std::unordered_map<std::string, int> &operandValues, std::set<uint16_t> &changedRegisters) {
    //Присваивает значение в регистр/флаг/по адресу в памяти/в стек

    if (left[0] == '(') {
        memSetValue(left, value, operandValues, changedRegisters);
        return;
    }

    if (left == "STACK") {
        memory[SP] = value;
        changedRegisters.insert(SP);
        SP--;
        return;
    }

    if (left[0] == 'S' && left[1] == 'R' && 
        left[2] == 'E' && left[3] == 'G' && left[4] == '(') {
        SREGSetValue(left, value, operandValues);
        changedRegisters.insert(63);
        return;
    }

    if (left == "C" || left == "Z" || left == "N" || left == "V" ||
        left == "S" || left == "H" || left == "T" || left == "I") {
        defSREGSetValue(left, value);
        changedRegisters.insert(63); 
        return;
    }

    if (left == "RX") {
        memory[26] = value >> 8;
        memory[27] = value & -1;
        changedRegisters.insert(26);
        changedRegisters.insert(27);
        return;
    }

    if (left == "RY") {
        memory[28] = value >> 8;
        memory[29] = value & -1;
        changedRegisters.insert(28);
        changedRegisters.insert(29);
        return;
    }

    if (left == "RZ") {
        memory[30] = value >> 8;
        memory[31] = value & -1;
        changedRegisters.insert(30);
        changedRegisters.insert(31);
        return;
    }

    if (left == "PC") {
        PC = value;
        return;
    }

    if (left == "Rr" || left == "Rd") {
    	if (operandValues.count(left) == 0) throw avrException("Unknown argument in comands description file.");
        memory[operandValues[left]] = value;
        changedRegisters.insert(operandValues[left]);
        return;
    }

    if (left[0] == 'R' && left[1] >='0' && left[1] <= '9') {
        setRegValue(left, value, changedRegisters);
        return;       
        }

    if (left[0] == 'R' && ((left.size() >= 3 && left[2] == '(') 
                            || (left.size() >= 4 && left[3] == '('))) {
        setRegBitValue(left, value, operandValues, changedRegisters);
        return;
    }
}