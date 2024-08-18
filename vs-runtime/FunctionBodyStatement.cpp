#include "FunctionBodyStatement.h"
#include "iostream"
#include "BlockStatement.h"
#include "Parser.h"
#include "EcmaContextStack.h"
#include "IdentifierCollection.h"

using namespace std;

void FunctionBodyStatement::add(Node* statement) {
	this->statements.push_back(statement);
}

vector<Node*>* FunctionBodyStatement::GetStatementList() {
	return &this->statements;
}

void FunctionBodyStatement::refresh_env() {
	if (EcmaContextStack::Instance()->size() != 0) {
		this->env->records = EcmaContextStack::Instance()->top()->GetEnvironmentRecord()->records;
	}
	else {
		this->env->records = NULL;
	}
}

BaseValue* FunctionBodyStatement::eval() {
	for (int i = 0; i < this->statements.size(); i++) {
		this->statements.at(i)->eval();
	}

	this->env->records = new IdentifierCollection();

	return 0;
}