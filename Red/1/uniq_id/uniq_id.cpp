#include <string>
#include <vector>
using namespace std;

#define UID_IMPL2(line) _ ## line
#define UID_IMPL1(line) UID_IMPL2(line)
#define UNIQ_ID UID_IMPL1(__LINE__)

int main() {
    int UNIQ_ID = 0;
    string UNIQ_ID = "hello";
    vector<string> UNIQ_ID = {"hello", "world"};
    vector<int> UNIQ_ID = {1, 2, 3, 4};
    return 0;
}
