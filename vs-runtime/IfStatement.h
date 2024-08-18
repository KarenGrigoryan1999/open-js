#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include <vector>
#include "string"
#include "Statement.h"
#include "Expression.h"
#include "Token.h"
#include "NumberType.h"
#include "StringType.h"
#include "UndefinedType.h"

using namespace std;

class IfStatement : public Statement
{
public:
	IfStatement(Expression* condition, Node* if_statement, Node* else_statement);
	BaseValue* eval() override;
private:
	Expression* condition_;
	Node* ifStatement_;
	Node* elseStatement_;
};

#endif