#include "test_runner.h"
#include "person.h" // comment out when submit

void TestEmpty() {
    Person person;
    AssertEqual(person.GetFullName(0), "Incognito");
    AssertEqual(person.GetFullName(-1001), "Incognito");
    AssertEqual(person.GetFullName(2019), "Incognito");
}

void TestFirstName() {
    Person person;
    person.ChangeFirstName(2019, "A");
    AssertEqual(person.GetFullName(2018), "Incognito");
    AssertEqual(person.GetFullName(2019), "A with unknown last name");
    AssertEqual(person.GetFullName(2020), "A with unknown last name");
    
    person.ChangeFirstName(-2, "B");
    AssertEqual(person.GetFullName(2020), "A with unknown last name");
    AssertEqual(person.GetFullName(2018), "B with unknown last name");
    AssertEqual(person.GetFullName(-3), "Incognito");
}

void TestLastName() {
    Person person;
    person.ChangeLastName(2019, "A");
    AssertEqual(person.GetFullName(2018), "Incognito");
    AssertEqual(person.GetFullName(2019), "A with unknown first name");
    AssertEqual(person.GetFullName(2020), "A with unknown first name");
    
    person.ChangeLastName(-2, "B");
    AssertEqual(person.GetFullName(2020), "A with unknown first name");
    AssertEqual(person.GetFullName(2018), "B with unknown first name");
    AssertEqual(person.GetFullName(-3), "Incognito");
}

void TestFullName() {
    {
        Person person;
        person.ChangeFirstName(2019, "A");
        person.ChangeLastName(2019, "B");
        AssertEqual(person.GetFullName(2018), "Incognito");
        AssertEqual(person.GetFullName(2019), "A B");
        AssertEqual(person.GetFullName(2020), "A B");
    }
    {
        Person person;
        person.ChangeFirstName(2019, "A");
        AssertEqual(person.GetFullName(2019), "A with unknown last name");
        
        person.ChangeLastName(2020, "B");
        AssertEqual(person.GetFullName(2019), "A with unknown last name");
        AssertEqual(person.GetFullName(2020), "A B");
        AssertEqual(person.GetFullName(2018), "Incognito");
        
        person.ChangeLastName(2018, "C");
        AssertEqual(person.GetFullName(2018), "C with unknown first name");
        AssertEqual(person.GetFullName(2019), "A C");
    }
    {
        Person person;
        person.ChangeLastName(2019, "A");
        person.ChangeFirstName(2020, "C");
        person.ChangeLastName(-2, "B");
        person.ChangeFirstName(-5, "D");
        AssertEqual(person.GetFullName(2020), "C A");
        AssertEqual(person.GetFullName(2018), "D B");
        AssertEqual(person.GetFullName(-6), "Incognito");
        AssertEqual(person.GetFullName(-3), "D with unknown last name");
    }
}

int main() {
    TestRunner runner;
    runner.RunTest(TestEmpty, "TestEmpty");
    runner.RunTest(TestFirstName, "TestFirstName");
    runner.RunTest(TestLastName, "TestLastName");
    runner.RunTest(TestFullName, "TestFullName");
    return 0;
}

