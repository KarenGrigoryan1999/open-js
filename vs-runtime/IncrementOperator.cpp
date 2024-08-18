#include "IncrementOperator.h"
#include "Token.h"
#include "NumberType.h"
#include "StringType.h"
#include "LexicalTokenType.h"
#include "NanType.h"
#include "DotExpression.h"
#include "EmptyExpression.h"

using namespace std;

IncrementOperator::IncrementOperator(bool is_prefix_operation, LexicalTokenType operation_type, Expression* exp, string var_name, EcmaEnvironment* block) {
	this->is_prefix_operation = is_prefix_operation;
	this->operation_type = operation_type;
	this->exp = exp;
	this->var_name = var_name;
	this->block = block;
}

BaseValue* IncrementOperator::eval() {
	//TODO реализация не соответствует спецификации! Метод ToNumeric для спецификации не реализован
	BaseValue* operand = exp->eval();
	BaseValue* res;

	if (operand->GetType() != ValueTypes::NUMBER_TYPE)
		res = new NanType();
	else
		res = new NumberType(this->operation_type == LexicalTokenType::INCREMENT ? operand->GetAsNumber() + 1 : operand->GetAsNumber() - 1);

	try {
		EcmaEnvironment* current = this->block;
		if (current == NULL) {
			BaseValue* elem = IdentifierCollection::GetGlobal(var_name);
			if (elem->GetType() == ValueTypes::FUNCTION_TYPE || elem->GetType() == ValueTypes::OBJECT_TYPE) {
				((DotExpression*)this->exp)->set_expression_to_write(new EmptyExpression(res));
				this->exp->eval();
			}
			else
				IdentifierCollection::SetGlobal(var_name, res, false);
		}
		else {
			while (current != NULL) {
				try {
					current->records->GetLocal(var_name);
					break;
				}
				catch (BaseException* error_message) {
					current = current->outer;
				}
			}
			if (current == NULL) {
				try {
					IdentifierCollection::GetGlobal(var_name);
					IdentifierCollection::SetGlobal(var_name, res, false);
					return this->is_prefix_operation ? res : operand;
				}
				catch (BaseException* error_message) {
					block->records->SetLocal(var_name, res, false);
					return this->is_prefix_operation ? res : operand;
				}
			}
			BaseValue* elem = current->records->GetLocal(var_name);
			if (elem->GetType() == ValueTypes::FUNCTION_TYPE || elem->GetType() == ValueTypes::OBJECT_TYPE) {
				((DotExpression*)this->exp)->set_expression_to_write(new EmptyExpression(res));
				this->exp->eval();
			}
			else
				current->records->SetLocal(var_name, res, false);
		}
	}
	catch (BaseException* err) {
		throw Errors::ThrowError(ExceptionTypes::SyntaxError, string("Invalid left-hand side expression in ") + (!this->is_prefix_operation ? "postfix" : "prefix") + " operation");
	}

	return this->is_prefix_operation ? res : operand;
}

//https://262.ecma-international.org/#sec-prefix-increment-operator
//https://262.ecma-international.org/#sec-postfix-increment-operator