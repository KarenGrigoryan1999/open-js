#include "BinaryOperator.h"

BinaryOperator::BinaryOperator(LexicalTokenType op, Expression* exp1, Expression* exp2) {
	this->op = op;
	this->exp1 = exp1;
	this->exp2 = exp2;
}

BaseValue* BinaryOperator::CallConvertationMethod(BaseValue* method, BaseValue* obj) {
	if (method && method->GetReference()) {
		CallExpression* caller = new CallExpression(method->GetReference(), NULL);
		caller->SetupThisObject(obj->GetReference());
		BaseValue* result = caller->eval();
		ValueTypes resType = result->GetType();
		if (resType != ValueTypes::OBJECT_TYPE && resType != ValueTypes::FUNCTION_TYPE) return result;
	}

	return NULL;
}

BaseValue* BinaryOperator::ToPrimitive(BaseValue* obj, ValueTypes preferred_type) {
	BaseValue* value_of = obj->GetReference()->GetPropertyValue("valueOf");
	BaseValue* to_string = obj->GetReference()->GetPropertyValue("toString");

	BaseValue* result;
	if (preferred_type == ValueTypes::NUMBER_TYPE) {
		result = this->CallConvertationMethod(value_of, obj);
		if (!result) {
			BaseValue* result = this->CallConvertationMethod(to_string, obj);
		}
	}
	else {
		result = this->CallConvertationMethod(to_string, obj);
		if (!result) {
			BaseValue* result = this->CallConvertationMethod(value_of, obj);
		}
	}

	if (result) return result;
	throw Errors::ThrowError(ExceptionTypes::TypeError, "Cannot convert object to primitive value");
}

//TODO: В рантайме пока нет +Infinity и -Infinity! Данные кейсы для действия из спеки с бинарными операторами нужно будет доделать
BaseValue* BinaryOperator::eval() {
	BaseValue* operand1 = exp1->eval();
	BaseValue* operand2 = exp2->eval();

	//https://262.ecma-international.org/#sec-applystringornumericbinaryoperator ветка кода, отвечающая за оператор +
	if (this->op == LexicalTokenType::PLUS) {
		//Если имеем дело с объектом, то сначала пытаемся привести его к примитиву
		if (operand1->GetType() == ValueTypes::OBJECT_TYPE || operand1->GetType() == ValueTypes::FUNCTION_TYPE) {
			operand1 = BinaryOperator::ToPrimitive(operand1, ValueTypes::STRING_TYPE);
		}

		if (operand2->GetType() == ValueTypes::OBJECT_TYPE || operand2->GetType() == ValueTypes::FUNCTION_TYPE) {
			operand2 = BinaryOperator::ToPrimitive(operand2, ValueTypes::STRING_TYPE);
		}

		if (operand1->GetType() == ValueTypes::STRING_TYPE || operand2->GetType() == ValueTypes::STRING_TYPE) {
			return new StringType(operand1->GetAsString() + operand2->GetAsString());
		}

		double left = operand1->GetAsNumber();
		double right = operand2->GetAsNumber();

		if (isnan(left) || isnan(right)) return new NanType();
		return new NumberType(left + right);
	}

	//https://262.ecma-international.org/#sec-numeric-types-number-subtract ветка кода, отвечающая за оператор -
	if (this->op == LexicalTokenType::MINUS) {
		if (operand1->GetType() == ValueTypes::OBJECT_TYPE || operand1->GetType() == ValueTypes::FUNCTION_TYPE) {
			operand1 = BinaryOperator::ToPrimitive(operand1, ValueTypes::NUMBER_TYPE);
		}

		if (operand2->GetType() == ValueTypes::OBJECT_TYPE || operand2->GetType() == ValueTypes::FUNCTION_TYPE) {
			operand2 = BinaryOperator::ToPrimitive(operand2, ValueTypes::NUMBER_TYPE);
		}

		double left = operand1->GetAsNumber();
		double right = operand2->GetAsNumber();

		if (isnan(left) || isnan(right)) return new NanType();
		return new NumberType(left - right);
	}
	//https://262.ecma-international.org/#sec-numeric-types-number-multiply ветка кода, отвечающая за оператор *
	if (this->op == LexicalTokenType::STAR) {
		if (operand1->GetType() == ValueTypes::OBJECT_TYPE || operand1->GetType() == ValueTypes::FUNCTION_TYPE) {
			operand1 = BinaryOperator::ToPrimitive(operand1, ValueTypes::NUMBER_TYPE);
		}

		if (operand2->GetType() == ValueTypes::OBJECT_TYPE || operand2->GetType() == ValueTypes::FUNCTION_TYPE) {
			operand2 = BinaryOperator::ToPrimitive(operand2, ValueTypes::NUMBER_TYPE);
		}

		double left = operand1->GetAsNumber();
		double right = operand2->GetAsNumber();

		if (isnan(left) || isnan(right)) return new NanType();
		return new NumberType(left * right);
	}
	//https://262.ecma-international.org/#sec-numeric-types-number-divide ветка кода, отвечающая за оператор /
	if (this->op == LexicalTokenType::DIV) {
		if (operand1->GetType() == ValueTypes::OBJECT_TYPE || operand1->GetType() == ValueTypes::FUNCTION_TYPE) {
			operand1 = BinaryOperator::ToPrimitive(operand1, ValueTypes::NUMBER_TYPE);
		}

		if (operand2->GetType() == ValueTypes::OBJECT_TYPE || operand2->GetType() == ValueTypes::FUNCTION_TYPE) {
			operand2 = BinaryOperator::ToPrimitive(operand2, ValueTypes::NUMBER_TYPE);
		}

		double left = operand1->GetAsNumber();
		double right = operand2->GetAsNumber();

		if (isnan(left) || isnan(right)) return new NanType();
		return new NumberType(left / right);
	}
}

//https://262.ecma-international.org/#sec-applystringornumericbinaryoperator