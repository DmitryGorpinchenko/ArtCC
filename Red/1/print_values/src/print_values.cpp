#include "test_runner.h"

#include <ostream>
using namespace std;

#define PRINT_VALUES(out, x, y)                 \
    ((out) << (x) << endl << (y) << endl)
//    do {                                        \
  //      ((out) << (x) << endl << (y) << endl);  \
    //} while (false)                             \

int main() {
    TestRunner tr;
    tr.RunTest([] {
        ostringstream output;
        PRINT_VALUES(output, 5, "red belt");
        ASSERT_EQUAL(output.str(), "5\nred belt\n");
    }, "PRINT_VALUES usage example1");
    tr.RunTest([] {
        ostringstream output;
        PRINT_VALUES(output << 1 << "s", 5, "red belt");
        ASSERT_EQUAL(output.str(), "1s5\nred belt\n");
    }, "PRINT_VALUES usage example2");
    tr.RunTest([] {
        ostringstream output;
        PRINT_VALUES(output, 0, 0);
        ASSERT_EQUAL(output.str(), "0\n0\n");
    }, "PRINT_VALUES usage example3");
    tr.RunTest([] {
        ostringstream output;
        if (false) {
            PRINT_VALUES(output, 0, 0);
        }
        else
            PRINT_VALUES(output, 1, 0);
        ASSERT_EQUAL(output.str(), "1\n0\n");
    }, "PRINT_VALUES usage example4");
}
