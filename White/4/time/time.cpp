#include <iostream>
#include <exception>
#include <string>
using namespace std;

string AskTimeServer() {
    static int count = 0;
    ++count;
    if (count < 2) {
        return "01:01:01";
    } else if (count < 3) {
        throw domain_error("domain error");
    } else if (count < 4) {
        throw system_error(error_code());
    }
    return "15:12:05";
}

class TimeServer {
public:
    string GetCurrentTime() {
        try {
            last_fetched_time = AskTimeServer();
        } catch (system_error&) {
        }
        return last_fetched_time;
    }

private:
    string last_fetched_time = "00:00:00";
};

int main() {
    TimeServer ts;
    for (int i = 0; i < 4; ++i) {
        try {
            cout << ts.GetCurrentTime() << endl;
        } catch (exception& e) {
            cout << "Exception got: " << e.what() << endl;
        }
    }
    return 0;
}
