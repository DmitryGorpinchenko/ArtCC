#pragma once

#include "iterator_range.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <iosfwd>
#include <utility>

struct Date {
    int year, month, day;
};

struct Contact {
    std::string name;
    std::optional<Date> birthday;
    std::vector<std::string> phones;
};
using ContactList = std::vector<Contact>; 

class PhoneBook {
public:
    explicit PhoneBook(ContactList _contacts);

    using ContactRange = IteratorRange<ContactList::const_iterator>;
    ContactRange FindByNamePrefix(std::string_view name_prefix) const;

    void SaveTo(std::ostream& output) const;
    
private:
    ContactList contacts;
};

PhoneBook DeserializePhoneBook(std::istream& input);

