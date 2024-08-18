#ifndef OBJECT_EXPRESSION_H
#define OBJECT_EXPRESSION_H

#include <vector>
#include <iostream>
#include "string"
#include "ObjectBase.h"
#include "BaseValue.h"
#include "EcmaPropertyAttributes.h"
#include "NullType.h"
#include "UndefinedType.h"
#include "Token.h"
#include "NumberType.h"
#include "BoolType.h"
#include "EmptyExpression.h"
#include "GarbigeCollector.h"
#include "HiddenProperties.h"

using namespace std;

class ObjectExpression : public CollectableEntity, public ObjectBase
{
public:
	ObjectExpression(bool has_null_proto);
	ObjectExpression();
	void add_property(string property_name, Expression* exp);
	void add_property(string property_name, EcmaPropertyAttributes property);
	BaseValue* eval() override;
	BaseValue* GetPropertyValue(string name);
	vector<string>* get_keys();
	BaseValue* SetPropertyValue(string name, Expression* exp);
	BaseValue* SetPropertyValue(string name, BaseValue* val);
	BaseValue* SetPropertyValueByDescriptor(string name, EcmaPropertyAttributes* props);
	BaseValue* SetHidenPropertyValue(HiddenProperties property, Expression* value);
	BaseValue* get_hidden_property(HiddenProperties property);
	int get_data_size() override;
	void trace() override;
	void set_object_proto(bool has_null_proto);
	int get_entity_type() override;
	vector<EcmaPropertyAttributes>* get_obj_properties();
private:
	vector<EcmaPropertyAttributes> properties;
	vector<string> key_names;

	//Скрытые ключи объектов
	BaseValue* primitive_value;
};

#endif
