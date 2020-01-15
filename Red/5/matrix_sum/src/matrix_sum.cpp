#include "test_runner.h"
#include <vector>
#include <future>
#include <cstdint>
#include <numeric>
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

        iterator& operator ++() { advancePage(); return *this; }

        const page& operator *() const { return curr_page; }
        page& operator *() { return curr_page; }
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

template <typename RowIt>
int64_t CalculateMatrixSum(RowIt first, RowIt last) {
    return accumulate(first, last, int64_t(0), [](int64_t acc, const auto& row) {
        return accumulate(begin(row), end(row), acc);
    });
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    const size_t page_size = max(size_t(2000), matrix.size() / 4);
    vector<future<int64_t>> futures;
    for (const auto& page : Paginate(matrix, page_size)) {
        futures.push_back(
            async([page]() {
                return CalculateMatrixSum(begin(page), end(page));
            })
        );
    }
    return accumulate(begin(futures), end(futures), int64_t(0), [](int64_t acc, auto& f) {
        return acc + f.get();
    });
}

void TestCalculateMatrixSum() {
    const vector<vector<int>> matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
    return 0; 
}
