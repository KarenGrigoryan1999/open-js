#include "NumberExpression.h"
#include "string"
#include "NumberType.h"
#include "Token.h"

using namespace std;

NumberExpression::NumberExpression(string value) {
	this->value = value;
}

BaseValue* NumberExpression::eval() {
	return new BaseValue(this->value, ValueTypes::NUMBER_TYPE);
}