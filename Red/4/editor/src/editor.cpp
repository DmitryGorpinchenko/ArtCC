#include "test_runner.h"

#include <vector>
using namespace std;

class Editor {
public:
    void Left() {
        if (!left.empty()) {
            char c = left.back();
            left.pop_back();
            right.push_back(c);
        }
    }
    void Right() {
        if (!right.empty()) {
            char c = right.back();
            right.pop_back();
            left.push_back(c);
        }
    }
    void Insert(char token) {
        left.push_back(token);
    }
    void Cut(size_t tokens = 1) {
        size_t cutted = Buffer(tokens);
        right.erase(end(right) - cutted, end(right));
    }
    void Copy(size_t tokens = 1) {
        Buffer(tokens);
    }
    void Paste() {
        left.insert(end(left), begin(buf), end(buf));
    }
    string GetText() const {
        string res;
        res.reserve(left.size() + right.size());
        res.insert(end(res), begin(left), end(left));
        res.insert(end(res), rbegin(right), rend(right));
        return res;
    }
    
private:
    size_t Buffer(size_t tokens) {
        size_t to_buf = min(tokens, right.size());
        buf.assign(rbegin(right), next(rbegin(right), to_buf));
        return to_buf;
    }

    vector<char> left, right, buf;
};

void TypeText(Editor& editor, const string& text) {
    for(char c : text) {
        editor.Insert(c);
    }
}

void TestEditing() {
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");
        for(size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for(size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
}

void TestReverse() {
    Editor editor;

    const string text = "esreveR";
    for(char c : text) {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "");

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
    Editor editor;

    editor.Paste();
    TypeText(editor, "example");
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
    return 0;
}
