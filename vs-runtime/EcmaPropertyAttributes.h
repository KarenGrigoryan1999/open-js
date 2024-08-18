#pragma once
#include "BaseValue.h"
#include "ObjectPropertyDescriptorState.h"

struct EcmaPropertyAttributes {
	BaseValue* value; //[[Value]]
	PropertyDescriptorState writable; //[[Writable]]
	PropertyDescriptorState configurable; //[[Configurable]]
	PropertyDescriptorState enumerable; //[[Enumerable]]
	BaseValue* get; //[[Get]]
	BaseValue* set; //[[Set]]
};

//https://262.ecma-international.org/#sec-property-attributes