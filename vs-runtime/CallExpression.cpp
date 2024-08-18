#include "string"
#include <iostream>
#include "CallExpression.h"
#include "Token.h"
#include "Functions.h"
#include "FunctionDeclaration.h"
#include "ReturnStatement.h"
#include "UndefinedType.h"
#include "EcmaEnvironment.h"
#include "EcmaContextStack.h"
#include "EcmaExecutionContext.h"
#include "ReferenceError.h"

using namespace std;

CallExpression::CallExpression(string name, EcmaEnvironment* block) {
	this->name_ = name;
	this->block_ = block;
	this->func_exp_ = NULL;
	this->evaluated_function = NULL;
}

CallExpression::CallExpression(Expression* exp, EcmaEnvironment* block) {
	this->func_exp_ = exp;
	this->block_ = block;
	this->obj_expression_ = NULL;
	this->evaluated_function = NULL;
}

void CallExpression::add(Expression* exp) {
	this->args_.push_back(exp);
}

void CallExpression::SetupThisObject(Expression* obj_expression) {
	this->obj_expression_ = obj_expression;
}

BaseValue* CallExpression::eval() {
	AnonumousFuncExpression* function_to_eval;

	// Если нам в конструкторе пришло имя функции, а не экспрешн, то пытаемся найти ее в окружении
	if (!this->func_exp_) {
		BaseValue* func = NULL;
		if (this->block_ != NULL) {
			EcmaEnvironment* current = this->block_;

			while (current != NULL) {
				if (current->records == NULL) {
					current = current->outer;
					continue;
				}
				try {
					func = current->records->GetLocal(this->name_);
					break;
				}
				catch (BaseException* error_message) {
					current = current->outer;
				}
			}
		}

		//Если не нашли локально, то пытаемся найти в глобальной области
		try {
			if (func == NULL) func = IdentifierCollection::GetGlobal(this->name_);
		}
		catch (BaseException* msg) {
			//Если нет в глобальной, то берем built-in функцию
			return Functions::Get(this->name_, &this->args_);
		}

		function_to_eval = (AnonumousFuncExpression*)func->GetReference();
	}
	else {
		function_to_eval = (AnonumousFuncExpression*)func_exp_->eval()->GetReference();
	}

	this->evaluated_function = function_to_eval;
	
	//Формируем EnvironmentRecord, который закинем в ExecutionContext функции
	EcmaEnvironment func_env = { new IdentifierCollection(), NULL };

	//Создаем ExecutionContext для нашей функции
	EcmaExecutionContext* exec_context = new EcmaExecutionContext();

	//Биндим новый EnvironmentRecord
	exec_context->SetEnvironmentRecord(&func_env);

	//Если функция вызвана из объекта, то устанавливаем его как this
	if(this->obj_expression_)
		exec_context->SetThis(this->obj_expression_->eval());
	else
		exec_context->SetThis(new UndefinedType());

	//Сетим аргументы в новый EnvironmentRecord
	 vector<string>* arg_list = function_to_eval->get_arg_list();

	int i;
	for (i = 0; i < args_.size(); i++) {
			func_env.records->SetLocal(arg_list->at(i), this->args_.at(i)->eval(), false);
	}
	for (i; i < arg_list->size(); i++) {
		func_env.records->SetLocal(arg_list->at(i), new UndefinedType(), false);
	}

	//Кладем новый ExecutionContext в стек
	EcmaContextStack::Instance()->push(exec_context);

	if (!function_to_eval->getBody()) {
		function_to_eval = (AnonumousFuncExpression*)function_to_eval->eval()->GetReference();
	}

	//Обновляем EnvironmentRecord для тела функции чтобы она юзала EnvironmentRecord из ExecutionContext
	function_to_eval->getBody()->refresh_env();

	BaseValue* ret = new UndefinedType();
	try {
		function_to_eval->getBody()->eval();
	}
	catch (ReturnStatement* rt) {
		ret = rt->GetResult()->value;
		if (ret->GetType() == ValueTypes::FUNCTION_TYPE) {
			AnonumousFuncExpression* returned_function = (AnonumousFuncExpression*)ret->GetReference();
			FunctionBodyStatement* ret_function_body = returned_function->getBody();
			FunctionBodyStatement* function_to_eval_body = function_to_eval->getBody();

			ret_function_body->env->outer = new EcmaEnvironment();
			ret_function_body->env->outer->outer = function_to_eval_body->env->outer;
			ret_function_body->env->outer->records = function_to_eval_body->env->records;
		}
	}

	//Убираем ExecutionContext из стека после исполнения функции
	EcmaContextStack::Instance()->pop();

	function_to_eval->getBody()->refresh_env();

	return ret;
}
