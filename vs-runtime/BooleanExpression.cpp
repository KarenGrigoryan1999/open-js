#include "BooleanExpression.h"
#include "string"
#include "Token.h"

using namespace std;

BooleanExpression::BooleanExpression(string value) {
	this->value = value;
}

BaseValue* BooleanExpression::eval() {
	if(this->value == "true") return new BoolType(true);
	return new BoolType(false);
}