#include "BlockBaseStatement.h"
#include "iostream"
#include "BlockStatement.h"
#include "Parser.h"

using namespace std;

BlockBaseStatement::BlockBaseStatement(EcmaEnvironment* statement) {
	this->env = new EcmaEnvironment();
	this->env->records = new IdentifierCollection();
	this->env->outer = statement;
}

void BlockBaseStatement::add(Node* statement) {
	this->statementList.push_back(statement);
}

BaseValue* BlockBaseStatement::eval() {
	for (int i = 0; i < this->statementList.size(); i++) {
		this->statementList[i]->eval();
	}

	return 0;
}