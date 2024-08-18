#ifndef BOOLEAN_EXPRESSION_H
#define BOOLEAN_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "BaseValue.h"
#include "BoolType.h"

using namespace std;

class BooleanExpression : public Expression
{
public:
	BooleanExpression(string value);
	BaseValue* eval() override;
	const char* plain_name = "BooleanExpression";
private:
	string value;
};

#endif