#ifndef NEW_EXPRESSION_H
#define NEW_EXPRESSION_H

#include "Expression.h"
#include "BaseValue.h"
#include "Token.h"
#include "NumberType.h"
#include "StringType.h"
#include "CallExpression.h"
#include "ObjectExpression.h"
#include "Errors.h"
#include "IdentifierReference.h"

using namespace std;

class NewOperator : public Expression
{
public:
	NewOperator(Expression* new_expression);
	BaseValue* eval() override;
private:
	Expression* new_expression_;
};

#endif