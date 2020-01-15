#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <utility>
#include <vector>
#include <cstdint>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    using Id = uint64_t;

    Id Add(T object) {
        const auto& [id, pair] = *obj_by_id.emplace_hint(obj_by_id.end(), next_id++, make_pair(move(object), 0));
        const auto& [obj, priority] = pair;
        auto& ids = ids_by_priority[priority];
        ids.emplace_hint(ids.end(), id);
        return id;
    }

    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
        for (auto it = range_begin; it != range_end; ++it) {
            *ids_begin++ = Add(move(*it));
        }
    }

    bool IsValid(Id id) const { return obj_by_id.count(id); }
    const T& Get(Id id) const { return obj_by_id.at(id).first; }

    void Promote(Id id) {
        auto& [obj, priority] = obj_by_id.at(id);
        auto it = ids_by_priority.find(priority);
        auto& ids = (*it).second;
        ids.erase(id);
        if (ids.empty()) {
            ids_by_priority.erase(it);
        }
        ids_by_priority[++priority].insert(id);
    }

    pair<const T&, int> GetMax() const {
        const auto& ids = (*ids_by_priority.rbegin()).second;
        const auto& entry = obj_by_id.at(*ids.rbegin());
        return entry;
    }

    pair<T, int> PopMax() {
        auto it_ids = prev(ids_by_priority.end());
        auto& ids = (*it_ids).second;
        auto it_id = prev(ids.end());
        const auto id = *it_id;
        ids.erase(it_id);
        if (ids.empty()) {
            ids_by_priority.erase(it_ids);
        }
        auto it = obj_by_id.find(id);
        pair<T, int> res = move((*it).second);
        obj_by_id.erase(it);
        return res;
    }

private:
    map<Id, pair<T, int>> obj_by_id;
    map<int, set<Id>> ids_by_priority;

    inline static Id next_id = 0;
};

//

class StringNonCopyable : public string {
public:
    using string::string; // allows to use ctors of std::string
    StringNonCopyable(const StringNonCopyable&) = delete;
    StringNonCopyable(StringNonCopyable&&) = default;
    StringNonCopyable& operator=(const StringNonCopyable&) = delete;
    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

void TestRangeCtor() {
    PriorityCollection<StringNonCopyable> strings;
    
    vector<StringNonCopyable> data;
    vector<PriorityCollection<StringNonCopyable>::Id> ids;
    strings.Add(begin(data), end(data), back_inserter(ids));
    
    const auto id = strings.Add("test");
    const auto p = strings.GetMax();
    const auto& el = strings.Get(id);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestRangeCtor);
    return 0;
}
