#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "string"
#include "Token.h"
#include "Node.h"
#include "Expression.h"
#include "BlockStatement.h"
#include "FunctionBodyStatement.h"

using namespace std;

class Parser
{
public:
	Parser(vector<Token>* lexems);
	vector<Token>* lexems;
	Token* get(int position);
	bool match(LexicalTokenType token_type);
	bool consume(LexicalTokenType token_type);
	int pos;
	Node* parse();
	Expression* additive();
	Expression* logical();
	Expression* andLogical();
	Expression* orLogical();
	Expression* complexity();
	Expression* get_property_from_object(Expression* current);
	Expression* function_expression();
	Node* evaluate_statement();
	Node* kus_statement();
	Node* variable_statement();
	Node* if_else_statement();
	Node* for_statement();
	Node* while_statement();
	Node* block_statement();
	FunctionBodyStatement* func_body_statement(int token_position, EcmaEnvironment* block);
	Expression* evaluate_expression();
	Expression* object_expression();
	Expression* unary();
	Expression* primary();
	Expression* typeof_expression();
	Expression* method_expression(Expression* exp, Expression* obj_exp);
	Expression* increment_expression();
	Expression* new_expression();

	static void set_current_block(EcmaEnvironment* block);
private:
	static EcmaEnvironment* current_block;
};

#endif
