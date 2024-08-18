#ifndef FUNCTION_BODY_STATEMENT_H
#define FUNCTION_BODY_STATEMENT_H

#include <vector>
#include "string"
#include "BlockBaseStatement.h"
#include "Expression.h"
#include "IdentifierCollection.h"
#include "EcmaEnvironment.h"

using namespace std;

class FunctionBodyStatement : public BlockBaseStatement
{
public:
	FunctionBodyStatement(EcmaEnvironment* statement) : BlockBaseStatement(statement) {};
	vector<Node*>* GetStatementList();
	void add(Node* statement);
	BaseValue* eval() override;
	void refresh_env();
	const char* plain_name = "FunctionBodyStatement";
private:
	vector<Node*> statements;
};

#endif