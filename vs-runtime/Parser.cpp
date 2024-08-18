#include "Parser.h"
#include "LexicalTokenType.h"
#include "NumberExpression.h"
#include "BinaryOperator.h"
#include "LogicalOperator.h"
#include "UnaryOperator.h"
#include "IdentifierReference.h"
#include "NanExpression.h"
#include "UndefinedExpression.h"
#include "VariableStatement.h"
#include "ForStatement.h"
#include "WhileStatement.h"
#include "IfStatement.h"
#include "TypeofOperator.h"
#include "StringExpression.h"
#include "BooleanExpression.h"
#include "BlockStatement.h"
#include "IncrementOperator.h"
#include "CallExpression.h"
#include "FunctionDeclaration.h"
#include "ReturnStatement.h"
#include "EcmaEnvironment.h"
#include "BlockStatement.h"
#include "FunctionBodyStatement.h"
#include "ObjectExpression.h"
#include "DotExpression.h"
#include "NumberType.h"
#include "ThisExpression.h"
#include "NewOperator.h"
#include "NullExpression.h"

using namespace std;

EcmaEnvironment* Parser::current_block = NULL;

Parser::Parser(vector<Token>* lexems) {
	this->lexems = lexems;
	Token token(LexicalTokenType::CODE_EOF, "");
	this->lexems->push_back(token);
	this->pos = 0;
}

Node* Parser::parse() {
	BlockStatement* statement = new BlockStatement(Parser::current_block);

	while (!this->match(LexicalTokenType::CODE_EOF)) {
		statement->add(this->evaluate_statement());
	}

	return statement;
}

Node* Parser::block_statement() {
	if (this->match(LexicalTokenType::L_BLOCK) && this->get(1)->type != LexicalTokenType::COLON) {
		BlockStatement* statement = new BlockStatement(Parser::current_block);
		Parser::current_block = statement->env;

		while (!this->match(LexicalTokenType::R_BLOCK)) {
			statement->add(this->block_statement());
		}

		Parser::set_current_block(Parser::current_block->outer);

		return statement;
	}

	return this->kus_statement();
}

FunctionBodyStatement* Parser::func_body_statement(int token_position, EcmaEnvironment* block) {
	this->pos = token_position;
	if (this->match(LexicalTokenType::L_BLOCK) && this->get(1)->type != LexicalTokenType::COLON) {
		FunctionBodyStatement* statement = new FunctionBodyStatement(block);
		Parser::current_block = statement->env;

		while (!this->match(LexicalTokenType::R_BLOCK)) {
			statement->add(this->block_statement());
		}

		Parser::set_current_block(Parser::current_block->outer);

		return statement;
	}

	throw "error";
}

Node* Parser::evaluate_statement() {
	return this->block_statement();
}

Node* Parser::kus_statement() {
	if (this->match(LexicalTokenType::IF_STATEMENT))
		return this->if_else_statement();
	if (this->match(LexicalTokenType::RETURN))
		return new ReturnStatement(this->evaluate_expression());
	else if (this->match(LexicalTokenType::FOR_STATEMENT))
		return this->for_statement();
	else if (this->match(LexicalTokenType::WHILE_STATEMENT))
		return this->while_statement();
	else this->variable_statement();
}

Node* Parser::variable_statement() {
	bool is_const = false;
	bool is_declaration = false;
	if (this->get(0)->type == LexicalTokenType::ONE_WORD && (this->get(0)->value == "const")) {
		is_const = true;
		is_declaration = true;
		this->match(LexicalTokenType::ONE_WORD);
	}
	if (this->get(0)->value == "let") {
		is_declaration = true;
		this->match(LexicalTokenType::ONE_WORD);
	}
	if (this->get(0)->type == LexicalTokenType::FUNCTION && this->get(1)->type == LexicalTokenType::ONE_WORD) {
		string var_name = this->get(1)->value;
		this->match(LexicalTokenType::FUNCTION);
		this->match(LexicalTokenType::ONE_WORD);
		FunctionDeclaration* func = new FunctionDeclaration(var_name, Parser::current_block, is_const, this);

		int args_length = 0;

		this->match(LexicalTokenType::L_BRACKET);

		while (!this->match(LexicalTokenType::R_BRACKET))
		{
			func->function->add(this->get(0)->value);
			this->consume(LexicalTokenType::ONE_WORD);
			this->match(LexicalTokenType::COMMA);
			args_length += 1;
		}

		func->function->add_property("length", {
			new NumberType(args_length),
			PropertyDescriptorState::TURNED_ON,
			PropertyDescriptorState::TURNED_ON,
			PropertyDescriptorState::TURNED_ON
		});
		func->function->SetBodyTokenPosition(this->pos);

		int i = 0;
		do {
			if (this->match(LexicalTokenType::L_BLOCK)) {
				i++;
			}
			else if (this->match(LexicalTokenType::R_BLOCK)) {
				i--;
			}
			else {
				this->pos += 1;
			}
		} while (i != 0);

		return func;
	}
	else if (this->get(1)->type == LexicalTokenType::EQUAL) {
		if (this->get(0)->type != LexicalTokenType::ONE_WORD)
			throw Errors::ThrowError(ExceptionTypes::SyntaxError, "left-hand side in assignment");
		string var_name = this->get(0)->value;
		this->match(LexicalTokenType::ONE_WORD);
		this->match(LexicalTokenType::EQUAL);

		return new VariableStatement(var_name, Parser::current_block, (Expression*)this->variable_statement(), is_const, is_declaration);
	}

	return this->evaluate_expression();
}

Node* Parser::if_else_statement() {
	Expression* condition = this->evaluate_expression();
	Node* if_statement = this->evaluate_statement();
	Node* else_statement = NULL;
	if (this->match(LexicalTokenType::ELSE_STATEMENT)) {
		else_statement = this->evaluate_statement();
	}

	return new IfStatement(condition, if_statement, else_statement);
}

Node* Parser::for_statement() {
	this->match(LexicalTokenType::L_BRACKET);
	Node* from = this->evaluate_statement();
	this->match(LexicalTokenType::SEMICOLON);
	Node* to = this->evaluate_statement();
	this->match(LexicalTokenType::SEMICOLON);
	Node* action = this->evaluate_statement();
	this->match(LexicalTokenType::R_BRACKET);
	Node* iterated_act = this->evaluate_statement();

	return new ForStatement(from, to, action, iterated_act);
}

Node* Parser::while_statement() {
	this->match(LexicalTokenType::L_BRACKET);
	Node* condition = this->evaluate_statement();
	this->match(LexicalTokenType::R_BRACKET);
	Node* iterated_act = this->evaluate_statement();

	return new WhileStatement(condition, iterated_act);
}

Expression* Parser::evaluate_expression() {
	return this->object_expression();
}

Expression* Parser::typeof_expression() {
	if (this->match(LexicalTokenType::TYPEOF)) {
		Expression* result = this->unary();
		return new TypeofOperator(result);
	} else
	return this->unary();
}

Expression* Parser::object_expression() {
	if (this->match(LexicalTokenType::L_BLOCK)) {
		ObjectExpression* object = new ObjectExpression(false);
		while (!this->match(LexicalTokenType::R_BLOCK)) {
			string key = this->get(0)->value;
			this->match(LexicalTokenType::ONE_WORD);
			this->match(LexicalTokenType::COLON);
			object->add_property(key, this->evaluate_expression());
			this->match(LexicalTokenType::COMMA);
		}

		return object;
	}

	return this->orLogical();
}

Expression* Parser::orLogical() {
	Expression* first = this->andLogical();
	while (true) {
		if (this->match(LexicalTokenType::LOGICAL_OR_EXPRESSION)) {
			first = new LogicalOperator(LexicalTokenType::LOGICAL_OR_EXPRESSION, first, this->andLogical());
			continue;
		}
		break;
	}

	return first;
}

Expression* Parser::andLogical() {
	Expression* first = this->logical();
	while (true) {
		if (this->match(LexicalTokenType::LOGICAL_AND_EXPRESSION)) {
			first = new LogicalOperator(LexicalTokenType::LOGICAL_AND_EXPRESSION, first, this->logical());
			continue;
		}
		break;
	}

	return first;
}

Expression* Parser::logical() {
	Expression* first = this->additive();

	while (true) {
		if (this->match(LexicalTokenType::NOT_EQ_CHECKING)) {
			first = new LogicalOperator(LexicalTokenType::NOT_EQ_CHECKING, first, this->additive());
			continue;
		}
		if (this->match(LexicalTokenType::NOT_EQ_STRICT_CHECKING)) {
			first = new LogicalOperator(LexicalTokenType::NOT_EQ_STRICT_CHECKING, first, this->additive());
			continue;
		}
		if (this->match(LexicalTokenType::EQ_CHECKING)) {
			first = new LogicalOperator(LexicalTokenType::EQ_CHECKING, first, this->additive());
			continue;
		}
		if (this->match(LexicalTokenType::EQ_STRICT_CHECKING)) {
			first = new LogicalOperator(LexicalTokenType::EQ_STRICT_CHECKING, first, this->additive());
			continue;
		}
		if (this->match(LexicalTokenType::LT)) {
			first = new LogicalOperator(LexicalTokenType::LT, first, this->additive());
			continue;
		}
		if (this->match(LexicalTokenType::GT)) {
			first = new LogicalOperator(LexicalTokenType::GT, first, this->additive());
			continue;
		}
		if (this->match(LexicalTokenType::GTE)) {
			first = new LogicalOperator(LexicalTokenType::GTE, first, this->additive());
			continue;
		}
		if (this->match(LexicalTokenType::LTE)) {
			first = new LogicalOperator(LexicalTokenType::LTE, first, this->additive());
			continue;
		}
		break;
	}

	return first;
}

Expression* Parser::additive() {
	Expression* first = this->complexity();

	while (true) {
		if (this->match(LexicalTokenType::PLUS)) {
			first = new BinaryOperator(LexicalTokenType::PLUS, first, this->complexity());
			continue;
		}
		if (this->match(LexicalTokenType::MINUS)) {
			first = new BinaryOperator(LexicalTokenType::MINUS, first, this->complexity());
			continue;
		}
		break;
	}

	return first;
}

Expression* Parser::complexity() {
	Expression* first = this->new_expression();

	while (true) {
		if (this->match(LexicalTokenType::STAR)) {
			first = new BinaryOperator(LexicalTokenType::STAR, first, this->new_expression());
			continue;
		}
		if (this->match(LexicalTokenType::DIV)) {
			first = new BinaryOperator(LexicalTokenType::DIV, first, this->new_expression());
			continue;
		}
		break;
	}

	return first;
}

Expression* Parser::new_expression() {
	if (this->match(LexicalTokenType::NEW_OPERATOR)) {
		return new NewOperator(this->increment_expression());
	}

	return this->increment_expression();
}

Expression* Parser::get_property_from_object(Expression* current) {
	Expression* prev_exp = NULL;
	while (true) {
		if (this->match(LexicalTokenType::DOT_OP)) {
			prev_exp = current;
			current = new DotExpression(current, this->get(0)->value);
			this->match(LexicalTokenType::ONE_WORD);
			continue;
		}
		break;
	}
	if (this->match(LexicalTokenType::L_BRACKET)) {
		return this->method_expression(current, prev_exp);
	}
	if (this->match(LexicalTokenType::EQUAL)) {
		//Если обнаруживаем знак равенства, то выполняем запись в property объекта
		((DotExpression*)current)->set_expression_to_write(this->evaluate_expression());
	}

	return current;
}

Expression* Parser::function_expression() {
	CallExpression* func = new CallExpression(this->get(0)->value, Parser::current_block);
	this->match(LexicalTokenType::ONE_WORD);
	this->match(LexicalTokenType::L_BRACKET);

	while (!this->match(LexicalTokenType::R_BRACKET)) {
		this->match(LexicalTokenType::COMMA);
		func->add(this->evaluate_expression());
	}

	return func;
}

Expression* Parser::method_expression(Expression* exp, Expression* obj_exp) {
	CallExpression* func = new CallExpression(exp, Parser::current_block);
	func->SetupThisObject(obj_exp);

	while (!this->match(LexicalTokenType::R_BRACKET)) {
		this->match(LexicalTokenType::COMMA);
		func->add(this->evaluate_expression());
	}

	return func;
}

Expression* Parser::increment_expression() {
	string token_type = this->get(0)->value;
	Expression* result = this->typeof_expression();
	if (this->match(LexicalTokenType::INCREMENT)) {
		return new IncrementOperator(false, LexicalTokenType::INCREMENT, result, token_type, Parser::current_block);
	}
	if (this->match(LexicalTokenType::DECREMENT)) {
		return new IncrementOperator(false , LexicalTokenType::DECREMENT, result, token_type, Parser::current_block);
	}

	return result;
}

Expression* Parser::unary() {
	if (this->match(LexicalTokenType::MINUS)) {
		return new UnaryOperator(LexicalTokenType::MINUS, this->primary());
	}
	if (this->match(LexicalTokenType::PLUS)) {
		return new UnaryOperator(LexicalTokenType::PLUS, this->primary());
	}
	if (this->match(LexicalTokenType::INVERSE)) {
		return new UnaryOperator(LexicalTokenType::INVERSE, this->primary());
	}

	return this->primary();
}

Expression* Parser::primary() {
	Token* current = this->get(0);
	if (this->match(LexicalTokenType::NUMBER)) {
		NumberExpression* exp = new NumberExpression(current->value);
		if (this->get(0)->type != LexicalTokenType::DOT_OP) return exp;
		else return this->get_property_from_object(exp);
	}
	if (current->type == LexicalTokenType::FUNCTION && this->get(1)->type == LexicalTokenType::L_BRACKET) {
		this->match(LexicalTokenType::FUNCTION);
		this->match(LexicalTokenType::L_BRACKET);

		AnonumousFuncExpression* func = new AnonumousFuncExpression(Parser::current_block, this);
		
		int args_length = 0;

		while (!this->match(LexicalTokenType::R_BRACKET))
		{
			func->add(this->get(0)->value);
			args_length += 1;
			this->consume(LexicalTokenType::ONE_WORD);
			this->match(LexicalTokenType::COMMA);
		}

		func->add_property("length", {
			new NumberType(args_length),
			PropertyDescriptorState::TURNED_ON,
			PropertyDescriptorState::TURNED_ON,
			PropertyDescriptorState::TURNED_ON
		});

		func->SetBodyTokenPosition(this->pos);
		
		int i = 0;
		do {
			if (this->match(LexicalTokenType::L_BLOCK)) {
				i++;
			}
			else if (this->match(LexicalTokenType::R_BLOCK)) {
				i--;
			}
			else {
				this->pos += 1;
			}
		} while (i != 0);

		return func;
	}
	if (this->match(LexicalTokenType::NAN_EXPRESSION)) {
		if (this->get(0)->type != LexicalTokenType::DOT_OP) return new NanExpression();
		else return this->get_property_from_object(new NanExpression());
	}
	if (this->match(LexicalTokenType::THIS_KEYWORD)) {
		Expression* exp = new ThisExpression();
		if (this->get(0)->type != LexicalTokenType::DOT_OP)
			return exp;
		else
			return this->get_property_from_object(exp);
	}
	if (this->match(LexicalTokenType::UNDEFINED_EXPRESSION)) {
		if (this->get(0)->type != LexicalTokenType::DOT_OP) return new UndefinedExpression();
		else return this->get_property_from_object(new UndefinedExpression());
	}
	if (this->match(LexicalTokenType::NULL_EXPRESSION)) {
		if (this->get(0)->type != LexicalTokenType::DOT_OP) return new NullExpression();
		else return this->get_property_from_object(new NullExpression());
	}
	if (this->match(LexicalTokenType::PLAIN_TEXT)) {
		StringExpression* exp = new StringExpression(current->value);
		if (this->get(0)->type != LexicalTokenType::DOT_OP) return exp;
		else return this->get_property_from_object(exp);
	}
	if (current->type == LexicalTokenType::ONE_WORD && this->get(1)->type == LexicalTokenType::L_BRACKET) {
		return this->function_expression();
	}
	if (this->match(LexicalTokenType::ONE_WORD)) {
		IdentifierReference* exp = new IdentifierReference(current->value, Parser::current_block);
		if (this->get(0)->type != LexicalTokenType::DOT_OP) {//Если после слова не стоит точка, то делаем вывод что мы имем дело с переменной
			return exp;
		}
		else {
			//Если после слова точка, то, вероятно мы достаем свойство из объекта
			return this->get_property_from_object(exp);
		}
	}
	if (this->match(LexicalTokenType::L_BRACKET)) {
		Expression* result = this->evaluate_expression();
		if (this->match(LexicalTokenType::R_BRACKET)) return result;
		else {
			throw string("Uncaught SyntaxError: missing ) after argument list");
		}
	}
	if (this->match(LexicalTokenType::BOOLEAN_ELEMENT)) {
		BooleanExpression* exp = new BooleanExpression(current->value);
		return exp;
	}
	if (this->match(LexicalTokenType::INCREMENT)) {
		return new IncrementOperator(true, LexicalTokenType::INCREMENT, this->unary(), this->get(0)->value, Parser::current_block);
	}
	if (this->match(LexicalTokenType::DECREMENT)) {
		return new IncrementOperator(true, LexicalTokenType::DECREMENT, this->unary(), this->get(0)->value, Parser::current_block);
	}
	if (this->match(LexicalTokenType::SIMPLE_COMMENT) || this->match(LexicalTokenType::MULTILINE_COMMENT) || this->match(LexicalTokenType::SEMICOLON)) {
		NumberExpression* exp = new NumberExpression("");//Временное решение чтобы пропустить все комментарии
		return exp;
	}

	throw string("Unknown token exception ") + current->value;
}

bool Parser::match(LexicalTokenType token_type) {
	Token* t = this->get(0);
	if (t->type != token_type) return false;
	this->pos += 1;
	return true;
}

bool Parser::consume(LexicalTokenType token_type) {
	Token* t = this->get(0);
	if (t->type != token_type) throw string("Uncaught token ") + t->value;
	this->pos += 1;
	return true;
}

Token* Parser::get(int position) {
	return &this->lexems->at(position+pos);
}

void Parser::set_current_block(EcmaEnvironment* block){
	Parser::current_block = block;
}