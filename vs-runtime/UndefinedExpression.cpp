#include "UndefinedExpression.h"
#include "string"
#include "UndefinedType.h"
#include "Token.h"

using namespace std;

UndefinedExpression::UndefinedExpression() {}

BaseValue* UndefinedExpression::eval() {
	return new UndefinedType();
}