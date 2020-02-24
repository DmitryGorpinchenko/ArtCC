#include "phone_book.h"
#include "contact.pb.h"

#include <iostream>
#include <algorithm>

PhoneBook::PhoneBook(std::vector<Contact> _contacts)
    : contacts(move(_contacts))
{
    std::sort(begin(contacts),
              end(contacts),
              [](const Contact& lhs, const Contact& rhs) {
                  return lhs.name < rhs.name;
              });
}

PhoneBook::ContactRange PhoneBook::FindByNamePrefix(std::string_view name_prefix) const {
    if (name_prefix.empty()) {
        return {
            begin(contacts),
            end(contacts)
        };
    }
    std::string next(name_prefix);
    ++next.back();
    const auto cmp = [](const Contact& lhs, std::string_view rhs) {
        return lhs.name < rhs;
    };
    auto l = lower_bound(begin(contacts), end(contacts), name_prefix, cmp);
    auto r = lower_bound(l, end(contacts), next, cmp);
    return { l, r };
}

namespace {

Contact ToContact(PhoneBookSerialize::Contact&& c) {
    Contact res;
    res.name = std::move(*c.mutable_name());
    if (c.has_birthday()) {
        res.birthday = {
            .year  = c.birthday().year(),
            .month = c.birthday().month(),
            .day   = c.birthday().day()
        };
    }
    res.phones.reserve(c.phone_number_size());
    for (auto& p : *c.mutable_phone_number()) {
        res.phones.push_back(std::move(p));
    }
    return res;
}

ContactList ToContactList(PhoneBookSerialize::ContactList&& l) {
    ContactList res;
    res.reserve(l.contact_size());
    for (auto& c : *l.mutable_contact()) {
        res.push_back(ToContact(std::move(c)));
    }
    return res;
}

PhoneBookSerialize::Date FromDate(const Date& d) {
    PhoneBookSerialize::Date res;
    res.set_year(d.year);
    res.set_month(d.month);
    res.set_day(d.day);
    return res;
}

PhoneBookSerialize::Contact FromContact(const Contact& c) {
    PhoneBookSerialize::Contact res;
    res.set_name(c.name);
    if (c.birthday) {
        *res.mutable_birthday() = FromDate(*c.birthday);
    }
    for (const auto& p : c.phones) {
        res.add_phone_number(p);
    }
    return res;
}

PhoneBookSerialize::ContactList FromContactList(const ContactList& l) {
    PhoneBookSerialize::ContactList res;
    for (const auto& c : l) {
        *res.add_contact() = FromContact(c);
    }
    return res;
}

}

void PhoneBook::SaveTo(std::ostream& output) const {
    FromContactList(contacts).SerializeToOstream(&output);
}

PhoneBook DeserializePhoneBook(std::istream& input) {
    PhoneBookSerialize::ContactList list;
    list.ParseFromIstream(&input);
    return PhoneBook(ToContactList(std::move(list)));
}

