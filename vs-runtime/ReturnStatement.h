#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include <vector>
#include <iostream>
#include "string"
#include "Expression.h"
#include "BaseValue.h"
#include "LexicalTokenType.h"
#include "NanType.h"
#include "Token.h"
#include "NumberType.h"
#include "BoolType.h"
#include "EcmaCompletionRecord.h"

using namespace std;

class ReturnStatement : public Expression
{
public:
	ReturnStatement(Expression* exp);
	ReturnStatement();
	BaseValue* eval() override;
	EcmaCompletionRecord* GetResult();
protected:
	EcmaCompletionRecord* result_;
	Expression* exp;
};

#endif