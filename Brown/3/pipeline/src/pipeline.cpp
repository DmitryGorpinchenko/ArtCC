#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>

using namespace std;

struct Email {
    string from;
    string to;
    string body;
};

istream& operator >>(istream& is, Email& val) {
    getline(is, val.from);
    getline(is, val.to);
    getline(is, val.body);
    return is;
}

ostream& operator <<(ostream& os, const Email& val) {
    return os << val.from << '\n'
              << val.to << '\n'
              << val.body;
}

class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        throw logic_error("Unimplemented");
    }

protected:
    void PassOn(unique_ptr<Email> email) const {
        if (next_worker) {
            next_worker->Process(move(email));
        }
    }

public:
    void SetNext(unique_ptr<Worker> next) {
        next_worker = move(next);
    }
    
private:
    unique_ptr<Worker> next_worker;
};

class Reader : public Worker {
public:
    Reader(istream& in) : is(in) {}
    
    void Process(unique_ptr<Email> email) override {}
    
    void Run() override {
        Email mail;
        while (is >> mail) {
            PassOn(make_unique<Email>(move(mail)));
        }
    }
private:
    istream& is;
};

class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;

    Filter(Function f) : pred(f) {}

    void Process(unique_ptr<Email> email) override {
        if (pred(*email)) {
            PassOn(move(email));
        }
    }

public:
    Function pred;
};

class Copier : public Worker {
public:
    Copier(string recipient) : copy_to(move(recipient)) {}
    
    void Process(unique_ptr<Email> email) override {
        PassOn(make_unique<Email>(*email));
        if (email->to != copy_to) {
            email->to = copy_to;
            PassOn(move(email));
        }
    }
private:
    string copy_to;
};

class Sender : public Worker {
public:
    Sender(ostream& out) : os(out) {}
    
    void Process(unique_ptr<Email> email) override {
        os << *email << '\n';
        PassOn(move(email));
    }
private:
    ostream& os;
};

class PipelineBuilder {
public:
    explicit PipelineBuilder(istream& in) {
        workers.push_back(make_unique<Reader>(in));
    }

    PipelineBuilder& FilterBy(Filter::Function filter) {
        workers.push_back(make_unique<Filter>(filter));
        return *this;
    }

    PipelineBuilder& CopyTo(string recipient) {
        workers.push_back(make_unique<Copier>(move(recipient)));
        return *this;
    }

    PipelineBuilder& Send(ostream& out) {
        workers.push_back(make_unique<Sender>(out));
        return *this;
    }

    unique_ptr<Worker> Build() {
        for (auto it = prev(end(workers)); it != begin(workers); ) {
            (*prev(it))->SetNext(move(*it));
            it = prev(workers.erase(it));
        }
        return move(workers.front());
    }

private:
    list<unique_ptr<Worker>> workers;
};

void TestSanity() {
    string input = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "ralph@example.com\n"
        "erich@example.com\n"
        "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "erich@example.com\n"
        "richard@example.com\n"
        "Are you sure you pressed the right button?\n"
    );

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}
