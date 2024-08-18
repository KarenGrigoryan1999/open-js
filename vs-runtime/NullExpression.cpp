#include "NullExpression.h"
#include "string"
#include "NullType.h"
#include "Token.h"

using namespace std;

NullExpression::NullExpression() {}

BaseValue* NullExpression::eval() {
	return new NullType();
}