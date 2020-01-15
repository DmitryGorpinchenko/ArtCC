#include "test_runner.h"

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <future>
using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator +=(const Stats& other) {
        for (const auto& [w, freq] : other.word_frequences) {
            word_frequences[w] += freq;
        }
    }
};

vector<string> ReadBatch(istream& input, size_t size) {
    vector<string> res;
    res.reserve(size);
    for (string line; (size > 0) && getline(input, line); --size) {
        res.push_back(move(line));
    }
    return res;
}

Stats ExploreDoc(const set<string>& key_words, string_view doc) {
    Stats res;
    do {
        size_t lpos = doc.find_first_not_of(' ');
        if (lpos == doc.npos) {
            break;
        }
        size_t rpos = doc.find(' ', lpos + 1);
        string word(doc.substr(lpos, rpos - lpos));
        if (key_words.count(word)) {
            ++res.word_frequences[move(word)];
        }
        if (rpos == doc.npos) {
            break;
        }
        doc.remove_prefix(rpos + 1);
    } while (true);
    return res;
}

template <typename DocIt>
Stats ExploreBatch(const set<string>& key_words, DocIt first, DocIt last) {
    Stats res;
    for (auto it = first; it != last; ++it) {
        res += ExploreDoc(key_words, *it);
    }
    return res;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    vector<future<Stats>> futures;
    do {
        const auto batch = ReadBatch(input, 5000);
        if (batch.empty()) {
            break;
        }
        futures.push_back(
            async([&key_words, batch{move(batch)}]() {
                return ExploreBatch(key_words, begin(batch), end(batch));
            })
        );
    } while (true);
    Stats res;
    for (auto& f : futures) {
        res += f.get();
    }
    return res;
}

void TestBasic() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
        {"yangle", 6},
        {"rocks", 2},
        {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
    return 0;
}
