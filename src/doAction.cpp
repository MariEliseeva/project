#include "avr.h"


void avr::doAction(std::string action, std::unordered_map<std::string, int> &operandValues, std::set<uint16_t> &changedRegisters) {
    //выполнение какого-то одного действия.

    std::string left = "", right = "";
    
    size_t i = 0;
    bool ifDo = true;
    if (action[0] == 'i' && action[1] == 'f') { 
        //Если есть условие, то здесь оно проверяется.
	    i += 3;
	    std::string expr;
	    size_t brackets = 1;
        for (; brackets != 0; i++) {
        	if (action[i] == ')') brackets--;
        	if (action[i] == '(') brackets++;
        	if (brackets != 0) expr += action[i];
        }

        ifDo = operEvaluate(expr, operandValues, changedRegisters);//expression.cpp
    }
    if (ifDo) {
        //делим выражение на правую и левую часть. Правая - выражение, левая - куда его значение присвоить.
        for (; action[i] != '=' && i < action.size(); i++) {
            left += action[i];
        }

        if (i == action.size()) throw avrException("Wrong instruction.");
        
        for (i = i + 1; i < action.size(); i++) {
            right += action[i];
        }

        int value = 0;


        if (right[0] == '(' && right[right.size() - 1] == ')') {
            if ((size_t)operEvaluate(right, operandValues, changedRegisters) >= memorySize) throw avrException("Reference to a non-existent memory."); 
            value = memory[operEvaluate(right, operandValues, changedRegisters)]; //expression.cpp
        } else {
            value = operEvaluate(right, operandValues, changedRegisters); //expression.cpp
        }
        
        setValue(left, value, operandValues, changedRegisters); //setValue.cpp

    }

}