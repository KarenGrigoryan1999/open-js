#include "ThisExpression.h"
#include "string"
#include "Token.h"
#include "EcmaContextStack.h"

using namespace std;

ThisExpression::ThisExpression() {}

BaseValue* ThisExpression::eval() {
	EcmaExecutionContext* context = EcmaContextStack::Instance()->top();
	if (context) {
		return context->GetThis();
	}
	return IdentifierCollection::GetGlobal("global");
}