#include "test_runner.h"

#include <iostream>
#include <map>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

class Database {
public:
    bool Put(const Record& record) {
        auto [l, u] = by_id.equal_range(record.id);
        if (l != u) {
            return false;
        }
        records.push_front(record);
        auto rit = begin(records);
        auto uit = by_user.emplace((*rit).user, rit);
        auto tit = by_time.emplace((*rit).timestamp, rit);
        auto kit = by_karma.emplace((*rit).karma, rit);
        by_id.emplace_hint(l, (*rit).id, Iters{ rit, uit, tit, kit });
        return true;
    }
    
    const Record* GetById(const string& id) const {
        auto it = by_id.find(id);
        return (it != end(by_id)) ? &(*((*it).second).rit) : nullptr;
    }
    
    bool Erase(const string& id) {
        auto it = by_id.find(id);
        if (it == end(by_id)) {
            return false;
        }
        auto iters = (*it).second;
        by_id.erase(it);
        by_user.erase(iters.uit);
        by_time.erase(iters.tit);
        by_karma.erase(iters.kit);
        records.erase(iters.rit);
        return true;
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        auto l = by_time.lower_bound(low);
        auto r = by_time.upper_bound(high);
        for (auto it = l; it != r; ++it) {
            if (!callback(*(*it).second)) {
                return;
            }
        }
    }

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        auto l = by_karma.lower_bound(low);
        auto r = by_karma.upper_bound(high);
        for (auto it = l; it != r; ++it) {
            if (!callback(*(*it).second)) {
                return;
            }
        }
    }

    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const {
        auto [l, r] = by_user.equal_range(user);
        for (auto it = l; it != r; ++it) {
            if (!callback(*(*it).second)) {
                return;
            }
        }
    }
    
private:
    using DataStore = list<Record>;
    template <typename Field> using SecondaryIndex = multimap<Field, DataStore::iterator>;
    struct Iters {
        DataStore::iterator rit;
        SecondaryIndex<string_view>::iterator uit;
        SecondaryIndex<int>::iterator tit, kit;
    };
    using PrimaryIndex = unordered_map<string_view, Iters>;

    DataStore records;
    
    SecondaryIndex<string_view> by_user;
    SecondaryIndex<int> by_time, by_karma;
    PrimaryIndex by_id;
};

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    return 0;
}
