#include "test_runner.h"

#include <vector>
#include <utility>

using namespace std;

template <typename T>
class Table {
public:
    Table(size_t rows, size_t cols) {
        Resize(rows, cols);
    }
    
    pair<size_t, size_t> Size() const { return { data.size(), (data.empty() ? 0 : data.at(0).size()) }; }
    
    void Resize(size_t rows, size_t cols) {
        data.resize(rows);
        for (auto& r : data) {
            r.resize(cols);
        }
    }

    const vector<T>& operator [](size_t row) const {
        return data[row];
    }
    vector<T>& operator [](size_t row) {
        return data[row];
    }

private:
    vector<vector<T>> data;
};

void TestTable() {
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u);
    ASSERT_EQUAL(t.Size().second, 1u);
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);
    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u);
    ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    return 0;
}
