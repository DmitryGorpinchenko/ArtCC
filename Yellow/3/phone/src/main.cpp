#include "phone_number.h"
#include "test_runner.h"

#include <exception>

void Test1() {
    {
        PhoneNumber n("+7-495-111-22-33");
        AssertEqual(n.GetCountryCode(), "7");
        AssertEqual(n.GetCityCode(), "495");
        AssertEqual(n.GetLocalNumber(), "111-22-33");
    }
    {
        PhoneNumber n("+7-495-1112233");
        AssertEqual(n.GetCountryCode(), "7");
        AssertEqual(n.GetCityCode(), "495");
        AssertEqual(n.GetLocalNumber(), "1112233");
    }
    {
        PhoneNumber n("+323-22-460002");
        AssertEqual(n.GetCountryCode(), "323");
        AssertEqual(n.GetCityCode(), "22");
        AssertEqual(n.GetLocalNumber(), "460002");
    }
    {
        PhoneNumber n("+1-2-coursera-cpp");
        AssertEqual(n.GetCountryCode(), "1");
        AssertEqual(n.GetCityCode(), "2");
        AssertEqual(n.GetLocalNumber(), "coursera-cpp");
    }
}

void Test2() {
    try {
        PhoneNumber n("1-2-333");
        Assert(false, "invalid_argument exception has not been thrown");
    } catch (std::invalid_argument& e) {
    }
    try {
        PhoneNumber n("+7-1233");
        Assert(false, "invalid_argument exception has not been thrown");
    } catch (std::invalid_argument& e) {
    }
}

int main() {
    TestRunner runner;
    runner.RunTest(Test1, "Test1");
    runner.RunTest(Test2, "Test2");
    return 0;
}
