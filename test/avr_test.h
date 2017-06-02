#pragma once

#include "Test.h"

#define DO_CHECK(EXPR) Test::check(EXPR, __func__, __FILE__, __LINE__)


class AvrTest: public Test {
  public:
    void avrTest1();
    void avrTest2();
    void operationPriorityTest(); 
    void fillActionsVectorTest();
    void fillOperandMapTest();
    void makeOperandMapTest();
    void operEvaluateTest();
    void runAllTests();
};
