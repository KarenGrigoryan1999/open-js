#include "WhileStatement.h"
#include "Token.h"
#include "NumberType.h"
#include "StringType.h"

using namespace std;

WhileStatement::WhileStatement(Node* condition, Node* iterated_action) {
	this->condition = condition;
	this->iterated_action = iterated_action;
}

BaseValue* WhileStatement::eval() {
	while (this->condition->eval()->GetAsBoolean()) this->iterated_action->eval();

	return 0;
}