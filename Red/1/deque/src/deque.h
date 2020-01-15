#pragma once

#include <vector>

template <typename T>
class Deque {
public:
    bool Empty() const {
        return front.empty() && back.empty();
    }
    size_t Size() const {
        return front.size() + back.size();
    }
    
    const T& operator [](size_t index) const {
        if (index < front.size()) {
            return front[front.size() - index - 1];
        }
        return back[index - front.size()];
    }
    T& operator [](size_t index) {
        return const_cast<T&>(static_cast<const Deque&>(*this)[index]);
    }
    const T& At(size_t index) const {
        if (index < front.size()) {
            return front.at(front.size() - index - 1);
        }
        return back.at(index - front.size());
    }
    T& At(size_t index) {
        return const_cast<T&>(static_cast<const Deque&>(*this).At(index));
    }
    
    const T& Front() const { return (*this)[0]; }
    T& Front() { return (*this)[0]; }
    const T& Back() const { return (*this)[Size() - 1]; }
    T& Back() { return (*this)[Size() - 1]; }

    void PushFront(const T& val) {
        front.push_back(val);
    }
    void PushBack(const T& val) {
        back.push_back(val);
    }

private:
    std::vector<T> front, back;
};
