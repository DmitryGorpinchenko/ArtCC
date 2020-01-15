#include "test_runner.h"
#include <sstream>
#include <string>
using namespace std;

class Logger {
public:
    explicit Logger(ostream& output_stream) : os(output_stream) {}

    void SetLogLine(bool value) { log_line = value; }
    void SetLogFile(bool value) { log_file= value; }

    void SetFile(const string& f) { file = f; }
    void SetLine(int l) { line = l; }

    void Log(const string& message) {
        if (log_file && log_line) {
            os << file << ':' << line << ' ';
        } else if (log_line) {
            os << "Line " << line << ' ';
        } else if (log_file) {
            os << file << ' ';
        }
        os << message << endl;
    }

private:
    ostream& os;
    string file;
    int line = 0;
    bool log_line = false;
    bool log_file = false;
};

#define LOG(logger, message)        \
    do {                            \
        logger.SetFile(__FILE__);   \
        logger.SetLine(__LINE__);   \
        logger.Log(message);        \
    } while (false)                 \

void TestLog() {
#line 1 "logger.cpp" // redefines 'reference point' for lines count for testing

    ostringstream logs;
    Logger l(logs);
    LOG(l, "hello");

    l.SetLogFile(true);
    LOG(l, "hello");

    l.SetLogLine(true);
    LOG(l, "hello");

    l.SetLogFile(false);
    LOG(l, "hello");

    string expected = "hello\n";
    expected += "logger.cpp hello\n";
    expected += "logger.cpp:10 hello\n";
    expected += "Line 13 hello\n";
    ASSERT_EQUAL(logs.str(), expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLog);
    return 0;
}
