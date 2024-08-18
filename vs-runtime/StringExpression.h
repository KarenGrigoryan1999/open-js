#ifndef STRING_EXPRESSION_H
#define STRING_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "BaseValue.h"

using namespace std;

class StringExpression : public Expression
{
public:
	StringExpression(string value);
	BaseValue* eval() override;
	const char* plain_name = "StringExpression";
private:
	string value;
};

#endif