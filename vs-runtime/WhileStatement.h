#ifndef WHILE_STATEMENT_H
#define WHILE_STATEMENT_H

#include <vector>
#include "string"
#include "Statement.h"
#include "Expression.h"

using namespace std;

class WhileStatement : public Statement
{
public:
	WhileStatement(Node* condition, Node* iterated_action);
	BaseValue* eval() override;
	const char* plain_name = "WhileStatement";
private:
	Node* condition;
	Node* iterated_action;
};

#endif