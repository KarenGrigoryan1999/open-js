#ifndef BASE_VALUE_H
#define BASE_VALUE_H

#include "string"
#include "CollectableEntity.h"
#include "VariableTypes.h"

using namespace std;

class ObjectExpression;
class BaseValue: public CollectableEntity
{
public:
	BaseValue(string data, ValueTypes type);
	BaseValue(ObjectExpression* exp, ValueTypes type);
	BaseValue(ObjectExpression* exp);
	virtual string GetAsString();
	virtual double GetAsNumber();
	virtual bool GetAsBoolean();
	ValueTypes GetType();
	int get_data_size() override;
	virtual ObjectExpression* GetReference();
	void trace() override;
	int get_entity_type() override;
protected:
	string data;
	ValueTypes type;
	ObjectExpression* ref;
};

#endif