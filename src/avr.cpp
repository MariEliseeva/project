#include "avr.h"

avr::avr(size_t _memorySize) {
	memorySize = _memorySize;
	if (memorySize < 64) memorySize = 64;
	SP = memorySize - 1; //в микроконтроллерах стек расположен в конце обычной памяти.
                        //stack in microcontroller is in the end of global memory.

	memory.resize(memorySize);

    for (size_t i = 0; i < memorySize; i++) {
       memory[i] = 0;
    }
}

int8_t avr::get(int t) {
    return memory[t];
}



size_t avr::perform(std::string &values, std::pair<std::string, std::string> &comands, std::set<uint16_t> &changedRegisters, bool quite) {
    std::unordered_map<std::string, int> operandValues; 
    //Здесь хранятся значения для констант и номера регистров - для Rr, Rd.
    // Here are values of constants and number of registers - for Rr and Rd.
    std::string argumentNumber[MAX_ARGS];
    //вспомогательный массив - храним имена аргументов в порядке, в котором они передаются.
    // additional array - here are names of arguments inan order they are given.
    
    if (comands.first != "-") {
        makeOperandMap(comands.first, operandValues, argumentNumber); 
        //Сопоставляет имени аргумента его номер по порядку, создаёт ключи в unordered_map значений.
        //Mathes a number of the argument to its name, makes keys in unotdered_map of values.

        fillOperandMap(values, operandValues, argumentNumber, changedRegisters); //Заполняет значениями.
                                                                                 //fills values.
        //реализация в файле readValues.cpp
        //realised in readValues.cpp
    }

    std::vector<std::string> actions; //действия, которые выполняются при выполнениии данной команды
                                      // actions which are performed in a curent command.
    fillActionsVector(comands.second, actions); //тоже в readValues.cpp. 
                                                //also in readValues.cpp

    for (size_t i = 0; i < actions.size(); i++) {
        doAction(actions[i], operandValues, changedRegisters); //выполнение. В файле doAction.cpp
                                                               //performing. Realized in file doAction.cpp
    }

    PC++; //Программный счётчик увеличивается на один после каждой команды.
          //Program counter. Increases after each command.
    if (!quite) {
    	for (int i = 0; i < 32; i++) {
            std::cout << (int) memory[i] << ' ';
        }
        std::cout << '\n';
    }

    return PC; //к какой команде следует переходить
               //to which command to go next.
}