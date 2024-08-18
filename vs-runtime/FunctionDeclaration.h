#ifndef FUNCTION_DECLARATION_H
#define FUNCTION_DECLARATION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "Node.h"
#include "BlockStatement.h"
#include "AnonumousFuncExpression.h"
#include "EcmaEnvironment.h"

using namespace std;

class FunctionDeclaration : public Expression
{
public:
	FunctionDeclaration(string name, EcmaEnvironment* block, bool is_const, Parser* parser);
	BaseValue* eval() override;
	AnonumousFuncExpression* function;
	const char* plain_name = "FunctionDeclaration";
private:
	bool is_const;
	string name;
	EcmaEnvironment* block;
};

#endif