#ifndef LOGICAL_EXPRESSION_H
#define LOGICAL_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "LexicalTokenType.h"

using namespace std;

class LogicalOperator : public Expression
{
public:
	LogicalOperator(LexicalTokenType op, Expression* exp1, Expression* exp2);
	BaseValue* eval() override;
private:
	bool IsLooselyEqual(BaseValue* x, BaseValue* y);
	bool IsStrictlyEqual(BaseValue* x, BaseValue* y);
	bool IsLessThan(BaseValue* x, BaseValue* y);
	bool IsLooselyLessThan(BaseValue* x, BaseValue* y);
	LexicalTokenType op;
	Expression* exp1;
	Expression* exp2;
};

#endif