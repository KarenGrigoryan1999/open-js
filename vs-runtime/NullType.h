#ifndef NULL_TYPE_H
#define NULL_TYPE_H

#include "string"
#include "BaseValue.h"
#include "VariableTypes.h"

using namespace std;

class NullType : public BaseValue
{
public:
	NullType() : BaseValue{ "", ValueTypes::NULL_TYPE } {};
	int get_entity_type() override;
};

#endif