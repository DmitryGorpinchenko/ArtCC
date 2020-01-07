#include <iostream>
#include <iomanip>
#include <sstream>
#include <exception>
#include <map>
#include <set>
#include <tuple>

class Date {
public:
    Date(int _year = 0, int _month = 1, int _day = 1) : year(_year), month(_month), day(_day) {
        if (month < 1 || month > 12) {
            throw std::invalid_argument("Month value is invalid: " + std::to_string(month));
        }
        if (day < 1 || day > 31) {
            throw std::invalid_argument("Day value is invalid: " + std::to_string(day));
        }
    }

    int GetYear() const { return year; }
    int GetMonth() const { return month; }
    int GetDay() const { return day; }
private:
    int year;
    int month;
    int day;
};

bool operator <(const Date& lhs, const Date& rhs) {
    const auto t = [](const Date& d) { return std::make_tuple(d.GetYear(), d.GetMonth(), d.GetDay()); };
    return t(lhs) < t(rhs);
}
std::ostream& operator <<(std::ostream& out, const Date& val) {
    out << std::setfill('0') << std::setw(4) << val.GetYear() << '-';
    out << std::setfill('0') << std::setw(2) << val.GetMonth() << '-';
    out << std::setfill('0') << std::setw(2) << val.GetDay();
    return out;
}
std::istream& operator >>(std::istream& in, Date& val) {
    std::string date_str;
    in >> date_str;
    std::istringstream ss(date_str);
    const auto invalid_arg = [&date_str]() {
        throw std::invalid_argument("Wrong date format: " + date_str);
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

class Database {
public:
    void AddEvent(const Date& date, const std::string& event) {
        if (!event.empty()) {
            db[date].insert(event);
        }
    }
    bool DeleteEvent(const Date& date, const std::string& event) {
        auto it = db.find(date);
        if (it != std::end(db)) {
            const auto erased = (*it).second.erase(event) > 0;
            if ((*it).second.empty()) {
                db.erase(it);
            }
            return erased;
        }
        return false;
    }
    int DeleteDate(const Date& date) {
        auto it = db.find(date);
        if (it != std::end(db)) {
            const auto n = (*it).second.size();
            db.erase(it);
            return n;
        }
        return 0;
    }
    const std::set<std::string>& Find(const Date& date) const {
        auto it = db.find(date);
        if (it != std::end(db)) {
            return (*it).second;
        }
        static const std::set<std::string> null;
        return null;
    }
    void Print() const {
        for (const auto& p : db) {
            for (const auto& e : p.second) {
                std::cout << p.first << ' ' << e << std::endl;
            }
        }
    }

private:
    std::map<Date, std::set<std::string>> db;
};

void run() {
    Database db;
    std::string command;
    while (getline(std::cin, command)) {
        if (!command.empty()) {
            Date date;
            std::string op, event;
            std::istringstream ss(command);
            ss >> op;
            if (op == "Add") {
                ss >> date >> event;
                db.AddEvent(date, event);
            } else if (op == "Del") {
                ss >> date;
                if (ss >> event) {
                    const auto res = db.DeleteEvent(date, event);
                    std::cout << (res ? "Deleted successfully" : "Event not found") << std::endl;
                } else {
                    const auto res = db.DeleteDate(date);
                    std::cout << ("Deleted " + std::to_string(res) + " events") << std::endl;
                }
            } else if (op == "Find") {
                ss >> date;
                for (const auto& e : db.Find(date)) {
                    std::cout << e << std::endl;
                }
            } else if (op == "Print") {
                db.Print();
            } else {
                std::cout << ("Unknown command: " + op) << std::endl;
            }
        }
    }
}

int main() {
    try {
        run();
    } catch (std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
    }
    return 0;
}

