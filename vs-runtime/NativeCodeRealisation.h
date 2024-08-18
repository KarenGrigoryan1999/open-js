#ifndef NATIVE_CODE_EXPRESSION_H
#define NATIVE_CODE_EXPRESSION_H

#include <vector>
#include "string"
#include "Expression.h"
#include "ReturnStatement.h"
#include "BaseValue.h"
#include "NativeMethodsTypes.h"
#include "ObjectPropertyDescriptorState.h"

using namespace std;

class NativeCodeRealisation : public ReturnStatement
{
public:
	NativeCodeRealisation(native_method_type type);
	BaseValue* eval() override;
	const char* plain_name = "NativeCodeRealisation";
private:
	Expression* exp;
	native_method_type type;
	PropertyDescriptorState convert_to_object_property_descriptor_state(BaseValue* val);
};

#endif