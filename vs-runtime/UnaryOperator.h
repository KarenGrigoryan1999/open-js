#ifndef UNARY_OPERATOR_H
#define UNARY_OPERATOR_H

#include <vector>
#include "string"
#include "Expression.h"
#include "BaseValue.h"
#include "LexicalTokenType.h"
#include "Token.h"
#include "NumberType.h"
#include "BoolType.h"

using namespace std;

class UnaryOperator : public Expression
{
public:
	UnaryOperator(LexicalTokenType op, Expression* exp);
	BaseValue* eval() override;
private:
	LexicalTokenType op;
	Expression* exp;
};

#endif