#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> nums;
    nums.reserve(n);
    for (int i = n; i > 0; --i) {
        nums.push_back(i);
    }
    // nums is ordered here by construction
    do {
        for (auto num : nums) {
            cout << num << ' ';
        }
        cout << endl;
    } while (prev_permutation(begin(nums), end(nums)));
    return 0;
}
