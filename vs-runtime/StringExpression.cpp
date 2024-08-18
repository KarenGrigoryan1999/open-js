#include "StringExpression.h"
#include "string"
#include "Token.h"
#include "StringType.h"

using namespace std;

StringExpression::StringExpression(string value) {
	this->value = value;
}

BaseValue* StringExpression::eval() {
	return new StringType(this->value);
}