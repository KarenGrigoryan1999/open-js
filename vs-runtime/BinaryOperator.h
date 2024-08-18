#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H

#include <vector>
#include "string"
#include "Expression.h"
#include "LexicalTokenType.h"
#include "Token.h"
#include "NumberType.h"
#include "StringType.h"
#include "LexicalTokenType.h"
#include "NanType.h"
#include "ObjectExpression.h"
#include "CallExpression.h"
#include "Errors.h"

using namespace std;

class BinaryOperator : public Expression
{
public:
	BinaryOperator(LexicalTokenType op, Expression* exp1, Expression* exp2);
	BaseValue* eval() override;
private:
	BaseValue* ToPrimitive(BaseValue* obj, ValueTypes preferred_type);
	BaseValue* CallConvertationMethod(BaseValue* method, BaseValue* obj);
	LexicalTokenType op;
	Expression* exp1;
	Expression* exp2;
};

#endif