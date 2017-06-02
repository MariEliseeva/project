#include <stdio.h>

#include "avr_test.h"
#include "avr.h"


void AvrTest::avrTest1() {
    avr av1(65);
    DO_CHECK(av1.memorySize == 65);
}

void AvrTest::avrTest2() {
    avr av1(65);
    DO_CHECK(av1.memorySize == 65);
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


void AvrTest::runAllTests() {
    avrTest1();
    avrTest2();
    operationPriorityTest();
    fillActionsVectorTest();
    makeOperandMapTest();
    fillOperandMapTest();
    operEvaluateTest();
}
