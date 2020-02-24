#include "yellow_pages.h"

#include <algorithm>
#include <tuple>
#include <set>
#include <optional>
#include <functional>

namespace YellowPages {

bool operator <(const Name& lhs, const Name& rhs) {
    return std::make_tuple(lhs.value(), lhs.type())
         < std::make_tuple(rhs.value(), rhs.type());
}
bool operator <(const Phone& lhs, const Phone& rhs) {
    return std::make_tuple(lhs.formatted(), lhs.type(), lhs.country_code(), lhs.local_code(), lhs.number(), lhs.extension(), lhs.description())
         < std::make_tuple(rhs.formatted(), rhs.type(), rhs.country_code(), rhs.local_code(), rhs.number(), rhs.extension(), rhs.description());
}
bool operator <(const Url& lhs, const Url& rhs) {
    return lhs.value() < rhs.value();
}

template <typename Has, typename Acc, typename Mut>
void FillSingle(Has has, Acc acc, Mut mut, const Signals& ordered_signals, Company& c) {
    for (const auto& s : ordered_signals) {
        if ((s.company().*has)()) {
            *(c.*mut)() = (s.company().*acc)();
            break;
        }
    }
}

template <typename Field, typename Acc, typename Mut>
void FillRepeated(Acc acc, Mut mut, const Signals& ordered_signals, const Providers& providers, Company& c) {
    std::optional<uint32_t> max_priority;
    std::set<Field> items;
    for (const auto& s : ordered_signals) {
        const auto priority = providers.at(s.provider_id()).priority();
        if (max_priority && *max_priority != priority) {
            break;
        }
        for (const auto& i : acc(s.company())) {
            if (!max_priority) {
                max_priority = priority;
            }
            items.insert(i);
        }
    }
    for (const auto& i : items) {
        *(c.*mut)() = i;
    }
}

Company Merge(const Signals& signals, const Providers& providers) {
    Company res;
    const Signals ordered_signals = [&providers, &signals]() {
        Signals res = signals;
        std::sort(rbegin(res),
                  rend(res),
                  [&providers](const Signal& lhs, const Signal& rhs) {
                      return providers.at(lhs.provider_id()).priority()
                           < providers.at(rhs.provider_id()).priority();
                  });
        return res;
    }();
    FillSingle(&Company::has_address,      &Company::address,      &Company::mutable_address,      ordered_signals, res);
    FillSingle(&Company::has_working_time, &Company::working_time, &Company::mutable_working_time, ordered_signals, res);
    FillRepeated<Name> ([](auto& c)->auto& { return c.names(); },  &Company::add_names,  ordered_signals, providers, res);
    FillRepeated<Phone>([](auto& c)->auto& { return c.phones(); }, &Company::add_phones, ordered_signals, providers, res);
    FillRepeated<Url>  ([](auto& c)->auto& { return c.urls(); },   &Company::add_urls,   ordered_signals, providers, res);
    return res;
}

}
