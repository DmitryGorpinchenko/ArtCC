#include "node.h"

template <typename T>
bool Cmp(Comparison cmp, const T& lhs, const T& rhs) {
    switch (cmp) {
    case Comparison::Less:            return lhs < rhs;
    case Comparison::LessOrEqual:     return !(rhs < lhs);
    case Comparison::Greater:         return rhs < lhs;
    case Comparison::GreaterOrEqual:  return !(lhs < rhs);
    case Comparison::Equal:           return (!(lhs < rhs)) && (!(rhs < lhs));
    case Comparison::NotEqual:        return (lhs < rhs) || (rhs < lhs);
    default: break;
    }
    return false;
}

bool DateComparisonNode::Evaluate(const Date& d, const string&) const {
    return Cmp(cmp, d, date);
}

bool EventComparisonNode::Evaluate(const Date&, const string& e) const {
    return Cmp(cmp, e, event);
}

bool LogicalOperationNode::Evaluate(const Date& d, const string& e) const {
    switch (op) {
    case LogicalOperation::And: return lhs->Evaluate(d, e) && rhs->Evaluate(d, e);
    case LogicalOperation::Or:  return lhs->Evaluate(d, e) || rhs->Evaluate(d, e);
    default: break;
    }
    return false;
}
