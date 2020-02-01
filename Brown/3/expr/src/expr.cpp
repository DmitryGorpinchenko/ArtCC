#include "Common.h"

using namespace std;

namespace Nodes {

struct Value : Expression {
    int val;
    
    Value(int v) : val(v) {} 
    
    int Evaluate() const override {
        return val;
    }
    
    string ToString() const override {
        return to_string(val);
    }
};

struct Sum : Expression {
    ExpressionPtr lhs, rhs;
    
    Sum(ExpressionPtr l, ExpressionPtr r) : lhs(move(l)), rhs(move(r)) {}
    
    int Evaluate() const override {
        return lhs->Evaluate() + rhs->Evaluate();
    }
    
    string ToString() const override {
        return '(' + lhs->ToString() + ")+(" + rhs->ToString() + ')';
    }
};

struct Product : Expression {
    ExpressionPtr lhs, rhs;
    
    Product(ExpressionPtr l, ExpressionPtr r) : lhs(move(l)), rhs(move(r)) {}
    
    int Evaluate() const override {
        return lhs->Evaluate() * rhs->Evaluate();
    }
    
    string ToString() const override {
        return '(' + lhs->ToString() + ")*(" + rhs->ToString() + ')';
    }
};

}

ExpressionPtr Value(int value) {
    return make_unique<Nodes::Value>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<Nodes::Sum>(move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<Nodes::Product>(move(left), move(right));
}
