#include "UnaryOperator.h"

UnaryOperator::UnaryOperator(LexicalTokenType op, Expression* exp) {
	this->op = op;
	this->exp = exp;
}

BaseValue* UnaryOperator::eval() {
	if (this->op == LexicalTokenType::PLUS) {
		return exp->eval();
	}
	if (this->op == LexicalTokenType::MINUS) {
		return new NumberType(exp->eval()->GetAsNumber()*(-1));
	}
	if (this->op == LexicalTokenType::INVERSE) {
		return new BoolType(!exp->eval()->GetAsBoolean());
	}
}