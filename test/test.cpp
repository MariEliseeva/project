#include "avr_test.h"

int main() {
    AvrTest at;

    at.runAllTests();
    AvrTest::showFinalResult();

    return 0;
}
