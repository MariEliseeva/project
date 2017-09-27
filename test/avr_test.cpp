#include <stdio.h>

#include "avr_test.h"
#include "avr.h"


void AvrTest::avrTest1() {
    avr av1(65);
    DO_CHECK(av1.memorySize == 65);
}

void AvrTest::avrTest2() {
    avr av1(60);
    DO_CHECK(av1.memorySize == 64);
}

void AvrTest::operationPriorityTest() {
    DO_CHECK(operationPriority('&') == 5);
}

void AvrTest::fillActionsVectorTest() {
    std::vector<std::string> actions;
    std::string comands = "C=Rd(7),Rd(7)=Rd(6),Rd(6)=Rd(5),Rd(5)=Rd(4),Rd(4)=Rd(3),Rd(3)=Rd(2),Rd(2)=Rd(1),Rd(1)=Rd(0),Rd(0)=0";
    fillActionsVector(comands, actions);
    DO_CHECK(actions[2] == "Rd(6)=Rd(5)");
}

void AvrTest::makeOperandMapTest()  {
    std::unordered_map<std::string, int> operandValues; 
    std::string argumentNumber[MAX_ARGS];
    std::string comands = "Rd,K8";
    makeOperandMap(comands, operandValues, argumentNumber); 
    DO_CHECK(operandValues.count("Rd") && argumentNumber[1] == "K8");
}

void AvrTest::fillOperandMapTest() {
    std::unordered_map<std::string, int> operandValues;
    avr av(64); 
    std::string argumentNumber[MAX_ARGS];
    operandValues["Rd"] = 0;
    operandValues["K8"] = 0;
    argumentNumber[0] = "Rd";
    argumentNumber[1] = "K8";
    std::set<uint16_t> changedRegisters;
    std::string values = "R5,(7*(4-3)+12/3-3*(2+1))";
    av.fillOperandMap(values, operandValues, argumentNumber, changedRegisters);
    DO_CHECK(operandValues["Rd"] == 5 && operandValues["K8"] == 7 * (4 - 3) + 12 / 3 - 3 * (2 + 1));
}

void AvrTest::operEvaluateTest() {
    std::unordered_map<std::string, int> operandValues;
    avr av(64); 
    std::string argumentNumber[MAX_ARGS];
    operandValues["Rd"] = 5;
    operandValues["K8"] = 7 * (4 - 3) + 12 / 3 - 3 * (2 + 1);
    argumentNumber[0] = "Rd";
    argumentNumber[1] = "K8";
    std::set<uint16_t> changedRegisters;
    std::string expr = "Rd+R5+3-K8";
    int result = av.operEvaluate(expr, operandValues, changedRegisters);
    int expected = 1;
    DO_CHECK(result == expected);  
}

void AvrTest::setRegBitValueTest() {
    std::string left = "R3(2)";
    int value = 1;
    std::unordered_map<std::string, int> operandValues;
    std::set<uint16_t> changedRegisters;
    avr av(64); 
    av.setRegBitValue(left, value,operandValues, changedRegisters);
    int res = (av.get(3) >> 2) & 1;
    DO_CHECK(res == 1);
}

void AvrTest::defSREGSetValueTest() {
    std::string left = "N";
    int value = 1;
    avr av(64);
    av.defSREGSetValue(left, value);
    int res = (av.get(63) >> 2) & 1;
    DO_CHECK(res == 1);
}

void AvrTest::setRegValueTest() {
    std::string left = "R17";
    int value = 23;
    avr av(64);
    std::set<uint16_t> changedRegisters;
    av.setRegValue(left, value, changedRegisters);
    DO_CHECK(av.get(17) == 23);
}

void AvrTest::SREGSetValueTest() { 
    std::string left = "SREG(b)";
    std::unordered_map<std::string, int> operandValues;
    operandValues["b"] = 5;
    int value = 1;
    avr av(64);
    av.SREGSetValue(left, value, operandValues);
    DO_CHECK(((av.get(63) >> 5) & 1) == 1);
}

void AvrTest::memSetValueTest() {
    std::string left = "(117)";
    int value = 42;
    std::unordered_map<std::string, int> operandValues;
    std::set<uint16_t> changedRegisters;
    avr av(200); 
    av.memSetValue(left, value, operandValues, changedRegisters);
    DO_CHECK(av.get(117) == 42);
}
void AvrTest::setValueTest1() {
    std::string left = "STACK";
    int value = 42;
    std::unordered_map<std::string, int> operandValues;
    std::set<uint16_t> changedRegisters;
    avr av(200); 
    av.setValue(left, value, operandValues, changedRegisters);
    DO_CHECK(av.get(av.SP + 1) == 42);
}

void AvrTest::setValueTest2() {
    std::string left = "RX";
    int value = 32767;
    std::unordered_map<std::string, int> operandValues;
    std::set<uint16_t> changedRegisters;
    avr av(200); 
    av.setValue(left, value, operandValues, changedRegisters);
    DO_CHECK(av.get(26) == 127 && av.get(27) == -1);
}

void AvrTest::constGetValueTest() { 
    std::string argument = "K8";
    std::unordered_map<std::string, int> operandValues;
    operandValues["K8"] = 123;
    avr av(64); 
    DO_CHECK(av.constGetValue (argument, operandValues) == 123);
}

void AvrTest::regGetValueTest() {
    std::string argument = "R15";
    avr av(80);
    av.memory[15] = 74;
    DO_CHECK(av.regGetValue(argument) == 74); 
}

void AvrTest::defSREGGetValueTest() {
    std::string argument = "H";
    avr av(80);
    av.memory[63] |= (1 << 5);
    DO_CHECK(av.defSREGGetValue(argument) == 1);
} 

void AvrTest::regBitGetValueTest() {
    std::string argument = "Rr(7)";
    std::unordered_map<std::string, int> operandValues;
    operandValues["Rr"] = 4;
    avr av(80);
    av.memory[4] |= (1 << 7);
    DO_CHECK(av.regBitGetValue(argument, operandValues) == 1);
}

void AvrTest::SREGGetValueTest() {
    std::unordered_map<std::string, int> operandValues;
    operandValues["k"] = 4;
    std::string argument = "SREG(k)";
    avr av(80);
    av.memory[63] |= (1 << 4);
    DO_CHECK(av.SREGGetValue (argument, operandValues) == 1);
}

void AvrTest::getValueTest1() {
   avr av(80);
   av.memory[30] = 127;
   av.memory[31] = -1;
   std::string argument = "RZ";
   std::unordered_map<std::string, int> operandValues;
   DO_CHECK(av.getValue(argument, operandValues) == 32767);
}

void AvrTest::getValueTest2() {
   avr av(80);
   av.memory[19] = 16;
   std::string argument = "Rr";
   std::unordered_map<std::string, int> operandValues;
   operandValues["Rr"] = 19;
   DO_CHECK(av.getValue(argument, operandValues) == 16);
}

void AvrTest::stackGetValueTest() {
    avr av(64);
    av.memory[av.SP] = 19;
    av.SP--;
    DO_CHECK(av.stackGetValue() == 19 && av.SP == 63);
}



void AvrTest::runAllTests() {
    avrTest1();
    avrTest2();
    operationPriorityTest();
    fillActionsVectorTest();
    makeOperandMapTest();
    fillOperandMapTest();
    operEvaluateTest();
    defSREGSetValueTest();
    setRegValueTest();
    setRegBitValueTest();
    SREGSetValueTest();
    memSetValueTest();
    setValueTest1();
    setValueTest2();
    constGetValueTest();
    regGetValueTest();
    defSREGGetValueTest();
    regBitGetValueTest();
    SREGGetValueTest();
    getValueTest1();
    getValueTest2();
    stackGetValueTest();
}
