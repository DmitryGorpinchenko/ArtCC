#pragma once

#include <iostream>

using namespace std;

class Date {
public:
    Date(int _year = 0, int _month = 1, int _day = 1);

    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;
private:
    int year;
    int month;
    int day;
};

Date ParseDate(istream& is);

bool operator <(const Date& lhs, const Date& rhs);
ostream& operator <<(ostream& out, const Date& val);
istream& operator >>(istream& in, Date& val);
