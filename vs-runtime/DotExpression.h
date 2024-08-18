#ifndef DOT_EXPRESSION_H
#define DOT_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "Token.h"
#include "NumberType.h"
#include "StringType.h"
#include "LexicalTokenType.h"
#include "NanType.h"
#include "ObjectExpression.h"
#include "IdentifierCollection.h"
#include "NewOperator.h"
#include "CallExpression.h"
#include "Errors.h"

using namespace std;

class DotExpression : public Expression
{
public:
	DotExpression(Expression* exp, string property_name);
	DotExpression(Expression* exp, HiddenProperties property_name);
	BaseValue* eval() override;
	void set_expression_to_write(Expression* exp);
	const char* plain_name = "DotExpression";
private:
	Expression* exp;
	string property_name;
	HiddenProperties hidden_property_name;
	Expression* expression_to_write;
	bool read;
	bool is_hidden_property;
	BaseValue* primitive_to_object_instance(ValueTypes type);
};

#endif