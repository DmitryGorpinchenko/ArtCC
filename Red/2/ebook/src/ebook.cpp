#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

static const int MAX_PAGE_NUM = 1'000;
static const int MAX_USER_ID = 100'000;

template <size_t MaxKey>
class IntMap {
public:
    void Update(int k, int v) {
        if (!has[k]) {
            has[k] = true;
            ++count;
        }
        vals[k] = v;
    }
    void Remove(int k) {
        if (has[k]) {
            has[k] = false;
            --count;
        }
    }
    
    int Val(int k) const { return vals[k]; }
    bool Has(int k) const { return has[k]; }
    int Size() const { return count; }

private:
    vector<int> vals = vector<int>(MaxKey + 1, 0);
    vector<bool> has = vector<bool>(MaxKey + 1, false);
    int count = 0;
};

class ReadingManager {
public:
    void Read(int user_id, int page_count) {
        if (page_counts.Has(user_id)) {
            --user_counts[page_counts.Val(user_id)];
        }
        page_counts.Update(user_id, page_count);
        ++user_counts[page_count];
    }

    double Cheer(int user_id) const {
        if (!page_counts.Has(user_id)) {
            return 0;
        }
        if (page_counts.Size() == 1) {
            return 1;
        }
        const int page = page_counts.Val(user_id);
        int res = 0;
        for (int i = 1; i < page; ++i) {
            res += user_counts[i];
        }
        return double(res) / (page_counts.Size() - 1);
    }

private:
    IntMap<MAX_USER_ID> page_counts;
    vector<int> user_counts = vector<int>(MAX_PAGE_NUM + 1, 0);
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}
