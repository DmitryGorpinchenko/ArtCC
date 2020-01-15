#include "test_runner.h"
#include <string>
#include <set>

using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target) {
        const auto& src = *data.emplace(source).first;
        const auto& trg = *data.emplace(target).first;
        forward[src] = trg;
        backward[trg] = src;
    }
    
    string_view TranslateForward(string_view source) const {
        if (auto it = forward.find(source); it != end(forward)) {
            return (*it).second;
        }
        return {};
    }
    
    string_view TranslateBackward(string_view target) const {
        if (auto it = backward.find(target); it != end(backward)) {
            return (*it).second;
        }
        return {};
    }

private:
    map<string_view, string_view> forward;
    map<string_view, string_view> backward;
    set<string> data;
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"), string("window"));
    translator.Add(string("stol"), string("table"));
    translator.Add(string("stol"), string("aaa"));

    ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
    ASSERT_EQUAL(translator.TranslateForward("stol"), "aaa");
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
    ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    return 0;
}
