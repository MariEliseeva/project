#include "avr.h"

int operationPriority( char c ) {
    if (c == ')') return 0;
    if (c == 'o') return 1;
    if (c == 'a') return 2;
    if (c == '|') return 3;
    if (c == '^') return 4;
    if (c == '&') return 5;
    if (c == '=') return 6;
    if (c == '<' || c == '>') return 7;
    if (c == '-' || c == '+') return 8;
    if (c == '*' || c == '/') return 9;
    if (c == '~' || c == '!') return 10;
    return 11;
}

int avr::operEvaluate(std::string expr, std::unordered_map<std::string, int> &operandValues, std::set<uint16_t> &changedRegisters) {
	//вычисление значения арифметического выражения.

    int operNumber = 0;
    int opers[MAX_LEN];
    int valueNumber = 0;
    int values[MAX_LEN];
 
    opers[operNumber] = '(';
    expr += ')';

    char operation;
    std::string variable = "";

    for (size_t i = 0; i < expr.size();)
        if (expr[i] == '(' || expr[i] == ')' ||
            expr[i] == '+' || expr[i] == '-' ||
            expr[i] == '*' || expr[i] == '/' || 
            expr[i] == '|' || expr[i] == '&' ||
            expr[i] == '^' || expr[i] == '~' ||
            expr[i] == '=' || expr[i] == '<' || 
            expr[i] == '<' || expr[i] == '!') {

        	if (expr[i] == '|' && expr[i + 1] == '|'){
        		i++;
        		expr[i] = 'o';
        	}
        	if (expr[i] == '&' && expr[i + 1] == '&'){
        		i++;
        		expr[i] = 'a';
        	}
        	if (expr[i] == '=') i++;

            while (operationPriority(expr[i]) <= operationPriority(opers[operNumber]) 
                   && opers[operNumber] != '('){

                operation = opers[operNumber];
                operNumber--;
                valueNumber--; 
                if (valueNumber < 0) throw avrException("Wrong expression error.");
                if (valueNumber == 0 && operation != '~' && operation != '!') throw avrException("Wrong expression error.");
                if (operation == '-') values[valueNumber - 1] -= values[valueNumber]; 
                if (operation == '+') values[valueNumber - 1] += values[valueNumber]; 
                if (operation == '*') values[valueNumber - 1] *= values[valueNumber]; 
                if (operation == '/') {
                	if (values[valueNumber] == 0) {
                		throw avrException("Divizion by zero.");
                	}
                	values[valueNumber - 1] /= values[valueNumber];
                }
                if (operation == '|') values[valueNumber - 1] |= values[valueNumber];
                if (operation == '&') values[valueNumber - 1] &= values[valueNumber];
                if (operation == '^') values[valueNumber - 1] ^= values[valueNumber];
                if (operation == '~') {values[valueNumber] = ~values[valueNumber]; valueNumber++;}
                if (operation == 'a') values[valueNumber - 1] = values[valueNumber - 1] && values[valueNumber];
                if (operation == 'o') values[valueNumber - 1] = values[valueNumber - 1] || values[valueNumber];
                if (operation == '!') {values[valueNumber] = !values[valueNumber]; valueNumber++;}
                if (operation == '=') values[valueNumber - 1] = values[valueNumber - 1] == values[valueNumber];
                if (operation == '<') values[valueNumber - 1] = values[valueNumber - 1] < values[valueNumber];
                if (operation == '>') values[valueNumber - 1] = values[valueNumber - 1] > values[valueNumber];
               }

            if (expr[i] == ')') {
                operNumber--;
                i++;
            }
            else
                opers[++operNumber] = expr[i++];
        } else {
            values[valueNumber] = 1;
            variable = "";
            while (expr.size() > i && !(expr[i] == '(' || expr[i] == ')' ||
                     expr[i] == '+' || expr[i] == '-' ||
                     expr[i] == '*' || expr[i] == '/' || 
                     expr[i] == '|' || expr[i] == '&' ||
                     expr[i] == '^' || expr[i] == '~' ||
                     expr[i] == '<' || expr[i] == '>' ||
                     expr[i] == '=' || expr[i] == '!')) {
            	variable += expr[i];
                i++;
            }
            values[valueNumber] = (int)getValue(variable, operandValues); // getValue.cpp - возвращает значение для операнда.
            valueNumber++;
        }
    if (values[0] < -127) setValue("V", 1, operandValues, changedRegisters);
    if (values[0] > 128) setValue("C", 1, operandValues, changedRegisters);
    values[0] = values[0] & ((1 << 8) - 1);
    if (values[0] < 0) setValue("N", 1, operandValues, changedRegisters);
    if (values[0] == 0) setValue("Z", 1, operandValues, changedRegisters);
    setValue("S", getValue("V", operandValues) ^ getValue("N", operandValues), operandValues, changedRegisters);
    return values[0];
}
