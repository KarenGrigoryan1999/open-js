#ifndef ANONUMOUS_FUNC_EXPRESSION_H
#define ANONUMOUS_FUNC_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "Node.h"
#include "EcmaEnvironment.h"
#include "FunctionBodyStatement.h"
#include "ObjectExpression.h"
#include "Parser.h"
#include "Token.h"
#include "Functions.h"
#include "EcmaPropertyAttributes.h"

using namespace std;

class AnonumousFuncExpression : public ObjectExpression
{
public:
	AnonumousFuncExpression(EcmaEnvironment* block);
	AnonumousFuncExpression(EcmaEnvironment* block, Parser* parser);
	void SetBodyTokenPosition(int token_position);
	void add(string arg);
	void setBody(FunctionBodyStatement* body);
	FunctionBodyStatement* getBody();
	BaseValue* eval() override;
	vector<string>* get_arg_list();
	EcmaEnvironment* getBlock();
	void setBlock(EcmaEnvironment* env);
private:
	void SetupPrototype();
	int token_position;
	bool is_struct;
	Parser* parser;
	string name;
	FunctionBodyStatement* body;
	vector<string> args;
	EcmaEnvironment* block;
};

#endif
