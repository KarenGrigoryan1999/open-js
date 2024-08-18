#ifndef STRING_TYPE_H
#define STRING_TYPE_H

#include "string"
#include "BaseValue.h"
#include "VariableTypes.h"

using namespace std;

class StringType : public BaseValue
{
public:
	StringType(string data) : BaseValue(data, ValueTypes::STRING_TYPE) {};
	int get_entity_type() override;
};

#endif