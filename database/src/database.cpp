#include "database.h"

#include <stdexcept>

void Database::Add(const Date& date, const string& event) {
    auto& ev = db[date];
    if (ev.lex_ord.insert(event).second) {
        ev.add_ord.push_back(event);
    }
}

Entry Database::Last(const Date& date) const {
    auto it = db.upper_bound(date);
    if (it == begin(db)) {
        throw invalid_argument("No entries");
    }
    const auto& [ d, events ] = *prev(it);
    return { d, events.add_ord.back() };
}

void Database::Print(ostream& os) const {
    for (const auto& [ date, events ] : db) {
        for (const auto& e : events.add_ord) {
            Entry entry{ date, e };
            os << entry << endl;
        }
    }
}

//

ostream& operator <<(ostream& os, const Entry& val) {
    return os << val.date << ' ' << val.event;
}
