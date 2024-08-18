#ifndef NULL_EXPRESSION_H
#define NULL_EXPRESSION_H

#include "Expression.h"
#include "BaseValue.h"

using namespace std;

class NullExpression : public Expression
{
public:
	NullExpression();
	BaseValue* eval() override;
	const char* plain_name = "NullExpression";
};

#endif