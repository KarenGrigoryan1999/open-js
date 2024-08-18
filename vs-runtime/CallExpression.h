#ifndef CALL_EXPRESSION_H
#define CALL_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "BaseValue.h"
#include "BlockStatement.h"
#include "AnonumousFuncExpression.h"

using namespace std;

class CallExpression : public Expression
{
public:
	CallExpression(string name, EcmaEnvironment* block);
	CallExpression(Expression* exp, EcmaEnvironment* block);
	void SetupThisObject(Expression* obj_expression);
	BaseValue* eval() override;
	void add(Expression* exp);
	AnonumousFuncExpression* evaluated_function;
private:
	string name_;
	Expression* func_exp_;
	vector<Expression*> args_;
	EcmaEnvironment* block_;
	Expression* obj_expression_;
};

#endif