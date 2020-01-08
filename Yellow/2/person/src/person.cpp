#include "person.h"

void Person::ChangeFirstName(int year, const std::string& first_name) {
    first_names[year] = first_name;
}

void Person::ChangeLastName(int year, const std::string& last_name) {
    last_names[year] = last_name;
}

std::string Person::GetFullName(int year) const {
    auto fn_it = first_names.upper_bound(year);
    auto ln_it = last_names.upper_bound(year);
    const auto fn = (fn_it == std::begin(first_names)) ? "" : (*std::prev(fn_it)).second;
    const auto ln = (ln_it == std::begin(last_names)) ? "" : (*std::prev(ln_it)).second;
    if (fn.empty() && ln.empty()) {
        return "Incognito";
    }
    if (fn.empty()) {
        return ln + " with unknown first name";
    }
    if (ln.empty()) {
        return fn + " with unknown last name";
    }
    return fn + " " + ln;
}
