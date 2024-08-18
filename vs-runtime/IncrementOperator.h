#ifndef INCREMENT_OPERATOR_H
#define INCREMENT_OPERATOR_H

#include <vector>
#include "string"
#include "Expression.h"
#include "LexicalTokenType.h"
#include "BlockStatement.h"
#include "Errors.h"

using namespace std;

class IncrementOperator : public Expression
{
public:
	IncrementOperator(bool is_prefix_operation, LexicalTokenType operation_type, Expression* exp1, string var_name, EcmaEnvironment* block);
	BaseValue* eval() override;
private:
	bool is_prefix_operation;
	LexicalTokenType operation_type;
	Expression* exp;
	string var_name;
	EcmaEnvironment* block;
};

#endif