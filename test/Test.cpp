#include <cstdio>

#include "Test.h"

int Test::failedNum = 0;
int Test::totalNum = 0;


void Test::check(bool expr, const char *func, const char  *filename, int lineNum) {
    totalNum++;

    if (expr) return;

    failedNum++;
    printf("test failed: %s in %s:%d\n", func, filename, lineNum);
}


void Test::showFinalResult() {
    if (failedNum == 0) {
        printf("All tests passed.\n");
        return;
    }

    printf("Failed %d of %d tests.", failedNum, totalNum);
}
