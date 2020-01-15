#include "test_runner.h"

#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

template <typename Token>
using Sentence = vector<Token>;

template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    vector<Sentence<Token>> res;
    auto it = begin(tokens);
    while (it != end(tokens)) {
        auto punct_begin = find_if(it, end(tokens), [](const Token& t) { return t.IsEndSentencePunctuation(); });
        auto punct_end = find_if(punct_begin, end(tokens), [](const Token& t) { return !t.IsEndSentencePunctuation(); });
        res.push_back({ make_move_iterator(it), make_move_iterator(punct_end) });
        it = punct_end;
    }
    return res;
}

struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator ==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

void TestSplitting() {
    ASSERT_EQUAL(
        SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
        vector<Sentence<TestToken>>({
            {{"Split"}, {"into"}, {"sentences"}, {"!"}}
        })
    );

    ASSERT_EQUAL(
        SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
        vector<Sentence<TestToken>>({
            {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
        })
    );

    ASSERT_EQUAL(
        SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
        vector<Sentence<TestToken>>({
            {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
            {{"Without"}, {"copies"}, {".", true}},
        })
    );
}


struct NonCopyableType {
    NonCopyableType(const NonCopyableType&) = delete;
    NonCopyableType& operator =(const NonCopyableType&) = delete;
    
    NonCopyableType(NonCopyableType&&) = default;
    NonCopyableType& operator =(NonCopyableType&&) = default;

    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator ==(const NonCopyableType& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const NonCopyableType& token) {
    return stream << token.data;
}

void TestNonCopyable() {
    SplitIntoSentences(vector<NonCopyableType>());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSplitting);
    RUN_TEST(tr, TestNonCopyable);
    return 0;
}
