#include <algorithm>

using namespace std;

template<typename ForwardIterator, typename UnaryPredicate>
ForwardIterator max_element_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred) {
    auto res = find_if(first, last, pred);
    for (auto it = res; it != last; it = find_if(next(it), last, pred)) {
        if (*res < *it) {
            res = it;
        }
    }
    return res;
}
