#ifndef TDZ_TYPE_H
#define TDZ_TYPE_H

#include "string"
#include "BaseValue.h"
#include "VariableTypes.h"

using namespace std;

class TDZValue : public BaseValue
{
public:
	TDZValue() : BaseValue{ "", ValueTypes::TDZ} {};
	ValueTypes GetType();
	ObjectExpression* get_ref();
	string GetAsString();
	double GetAsNumber();
	bool GetAsBoolean();
	int get_entity_type() override;
private:
	void base_tdz_get_value();
};

#endif