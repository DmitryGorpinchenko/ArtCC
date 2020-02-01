#pragma once

#include <utility>

namespace RAII {

template <typename Provider>
class Booking {
public:
    Booking(Provider* p, typename Provider::BookingId i)
        : provider(p)
        , id(i)
    {}
    Booking(const Booking&) = delete;
    Booking& operator =(const Booking&) = delete;
    Booking(Booking&& other) : provider(other.provider), id(other.id) {
        other.provider = nullptr;
    }
    Booking& operator =(Booking&& rhs) {
        if (this != &rhs) {
            Booking tmp(std::move(rhs));
            Swap(tmp);
        }
        return *this;
    }
    
    ~Booking() {
        if (provider) {
            provider->CancelOrComplete(*this);
        }
    }
    
    void Swap(Booking& other) {
        std::swap(provider, other.provider);
        std::swap(id, other.id);
    }
    
private:
    Provider* provider;
    typename Provider::BookingId id;
};

}
