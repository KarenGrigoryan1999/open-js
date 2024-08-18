#include "EmptyExpression.h"

using namespace std;

EmptyExpression::EmptyExpression(BaseValue* value) {
	this->value = value;
}

BaseValue* EmptyExpression::eval() {
	return this->value;
}