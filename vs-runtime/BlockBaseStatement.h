#ifndef BASE_BLOCK_STATEMENT_H
#define BASE_BLOCK_STATEMENT_H

#include <vector>
#include "string"
#include "Statement.h"
#include "Expression.h"
#include "IdentifierCollection.h"
#include "EcmaEnvironment.h"

using namespace std;

class BlockBaseStatement : public Statement
{
public:
	BlockBaseStatement(EcmaEnvironment* statement);
	void add(Node* statement);
	EcmaEnvironment* env;
	BaseValue* eval() override;
private:
	vector<Node*> statementList;
};

#endif