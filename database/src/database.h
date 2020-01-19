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

class Database {
    struct Events {
        set<string> lex_ord;
        vector<string> add_ord;
    };
public:
    void Add(const Date& date, const string& event);
    
    template <typename Pred>
    int RemoveIf(Pred pred) {
        int removed = 0;
        for (auto it = begin(db); it != end(db); ) {
            const auto& date = (*it).first;
            auto& events = (*it).second;
            
            auto new_end = stable_partition(begin(events.add_ord), end(events.add_ord), [pred, &date](const auto& event) { return !pred(date, event); });
            removed += distance(new_end, end(events.add_ord));
            if (new_end == begin(events.add_ord)) {
                it = db.erase(it);
            } else {
                for (auto e_it = new_end; e_it != end(events.add_ord); ++e_it) {
                    events.lex_ord.erase(*e_it);
                }
                events.add_ord.erase(new_end, end(events.add_ord));
                ++it;
            }
        }
        return removed;
    }

    template <typename Pred>
    vector<Entry> FindIf(Pred pred) const {
        vector<Entry> res;
        for (const auto& [ date, events ] : db) {
            for (const auto& e : events.add_ord) {
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

