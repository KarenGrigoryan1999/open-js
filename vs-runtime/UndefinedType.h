#ifndef UNDEFINED_TYPE_H
#define UNDEFINED_TYPE_H

#include "string"
#include "BaseValue.h"
#include "VariableTypes.h"

using namespace std;

class UndefinedType : public BaseValue
{
public:
	UndefinedType() : BaseValue{ "", ValueTypes::UNDEFINED_TYPE } {};
	int get_entity_type() override;
};

#endif