#include "ForStatement.h"
#include "Token.h"
#include "NumberType.h"
#include "StringType.h"

using namespace std;

ForStatement::ForStatement(Node* from, Node* to, Node* action, Node* iterated_action) {
	this->from = from;
	this->to = to;
	this->action = action;
	this->iterated_action = iterated_action;
}

BaseValue* ForStatement::eval() {
	for (from->eval(); to->eval()->GetAsBoolean() == true; action->eval()) this->iterated_action->eval();

	return 0;
}