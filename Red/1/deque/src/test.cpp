#include "deque.h"
#include "test_runner.h"

#include <stdexcept>

void Test() {
    {
        Deque<int> d;
        ASSERT(d.Empty());
        try {
            d.At(0);
            ASSERT(false);
        } catch (std::out_of_range& e) {
        }
    }
    {
        Deque<int> d;
        d.PushFront(4);
        d.PushFront(3);
        d.PushBack(5);
        try {
            d.At(3);
            ASSERT(false);
        } catch (std::out_of_range& e) {
        }
    }
    {
        Deque<int> d;
        d.PushFront(4);
        d.PushFront(3);
        d.PushBack(5);
        ASSERT_EQUAL(d[1], 4);
        d[1] = 10;
        ASSERT_EQUAL(d[1], 10);
        d.At(2) = 11;
        ASSERT_EQUAL(d.At(2), 11);
    }
    {
        Deque<int> d;
        d.PushFront(4);
        d.PushFront(3);
        ASSERT_EQUAL(d[1], 4);
        d[1] = 10;
        ASSERT_EQUAL(d[1], 10);
        d.At(0) = 11;
        ASSERT_EQUAL(d.At(0), 11);
    }
    {
        Deque<int> d;
        d.PushBack(4);
        d.PushBack(3);
        ASSERT_EQUAL(d[1], 3);
        d[1] = 10;
        ASSERT_EQUAL(d[1], 10);
        d.At(0) = 11;
        ASSERT_EQUAL(d.At(0), 11);
    }
    {
        Deque<int> d;
        d.PushFront(4);
        d.PushFront(3);
        d.PushBack(5);
        ASSERT(!d.Empty());
        ASSERT_EQUAL(d.Size(), 3);
        ASSERT_EQUAL(d[0], 3);
        ASSERT_EQUAL(d[1], 4);
        ASSERT_EQUAL(d[2], 5);
        ASSERT_EQUAL(d.At(0), 3);
        ASSERT_EQUAL(d.At(1), 4);
        ASSERT_EQUAL(d.At(2), 5);
        ASSERT_EQUAL(d.Front(), 3);
        ASSERT_EQUAL(d.Back(), 5);
    }
    {
        Deque<int> d;
        d.PushFront(4);
        d.PushFront(3);
        d.PushFront(2);
        ASSERT(!d.Empty());
        ASSERT_EQUAL(d.Size(), 3);
        ASSERT_EQUAL(d[0], 2);
        ASSERT_EQUAL(d[1], 3);
        ASSERT_EQUAL(d[2], 4);
        ASSERT_EQUAL(d.At(0), 2);
        ASSERT_EQUAL(d.At(1), 3);
        ASSERT_EQUAL(d.At(2), 4);
        ASSERT_EQUAL(d.Front(), 2);
        ASSERT_EQUAL(d.Back(), 4);
    }
    {
        Deque<int> d;
        d.PushBack(4);
        d.PushBack(3);
        d.PushBack(2);
        ASSERT(!d.Empty());
        ASSERT_EQUAL(d.Size(), 3);
        ASSERT_EQUAL(d[0], 4);
        ASSERT_EQUAL(d[1], 3);
        ASSERT_EQUAL(d[2], 2);
        ASSERT_EQUAL(d.At(0), 4);
        ASSERT_EQUAL(d.At(1), 3);
        ASSERT_EQUAL(d.At(2), 2);
        ASSERT_EQUAL(d.Front(), 4);
        ASSERT_EQUAL(d.Back(), 2);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test);
    return 0;
}
