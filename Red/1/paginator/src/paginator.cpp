#include "test_runner.h"

#include <numeric>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

template <typename Iterator>
class Paginator {
    // page
    class page {
    public:
        page(Iterator f, Iterator l, size_t n) : first(f), last(l), sz(n) {}

        Iterator begin() const { return first; }
        Iterator end() const { return last; }

        size_t size() const { return sz; }
    private:
        Iterator first, last;
        size_t sz;
    };
    // iterator
    class iterator {
    public:
        iterator(const Paginator& own, size_t id)
            : owner(own)
            , page_id(id)
            , curr_page(page_id < owner.size() ? owner[page_id] : page(owner.last, owner.last, 0u))
        {}
    
        iterator operator +(size_t n) {
            auto it = *this;
            return it += n;
        }
        /*iterator operator -(size_t n) {}
        iterator operator -(iterator rhs) {}*/

        iterator& operator ++() { advancePage(); return *this; }
        /*iterator& operator --() { return *this; }*/
        iterator& operator +=(size_t n) { advancePage(n); return *this; }
        /*iterator& operator -=(size_t n) { return *this; }*/

        const page& operator *() const { return curr_page; }
        page& operator *() { return curr_page; }
        const page* operator ->() const { return &curr_page; }
        page* operator ->() { return &curr_page; }
        bool operator ==(iterator rhs) const { return (&owner == &rhs.owner) && (page_id == rhs.page_id); }
        bool operator !=(iterator rhs) const { return !(*this == rhs); }
    private:
        void advancePage(size_t n = 1) {
            page_id += n;
            if (page_id < owner.size()) {
                size_t start = page_id * owner.page_size;
                size_t diff = owner.range_size - start;
                size_t m = min(owner.page_size, diff);
                auto f = (n == 1) ? curr_page.end() : next(curr_page.begin(), n * owner.page_size);
                auto l = next(f, m);
                curr_page = { f, l, m };
            } else {
                curr_page = { owner.last, owner.last, 0u };
            }
        }

        const Paginator& owner;
        size_t page_id;

        page curr_page;
    };
public:
    Paginator(Iterator f, Iterator l, size_t page_sz)
        : first(f)
        , last(l)
        , range_size(distance(first, last))
        , page_size(page_sz)
    {}

    iterator begin() const { return { *this, 0 }; }
    iterator end() const { return { *this, size() }; }

    size_t size() const {
        return (range_size / page_size) + ((range_size % page_size) != 0);
    }

    page operator [](size_t id) const {
        size_t start = id * page_size;
        size_t diff = range_size - start;
        size_t n = min(page_size, diff);
        auto f = (start > diff) ? prev(last, diff) : next(first, start);
        auto l = (n > diff - n) ? prev(last, diff - n) : next(f, n);
        return { f, l, n };
    }

private:
    const Iterator first, last;
    const size_t range_size;
    const size_t page_size;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

// Tests

void TestPageCounts() {
    vector<int> v(15);

    ASSERT_EQUAL(Paginate(v, 1).size(), v.size());
    ASSERT_EQUAL(Paginate(v, 3).size(), 5u);
    ASSERT_EQUAL(Paginate(v, 5).size(), 3u);
    ASSERT_EQUAL(Paginate(v, 4).size(), 4u);
    ASSERT_EQUAL(Paginate(v, 15).size(), 1u);
    ASSERT_EQUAL(Paginate(v, 150).size(), 1u);
    ASSERT_EQUAL(Paginate(v, 14).size(), 2u);
}

void TestLooping() {
    vector<int> v(15);
    iota(begin(v), end(v), 1);

    Paginator<vector<int>::iterator> paginate_v(v.begin(), v.end(), 6);
    ostringstream os;
    for (const auto& page : paginate_v) {
        for (int x : page) {
            os << x << ' ';
        }
        os << '\n';
    }

    ASSERT_EQUAL(os.str(), "1 2 3 4 5 6 \n7 8 9 10 11 12 \n13 14 15 \n");
}

void TestModification() {
    vector<string> vs = {"one", "two", "three", "four", "five"};
    for (auto page : Paginate(vs, 2)) {
        for (auto& word : page) {
            word[0] = toupper(word[0]);
        }
    }

    const vector<string> expected = {"One", "Two", "Three", "Four", "Five"};
    ASSERT_EQUAL(vs, expected);
}

void TestPageSizes() {
    string letters(26, ' ');

    Paginator letters_pagination(letters.begin(), letters.end(), 11);
    vector<size_t> page_sizes;
    for (const auto& page : letters_pagination) {
        page_sizes.push_back(page.size());
    }

    const vector<size_t> expected = {11, 11, 4};
    ASSERT_EQUAL(page_sizes, expected);
}

void TestConstContainer() {
    const string letters = "abcdefghijklmnopqrstuvwxyz";

    vector<string> pages;
    for (const auto& page : Paginate(letters, 10)) {
        pages.push_back(string(page.begin(), page.end()));
    }

    const vector<string> expected = {"abcdefghij", "klmnopqrst", "uvwxyz"};
    ASSERT_EQUAL(pages, expected);
}

void TestPagePagination() {
    vector<int> v(22);
    iota(begin(v), end(v), 1);

    vector<vector<int>> lines;
    for (const auto& split_by_9 : Paginate(v, 9)) {
        for (const auto& split_by_4 : Paginate(split_by_9, 4)) {
            lines.push_back({});
            for (int item : split_by_4) {
                lines.back().push_back(item);
            }
        }
    }

    const vector<vector<int>> expected = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9},
        {10, 11, 12, 13},
        {14, 15, 16, 17},
        {18},
        {19, 20, 21, 22}
    };
    ASSERT_EQUAL(lines, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestPageCounts);
    RUN_TEST(tr, TestLooping);
    RUN_TEST(tr, TestModification);
    RUN_TEST(tr, TestPageSizes);
    RUN_TEST(tr, TestConstContainer);
    RUN_TEST(tr, TestPagePagination);
    return 0;
}

