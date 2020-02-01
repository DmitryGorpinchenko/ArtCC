#pragma once

#include <mutex>
#include <shared_mutex>

template <typename T>
class Synchronized {
    class Read {
        std::shared_lock<std::shared_mutex> lock;
    public:
        const T& ref_to_value;
        
        Read(const T& val, std::shared_mutex& m)
            : lock(m)
            , ref_to_value(val)
        {}
    };
    class Write {
        std::unique_lock<std::shared_mutex> lock;
    public:
        T& ref_to_value;
        
        Write(T& val, std::shared_mutex& m)
            : lock(m)
            , ref_to_value(val)
        {}
    };
public:
    explicit Synchronized(T initial = T()) : value(std::move(initial)) {}

    Write GetWriteAccess() { return Write(value, m); }
    Read GetReadAccess() const { return Read(value, m); }

private:
    mutable std::shared_mutex m;
    T value;
};
