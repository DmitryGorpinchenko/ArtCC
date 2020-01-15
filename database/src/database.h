#pragma once

#include "date.h"

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Entry {
    Date date;
    string event;
};

class Events {
public:
    void Add(const string& event) {
        if (lex_ord.insert(event).second) {
            add_ord.push_back(event);
        }
    }
    
    template <typename Pred>
    int RemoveIf(Pred pred) {
        int removed = 0;
        for (auto it = begin(lex_ord); it != end(lex_ord); ) {
            if (pred(*it)) {
                it = lex_ord.erase(it);
                ++removed;
            } else {
                ++it;
            }
        }
        add_ord.erase(remove_if(begin(add_ord), end(add_ord), pred), end(add_ord));
        return removed;
    }
    
    const vector<string>& addOrdered() const { return add_ord; }
    const string& Last() const { return add_ord.back(); }
    bool Empty() const { return add_ord.empty(); }
private:
    set<string> lex_ord;
    vector<string> add_ord;
};

class Database {
public:
    void Add(const Date& date, const string& event);
    
    template <typename Pred>
    int RemoveIf(Pred pred) {
        int removed = 0;
        for (auto it = begin(db); it != end(db); ) {
            const auto& date = (*it).first;
            auto& events = (*it).second;
            removed += events.RemoveIf([pred, &date](const auto& event) { return pred(date, event); });
            if (events.Empty()) {
                it = db.erase(it);
            } else {
                ++it;
            }
        }
        return removed;
    }

    template <typename Pred>
    vector<Entry> FindIf(Pred pred) const {
        vector<Entry> res;
        for (const auto& [ date, events ] : db) {
            for (const auto& e : events.addOrdered()) {
                if (pred(date, e)) {
                    res.push_back({ date, e });
                }
            }
        }
        return res;
    }
    
    Entry Last(const Date& date) const;
    void Print(ostream& os) const;

private:
    map<Date, Events> db;
};

ostream& operator <<(ostream& os, const Entry& val);

