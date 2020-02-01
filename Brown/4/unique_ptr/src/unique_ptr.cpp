#include "test_runner.h"

#include <cstddef>  // for nullptr_t

using namespace std;

template <typename T>
class UniquePtr {
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator =(const UniquePtr&) = delete;
public:
    UniquePtr() = default;
    UniquePtr(T* ptr) : data(ptr) {}
    UniquePtr(UniquePtr&& other) : data(other.Release()) {}
    ~UniquePtr() { delete data; }
    
    UniquePtr& operator =(nullptr_t) {
        Reset(nullptr);
        return *this;
    }
    UniquePtr& operator =(UniquePtr&& other) {
        if (this != &other) {
            UniquePtr tmp(move(other));
            Swap(tmp);
        }
        return *this;
    }

    T& operator *() const { return *data; }
    T* operator ->() const { return data; }
    
    T* Get() const { return data; }

    T* Release() {
        auto tmp = data;
        data = nullptr;
        return tmp;
    }

    void Reset(T* ptr) {
        UniquePtr tmp(ptr);
        Swap(tmp);
    }

    void Swap(UniquePtr& other) {
        swap(data, other.data);
    }
    
private:
    T* data = nullptr;
};

//

struct Item {
    static int counter;
    int value;
    Item(int v = 0): value(v) {
        ++counter;
    }
    Item(const Item& other): value(other.value) {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;

void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        ptr.Reset(new Item);
        ASSERT_EQUAL(Item::counter, 1);
    }
    ASSERT_EQUAL(Item::counter, 0);

    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        auto rawPtr = ptr.Release();
        ASSERT_EQUAL(Item::counter, 1);

        delete rawPtr;
        ASSERT_EQUAL(Item::counter, 0);
    }
    ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    ASSERT_EQUAL(ptr.Get()->value, 42);
    ASSERT_EQUAL((*ptr).value, 42);
    ASSERT_EQUAL(ptr->value, 42);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLifetime);
    RUN_TEST(tr, TestGetters);
    return 0;
}
