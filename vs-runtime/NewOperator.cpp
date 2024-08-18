#include "NewOperator.h"

NewOperator::NewOperator(Expression* new_expression) {
	this->new_expression_ = new_expression;
}

BaseValue* NewOperator::eval() {
	//Runtime Semantics
	ObjectExpression* obj = new ObjectExpression(false);//Создаем новый объект
	BaseValue* evaluated_obj = obj->eval();

	if (dynamic_cast<IdentifierReference*>(this->new_expression_)) {
		this->new_expression_ = this->new_expression_->eval()->GetReference();
	}

	CallExpression* constructor;
	if (dynamic_cast<CallExpression*>(this->new_expression_)) {
		constructor = (CallExpression*)this->new_expression_;//Берем конструктор
	}
	else if (dynamic_cast<AnonumousFuncExpression*>(this->new_expression_)) {
		constructor = new CallExpression(this->new_expression_, NULL);
	}
	else {
		throw Errors::ThrowError(ExceptionTypes::TypeError, "operand is not a constructor");
	}
	constructor->SetupThisObject(obj);//Биндим созданный объект как this
	constructor->eval();//Выполняем конструктор
	evaluated_obj->GetReference()->SetPropertyValue("__proto__", constructor->evaluated_function->GetPropertyValue("prototype"));

	return evaluated_obj;
}

//https://262.ecma-international.org/#sec-new-operator