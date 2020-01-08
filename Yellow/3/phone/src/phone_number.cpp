#include "phone_number.h"

#include <sstream>
#include <exception>

PhoneNumber::PhoneNumber(const std::string &international_number) {
    std::istringstream ss(international_number);
    if (ss.peek() == '+') {
        ss.ignore(1);
        getline(ss, country_code_, '-');
        getline(ss, city_code_, '-');
        getline(ss, local_number_);
    }
    if (country_code_.empty() || city_code_.empty() || local_number_.empty()) {
        throw std::invalid_argument("");
    }
}

std::string PhoneNumber::GetCountryCode() const { return country_code_; }
std::string PhoneNumber::GetCityCode() const { return city_code_; }
std::string PhoneNumber::GetLocalNumber() const { return local_number_; }

std::string PhoneNumber::GetInternationalNumber() const {
    std::ostringstream ss;
    ss << '+' << country_code_ << '-' << city_code_ << '-' << local_number_;
    return ss.str();
}
