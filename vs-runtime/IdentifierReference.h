#ifndef VARIABLE_EXPRESSION_H
#define VARIABLE_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "BaseValue.h"
#include "BlockStatement.h"
#include "ReferenceError.h"

using namespace std;

class IdentifierReference : public Expression
{
public:
	IdentifierReference(string name, EcmaEnvironment* block);
	BaseValue* eval() override;
	const char* plain_name = "IdentifierReference";
private:
	string name;
	EcmaEnvironment* block;
};

#endif