#pragma once

class Test {
  protected:
    static int failedNum;
    static int totalNum;

  public:
    static void check(bool expr, const char *func, const char  *filename, int lineNum); 
    static void showFinalResult();

    virtual void runAllTests() = 0;
};
