#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate() {
        auto o = TryAllocate();
        if (!o) {
            o = new T;
            allocated.insert(o);
            return o;
        }
        return o;
    }
    T* TryAllocate() {
        if (!pool.empty()) {
            auto o = pool.front();
            allocated.insert(o);
            pool.pop();
            return o;
        }
        return nullptr;
    }

    void Deallocate(T* o) {
        if (allocated.erase(o)) {
            pool.push(o);
        } else {
            throw invalid_argument("object was not allocated previously");
        }
    }

    ~ObjectPool() {
        for (auto o : allocated) {
            delete o;
        }
        while (!pool.empty()) {
            auto o = pool.front();
            pool.pop();
            delete o;
        }
    }

private:
    queue<T*> pool;
    set<T*> allocated;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
