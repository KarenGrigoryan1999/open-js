#ifndef NAN_EXPRESSION_H
#define NAN_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "BaseValue.h"

using namespace std;

class NanExpression : public Expression
{
public:
	NanExpression();
	BaseValue* eval() override;
	const char* plain_name = "NanExpression";
};

#endif