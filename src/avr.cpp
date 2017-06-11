#include "avr.h"

avr::avr(size_t _memorySize) {
	memorySize = _memorySize;
	if (memorySize < 64) memorySize = 64;
	SP = memorySize - 1; //в микроконтроллерах стек расположен в конце обычной памяти.

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
    std::string argumentNumber[MAX_ARGS];
    //вспомогательный массив - храним имена аргументов в порядке, в котором они передаются.
    
    if (comands.first != "-") {
        makeOperandMap(comands.first, operandValues, argumentNumber); 
        //Сопоставляет имени аргумента его номер по порядку, создаёт ключи в unordered_map значений.

        fillOperandMap(values, operandValues, argumentNumber, changedRegisters); //Заполняет значениями.
        //реализация в файле readValues.cpp
    }

    std::vector<std::string> actions; //действия, которые выполняются при выполнениии данной команды
    fillActionsVector(comands.second, actions); //тоже в readValues.cpp. 

    for (size_t i = 0; i < actions.size(); i++) {
        doAction(actions[i], operandValues, changedRegisters); //выполнение. В файле doAction.cpp
    }

    PC++; //Программный счётчик увеличивается на один после каждой команды.

    if (!quite) {
    	for (int i = 0; i < 32; i++) {
            std::cout << (int) memory[i] << ' ';
        }
        std::cout << '\n';
    }

    return PC; //к какой команде следует переходить
}