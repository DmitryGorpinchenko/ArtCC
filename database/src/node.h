#pragma once

#include "date.h"

#include <string>
#include <memory>

using namespace std;

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum class LogicalOperation {
    And,
    Or
};

class Node {
public:
    virtual bool Evaluate(const Date& d, const string& e) const = 0;
private:
};

class EmptyNode : public Node {
public:
    bool Evaluate(const Date& d, const string& e) const override {
        return true;
    }
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(Comparison c, const Date& d)
        : cmp(c)
        , date(d)
    {}
    
    bool Evaluate(const Date& d, const string&) const override;
private:
    Comparison cmp;
    Date date;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(Comparison c, const string& e)
        : cmp(c)
        , event(e)
    {}
    
    bool Evaluate(const Date&, const string& e) const override;
private:
    Comparison cmp;
    std::string event;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(LogicalOperation o, std::shared_ptr<Node> l, std::shared_ptr<Node> r)
        : op(o)
        , lhs(l)
        , rhs(r)
    {}
    
    bool Evaluate(const Date& d, const string& e) const override;
private:
    LogicalOperation op;
    std::shared_ptr<Node> lhs, rhs;
};
