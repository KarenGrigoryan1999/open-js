#include "VariableStatement.h"
#include "Expression.h"
#include "IdentifierCollection.h"
#include "EcmaContextStack.h"
#include "TDZValue.h"
#include "ReferenceError.h"
#include "Errors.h"

VariableStatement::VariableStatement(string name, EcmaEnvironment* block, Expression* exp, bool is_const, bool is_declaration) {
	this->name = name;
	this->exp = exp;
	this->is_const = is_const;
	this->is_declaration = is_declaration;
	this->block = block;

	if (is_declaration) {
		//Если у нас происходит объявление переменной, то на стадии StaticSemantic для переменных LexicalStatement должна произойти проверка, на то, что переменная не была определена ранее в блоке
		bool was_declared = false;
		if (this->block != NULL) {
			try {
				BaseValue* val = this->block->records->GetLocal(name);
				was_declared = true;
			} catch(BaseException* e) {}
		}

		if (was_declared) {
			throw Errors::ThrowError(ExceptionTypes::SyntaxError, "Identifier " + name + string(" has already been declared"));
		}
	}
	if (this->block == NULL)
		IdentifierCollection::SetGlobal(name, new TDZValue(), is_const);
	else
	this->block->records->SetLocal(name, new TDZValue(), is_const);//TDZ - Temporary Dead Zone - пока переменная не определена, часть блока до определения этой переменной - это мертвая зона
}

BaseValue* VariableStatement::eval() {
	BaseValue* evaluated_data = this->exp->eval();
	if (block != NULL) {
		if(block->records != NULL) {
			try {
				block->records->GetLocal(this->name);
			}
			catch (BaseException* error_message) {
				block->records->SetLocal(this->name, evaluated_data, this->is_const);
				return evaluated_data;
			}
		}
	}
	else {
		IdentifierCollection::SetGlobal(this->name, evaluated_data, this->is_const);
		return evaluated_data;
	}
	return evaluated_data;
}