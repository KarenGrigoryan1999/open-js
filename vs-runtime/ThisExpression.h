#ifndef THIS_EXPRESSION_H
#define THIS_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "BaseValue.h"

using namespace std;

class ThisExpression : public Expression
{
public:
	ThisExpression();
	BaseValue* eval() override;
	const char* plain_name = "ThisExpression";
};

#endif