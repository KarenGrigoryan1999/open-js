#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

#include <vector>
#include "string"
#include "Statement.h"
#include "Expression.h"

using namespace std;

class ForStatement : public Statement
{
public:
	ForStatement(Node* from, Node* to, Node* action, Node* iterated_action);
	BaseValue* eval() override;
	const char* plain_name = "ForStatement";
private:
	Node* from;
	Node* to;
	Node* action;
	Node* iterated_action;
};

#endif