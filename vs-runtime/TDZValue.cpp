#include "TDZValue.h"
#include "ObjectExpression.h"
#include "AnonumousFuncExpression.h"

AnonumousFuncExpression* DefaultFunc = new AnonumousFuncExpression(NULL);

ObjectExpression* TDZValue::get_ref() {
	base_tdz_get_value();
	return DefaultFunc;
}

string TDZValue::GetAsString() {
	base_tdz_get_value();
	return "";
}

double TDZValue::GetAsNumber() {
	base_tdz_get_value();
	return 0;
}

bool TDZValue::GetAsBoolean() {
	base_tdz_get_value();
	return false;
}

int TDZValue::get_entity_type() {
	return 4;
}

void TDZValue::base_tdz_get_value() {
	throw string("Uncaught ReferenceError: Cannot access variable before initialization");
}