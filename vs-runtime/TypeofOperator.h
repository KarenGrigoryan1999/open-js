#ifndef TYPEOF_STATEMENT_H
#define TYPEOF_STATEMENT_H

#include <vector>
#include "iostream"
#include "string"
#include "Statement.h"
#include "Expression.h"
#include "Token.h"
#include "Expression.h"
#include "StringType.h"
#include "ReferenceError.h"

using namespace std;

class TypeofOperator : public Expression
{
public:
	TypeofOperator(Expression* unary_expression);
	BaseValue* eval() override;
	const char* plain_name = "TypeofOperator";
private:
	Expression* unary_expression_;
};

#endif