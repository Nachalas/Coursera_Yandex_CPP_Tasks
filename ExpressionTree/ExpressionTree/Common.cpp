#include "Common.h"

#include <sstream>

using namespace std;

class intNode : public Expression {
public:
    intNode() = default;
    intNode(int valueI) : value(valueI) {}

    int Evaluate() const override {
        return value;
    }

    std::string ToString() const override {
        return to_string(value);
    }
private:
    int value;
};

class operationNode : public Expression {
public:
    operationNode() = default;
    operationNode(char operationI, unique_ptr<Expression> lhsI, unique_ptr<Expression> rhsI)
        : operation(operationI),
        lhs(move(lhsI)),
        rhs(move(rhsI))
    {}

    int Evaluate() const override {
        if (operation == '+') {
            return lhs->Evaluate() + rhs->Evaluate();
        }
        else if (operation == '*') {
            return lhs->Evaluate() * rhs->Evaluate();
        }
    }

    std::string ToString() const override {
        ostringstream ss;
        if (operation == '+') {
            ss << '(' + lhs->ToString() + ')';
            ss << '+';
            ss << '(' + rhs->ToString() + ')';
        }
        else if (operation == '*') {
            ss << '(' + lhs->ToString() + ')';
            ss << '*';
            ss << '(' + rhs->ToString() + ')';
        }
        return ss.str();
    }
private:
    char operation;
    unique_ptr<Expression> lhs;
    unique_ptr<Expression> rhs;
};

using ExpressionPtr = std::unique_ptr<Expression>;

ExpressionPtr Value(int value) {
    return make_unique<intNode>(value);
}
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<operationNode>('+', move(left), move(right));
}
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<operationNode>('*', move(left), move(right));
}