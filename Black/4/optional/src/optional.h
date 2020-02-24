#pragma once

#include <utility>

struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;
    Optional(const T& elem) { *this = elem; }
    Optional(T&& elem) { *this = std::move(elem); }
    Optional(const Optional& other) { *this = other; }
    Optional(Optional&& other) { *this = std::move(other); }

    Optional& operator =(const T& elem) {
        if (!defined) {
            new (data) T(elem);
            defined = true;
        } else {
            *(*this) = elem;
        }
        return *this;
    }
    Optional& operator =(T&& elem) {
        if (!defined) {
            new (data) T(std::move(elem));
            defined = true;
        } else {
            *(*this) = std::move(elem);
        }
        return *this;
    }
    Optional& operator =(const Optional& other) {
        if (this != &other) {
            if (other.defined) {
                *this = *other;
            } else {
                Reset();
            }
        }
        return *this;
    }
    Optional& operator =(Optional&& other) {
        if (this != &other) {
            if (other.defined) {
                *this = std::move(*other);
            } else {
                Reset();
            }
        }
        return *this;
    }

    bool HasValue() const { return defined; }

    T& operator *() { return *reinterpret_cast<T*>(data); }
    const T& operator *() const { return *reinterpret_cast<const T*>(data); }
    T* operator ->() { return &*(*this); }
    const T* operator ->() const { return &*(*this); }

    T& Value() {
        return const_cast<T&>(static_cast<const Optional&>(*this).Value());
    }
    const T& Value() const {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return *(*this);
    }

    void Reset() {
        if (defined) {
            (*(*this)).~T();
            defined = false;
        }
    }

    ~Optional() {
        Reset();
    }
};
