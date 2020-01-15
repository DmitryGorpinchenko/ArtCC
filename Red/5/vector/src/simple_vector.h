#pragma once

#include <cstdlib>
#include <algorithm>

template <typename T>
class SimpleVector {
public:
    SimpleVector() = default;
    SimpleVector(const SimpleVector& rhs)
        : SimpleVector(rhs.size_)
    {
        std::copy(rhs.begin(), rhs.end(), begin());
    }
    SimpleVector(SimpleVector&& rhs)
    {
        Swap(rhs);
    }
    explicit SimpleVector(size_t size)
        : data_(new T[size])
        , capacity_(size)
        , size_(size)
    {}
    ~SimpleVector() {
        delete [] data_;
    }
    
    SimpleVector& operator =(const SimpleVector& rhs) {
        if (this != &rhs) {
            auto tmp = rhs;
            Swap(tmp);
        }
        return *this;
    }
    SimpleVector& operator =(SimpleVector&& rhs) {
        if (this != &rhs) {
            auto tmp = move(rhs);
            Swap(tmp);
        }
        return *this;
    }

    T& operator [](size_t index) { return data_[index]; }
    const T& operator [](size_t index) const { return data_[index]; }

    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }

    size_t Size() const { return size_; }
    size_t Capacity() const { return capacity_; }
    
    void PushBack(const T& value) {
        Realloc(size_ + 1);
        data_[size_] = value;
        ++size_;
    }
    void PushBack(T&& value) {
        Realloc(size_ + 1);
        data_[size_] = std::move(value);
        ++size_;
    }

private:
    void Swap(SimpleVector& rhs) {
        std::swap(data_, rhs.data_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(size_, rhs.size_);
    }

    void Realloc(size_t min_size) {
        if (capacity_ < min_size) {
            const auto new_capacity = std::max(min_size, 2 * capacity_);
            const auto tmp = new T[new_capacity];
            std::move(begin(), end(), tmp);
            delete[] data_;
            data_ = tmp;
            capacity_ = new_capacity;
        }
    }

    T* data_ = nullptr;
    size_t capacity_ = 0;
    size_t size_ = 0;
};
