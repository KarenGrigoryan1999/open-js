#ifndef UNDEFINED_EXPRESSION_H
#define UNDEFINED_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "BaseValue.h"

using namespace std;

class UndefinedExpression : public Expression
{
public:
	UndefinedExpression();
	BaseValue* eval() override;
	const char* plain_name = "UndefinedExpression";
};

#endif