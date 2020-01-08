#pragma once

#include <string>

class PhoneNumber {
public:
    explicit PhoneNumber(const std::string &international_number);

    std::string GetCountryCode() const;
    std::string GetCityCode() const;
    std::string GetLocalNumber() const;
    std::string GetInternationalNumber() const;

private:
    std::string country_code_;
    std::string city_code_;
    std::string local_number_;
};
