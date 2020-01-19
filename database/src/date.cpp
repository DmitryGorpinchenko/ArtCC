#include "date.h"

#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <string>
#include <tuple>

// Date

Date::Date(int _year, int _month, int _day) : year(_year), month(_month), day(_day) {
    if (month < 1 || month > 12) {
        throw invalid_argument("Month value is invalid: " + to_string(month));
    }
    if (day < 1 || day > 31) {
        throw invalid_argument("Day value is invalid: " + to_string(day));
    }
}

int Date::GetYear() const { return year; }
int Date::GetMonth() const { return month; }
int Date::GetDay() const { return day; }

//

Date ParseDate(istream& is) {
    Date d;
    is >> d;
    return d;
}

bool operator <(const Date& lhs, const Date& rhs) {
    const auto t = [](const Date& d) { return make_tuple(d.GetYear(), d.GetMonth(), d.GetDay()); };
    return t(lhs) < t(rhs);
}
ostream& operator <<(ostream& out, const Date& val) {
    out << setfill('0') << setw(4) << val.GetYear() << '-';
    out << setfill('0') << setw(2) << val.GetMonth() << '-';
    out << setfill('0') << setw(2) << val.GetDay();
    return out;
}
istream& operator >>(istream& in, Date& val) {
    string date_str;
    in >> date_str;
    istringstream ss(date_str);
    const auto invalid_arg = [&date_str]() {
        throw invalid_argument("Wrong date format: " + date_str);
    };
    const auto ignore = [&ss, invalid_arg](char c) {
        if (ss.peek() == c) {
            ss.ignore(1);
        } else {
            invalid_arg();
        }
    };
    const auto read = [&ss, invalid_arg](int& val) {
        if (!(ss >> val)) {
            invalid_arg();
        }
    };
    const auto ensure_empty = [&ss, invalid_arg]() {
        char dummy;
        if (ss >> dummy) {
            invalid_arg();
        }
    };
    int year, month, day;
    read(year);
    ignore('-');
    read(month);
    ignore('-');
    read(day);
    ensure_empty();
    val = { year, month, day };
    return in;
}
