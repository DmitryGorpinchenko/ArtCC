#pragma once

#include <cstddef>
#include <utility>
#include <memory>

template <typename T>
struct RawMemory {
    T* buf = nullptr;
    size_t cp = 0;
    
    RawMemory() = default;
    RawMemory(size_t n) : buf(New(n)), cp(n) {}
    RawMemory(const RawMemory&) = delete;
    RawMemory(RawMemory&& other) {
        Swap(other);
    }
    ~RawMemory() {
        Delete(buf);
    }
    
    RawMemory& operator =(const RawMemory&) = delete;
    RawMemory& operator =(RawMemory&& rhs) noexcept {
        Swap(rhs);
        return *this;
    }
    
    void Swap(RawMemory& other) noexcept {
        std::swap(buf, other.buf);
        std::swap(cp, other.cp);
    }
    
    static T* New(size_t n) {
        return static_cast<T*>(operator new(sizeof(T) * n));
    }
    static void Delete(T* buf) {
        operator delete(buf);
    }
};

template <typename T>
class Vector {
public:
    Vector() = default;
    Vector(size_t n)
        : data(n)
    {
        std::uninitialized_value_construct_n(begin(), n);
        sz = n;
    }
    Vector(const Vector& other)
        : data(other.sz)
    {
        std::uninitialized_copy(other.begin(), other.end(), begin());
        sz = other.sz;
    }
    Vector(Vector&& other) {
        Swap(other);
    }

    ~Vector() {
        std::destroy(begin(), end());
    }

    Vector& operator =(const Vector& rhs) {
        if (this != &rhs) {
            if (rhs.sz > data.cp) {
                Vector tmp(rhs);
                Swap(tmp);
            } else {
                size_t i = 0;
                for (; i < sz && i < rhs.sz; ++i) {
                    (*this)[i] = rhs[i];
                }
                if (i < sz) {
                    std::destroy(begin() + i, begin() + sz);
                } else if (i < rhs.sz) {
                    std::uninitialized_copy(rhs.begin() + i, rhs.end(), begin() + i);
                }
                sz = rhs.sz;
            }
        }
        return *this;
    }
    Vector& operator =(Vector&& rhs) noexcept {
        Swap(rhs);
        return *this;
    }

    void Reserve(size_t n) {
        if (n > data.cp) {
            RawMemory<T> tmp(n);
            std::uninitialized_move(begin(), end(), tmp.buf);
            std::destroy(begin(), end());
            data.Swap(tmp);
        }
    }

    void Resize(size_t n) {
        if (n > sz) {
            Reserve(n);
            std::uninitialized_value_construct(begin() + sz, begin() + n);
        } else if (n < sz) {
            std::destroy(begin() + n, begin() + sz);
        }
        sz = n;
    }

    void PushBack(const T& elem) {
        Reallocate();
        Construct(end(), elem);
        ++sz;
    }
    void PushBack(T&& elem) {
        Reallocate();
        Construct(end(), std::move(elem));
        ++sz;
    }
    template <typename ... Args>
    T& EmplaceBack(Args&&... args) {
        Reallocate();
        Construct(end(), std::forward<Args>(args)...);
        ++sz;
        return (*this)[sz - 1];
    }

    void PopBack() {
        std::destroy_at(end() - 1);
        --sz;
    }

    size_t Size() const noexcept { return sz; }
    size_t Capacity() const noexcept { return data.cp; }

    const T& operator [](size_t i) const { return data.buf[i]; }
    T& operator [](size_t i) { return data.buf[i]; }
    
    void Swap(Vector& other) noexcept {
        data.Swap(other.data);
        std::swap(sz, other.sz);
    }

    using iterator = T*;
    using const_iterator = const T*;

    iterator begin() noexcept { return data.buf; }
    iterator end() noexcept { return data.buf + sz; }

    const_iterator begin() const noexcept { return data.buf; }
    const_iterator end() const noexcept { return data.buf + sz; }

    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

    iterator Insert(const_iterator pos, const T& elem) {
        if (pos == cend()) {
            PushBack(elem);
            return end() - 1;
        }
        const auto dist = pos - begin();
        RawMemory<T> tmp(sz + 1);
        std::uninitialized_move(begin(), begin() + dist, tmp.buf);
        Construct(tmp.buf + dist, elem);
        std::uninitialized_move(begin() + dist, end(), tmp.buf + dist + 1);
        data.Swap(tmp);
        ++sz;
        return begin() + dist;
    }
    iterator Insert(const_iterator pos, T&& elem) {
        if (pos == cend()) {
            PushBack(std::move(elem));
            return end() - 1;
        }
        const auto dist = pos - begin();
        RawMemory<T> tmp(sz + 1);
        std::uninitialized_move(begin(), begin() + dist, tmp.buf);
        Construct(tmp.buf + dist, std::move(elem));
        std::uninitialized_move(begin() + dist, end(), tmp.buf + dist + 1);
        data.Swap(tmp);
        ++sz;
        return begin() + dist;
    }
    template <typename ... Args>
    iterator Emplace(const_iterator pos, Args&&... args) {
        if (pos == cend()) {
            EmplaceBack(std::forward<Args>(args)...);
            return end() - 1;
        }
        const auto dist = pos - begin();
        RawMemory<T> tmp(sz + 1);
        std::uninitialized_move(begin(), begin() + dist, tmp.buf);
        Construct(tmp.buf + dist, std::forward<Args>(args)...);
        std::uninitialized_move(begin() + dist, end(), tmp.buf + dist + 1);
        data.Swap(tmp);
        ++sz;
        return begin() + dist;
    }

    iterator Erase(const_iterator pos) {
        if (pos == cend() - 1) {
            PopBack();
            return end();
        }
        const auto dist = pos - begin();
        RawMemory<T> tmp(sz - 1);
        std::uninitialized_move(begin(), begin() + dist, tmp.buf);
        std::uninitialized_move(begin() + dist + 1, end(), tmp.buf + dist);
        data.Swap(tmp);
        --sz;
        return begin() + dist;
    }
    
private:
    template <typename... Args>
    void Construct(T* buf, Args&&... args) {
        new (buf) T(std::forward<Args>(args)...);
    }
    
    void Reallocate() {
        if (sz == data.cp) {
            Reserve((sz == 0) ? 1 : (2 * sz));
        }
    }

    RawMemory<T> data;
    size_t sz = 0;
};
