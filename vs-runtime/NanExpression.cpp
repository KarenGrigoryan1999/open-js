#include "NanExpression.h"
#include "string"
#include "NanType.h"
#include "Token.h"

using namespace std;

NanExpression::NanExpression() {}

BaseValue* NanExpression::eval() {
	return new NanType();
}