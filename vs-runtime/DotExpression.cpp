#include "DotExpression.h"

using namespace std;

DotExpression::DotExpression(Expression* exp, string property_name) {
	this->exp = exp;
	this->property_name = property_name;
	this->read = true;
	this->is_hidden_property = false;
}

DotExpression::DotExpression(Expression* exp, HiddenProperties property_name) {
	this->exp = exp;
	this->read = true;
	this->is_hidden_property = true;
	this->hidden_property_name = property_name;
	this->expression_to_write = NULL;
}

void DotExpression::set_expression_to_write(Expression* exp) {
	this->read = false;
	this->is_hidden_property = false;
	this->expression_to_write = exp;
}

BaseValue* DotExpression::eval() {
	BaseValue* obj = this->exp->eval();
	ValueTypes var_type = obj->GetType();
	//Если перед точкой undefined или null, то боксинг работать не должен и мы можем сразу выкинуть исключение
	if (var_type == ValueTypes::UNDEFINED_TYPE || var_type == ValueTypes::NULL_TYPE) {
		throw Errors::ThrowError(ExceptionTypes::TypeError, string("Cannot read properties of ") + (var_type == ValueTypes::UNDEFINED_TYPE ? "undefined" : "null") + " (reading '" + property_name + "')");
	}
	// Если перед точной примитивный тип, то делаем боксинг, чтобы работать со свойствами
	if (var_type == ValueTypes::STRING_TYPE) {
		obj = this->primitive_to_object_instance(ValueTypes::STRING_TYPE);
	}

	if (this->read) {
		if(this->is_hidden_property)
			return ((ObjectExpression*)obj->GetReference())->get_hidden_property(this->hidden_property_name);
		else
			return ((ObjectExpression*)obj->GetReference())->GetPropertyValue(property_name);
	}
	else {
		if (this->is_hidden_property)
			return ((ObjectExpression*)obj->GetReference())->SetHidenPropertyValue(HiddenProperties::PrimitiveValue, this->expression_to_write);
		else
		return ((ObjectExpression*)obj->GetReference())->SetPropertyValue(property_name, this->expression_to_write);
	}
}

BaseValue* DotExpression::primitive_to_object_instance(ValueTypes type) {
	BaseValue* primitive_prototype = NULL;
	if (type == ValueTypes::STRING_TYPE) {
		primitive_prototype = IdentifierCollection::GetGlobal("String");
	}
	
	ObjectExpression* string_object = new ObjectExpression(false);//Создаем новый объект
	BaseValue* evaluated_obj = string_object->eval();

	ObjectExpression* object_reference = evaluated_obj->GetReference();

	object_reference->SetPropertyValue("__proto__", primitive_prototype->GetReference()->GetPropertyValue("prototype"));
	object_reference->SetHidenPropertyValue(HiddenProperties::PrimitiveValue, this->exp);
	
	return evaluated_obj;
}