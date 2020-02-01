#include <iostream>
#include <string>
#include <numeric>
#include <ctime>
using namespace std;

// Date

class Date {
public:
    Date() = default;
    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    time_t AsTimestamp() const {
        if (!valid) {
            tm t;
            t.tm_sec   = 0;
            t.tm_min   = 0;
            t.tm_hour  = 0;
            t.tm_mday  = day;
            t.tm_mon   = month - 1;
            t.tm_year  = year - 1900;
            t.tm_isdst = 0;
            stamp = mktime(&t);
            valid = true;
        }
        return stamp;
    }
    
private:
    int year = 0;
    int month = 0;
    int day = 0;
    mutable time_t stamp;
    mutable bool valid = false;
};

istream& operator >>(istream& is, Date& val) {
    int y, m, d;
    char delim;
    is >> y >> delim >> m >> delim >> d;
    val = Date(y, m, d);
    return is;
}

int ComputeDaysDiff(const Date& to, const Date& from) {
    const time_t timestamp_to = to.AsTimestamp();
    const time_t timestamp_from = from.AsTimestamp();
    static const int SECONDS_IN_DAY = 60 * 60 * 24;
    return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

static const Date START_DATE = Date(2000, 1, 1);
static const Date END_DATE = Date(2100, 1, 1);

//

class BudgetManager {
public:
    void Earn(const Date& from, const Date& to, int value) {
        const size_t days = ComputeDaysDiff(to, from) + 1;
        const size_t start = ComputeDaysDiff(from, START_DATE);
        const double per_day = double(value) / days;
        for (size_t i = start; i < start + days; ++i) {
            records[i].earned += per_day;
        }
    }
    
    void Spend(const Date& from, const Date& to, int value) {
        const auto days = ComputeDaysDiff(to, from) + 1;
        const auto start = ComputeDaysDiff(from, START_DATE);
        const auto per_day = double(value) / days;
        for (size_t i = start; i < start + days; ++i) {
            records[i].spent += per_day;
        }
    }
    
    void PayTax(const Date& from, const Date& to, double percent) {
        const auto days = ComputeDaysDiff(to, from) + 1;
        const auto start = ComputeDaysDiff(from, START_DATE);
        const auto factor = 1. - percent / 100.;
        for (size_t i = start; i < start + days; ++i) {
            records[i].earned *= factor;
        }
    }
    
    double ComputeIncome(const Date& from, const Date& to) {
        const auto days = ComputeDaysDiff(to, from) + 1;
        const auto start = ComputeDaysDiff(from, START_DATE);
        double res = 0;
        for (size_t i = start; i < start + days; ++i) {
            res += (records[i].earned - records[i].spent);
        }
        return res;
    }
private:
    struct Record {
        double earned = 0;
        double spent = 0;
    };
    Record records[1 << 16];
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cout.precision(15);
    
    int n;
    cin >> n;
    
    BudgetManager manager;
    for (int i = 0; i < n; ++i) {
        Date from, to;
        string op;
        cin >> op >> from >> to;
        if (op == "ComputeIncome") {
            cout << manager.ComputeIncome(from, to) << '\n';
        } else if (op == "Earn") {
            int val;
            cin >> val;
            manager.Earn(from, to, val);
        } else if (op == "PayTax") {
            int percent;
            cin >> percent;
            manager.PayTax(from, to, percent);
        } else if (op == "Spend") {
            int val;
            cin >> val;
            manager.Spend(from, to, val);
        }
    }
    
    return 0;
}
