#include "ObjectExpression.h"
#include "CallExpression.h"

using namespace std;

ObjectExpression::ObjectExpression(bool has_null_proto) {
	this->set_object_proto(has_null_proto);
}

ObjectExpression::ObjectExpression() {
	this->set_object_proto(true);
}

void ObjectExpression::add_property(string property_name, Expression* exp) {
	this->properties.push_back({
		exp->eval(),
		PropertyDescriptorState::TURNED_ON,
		PropertyDescriptorState::TURNED_ON,
		PropertyDescriptorState::TURNED_ON
	});
	this->key_names.push_back(property_name);
}

BaseValue* ObjectExpression::SetHidenPropertyValue(HiddenProperties property, Expression* value) {
	switch (property)
	{
	case HiddenProperties::PrimitiveValue:
		return this->primitive_value = value->eval();
	default:
		return this->primitive_value = value->eval();
	}
}

BaseValue* ObjectExpression::get_hidden_property(HiddenProperties property) {
	switch (property)
	{
	case HiddenProperties::PrimitiveValue:
		return this->primitive_value;
		break;
	default:
		return this->primitive_value;
	}
}

void ObjectExpression::add_property(string property_name, EcmaPropertyAttributes property) {
	this->properties.push_back(property);
	this->key_names.push_back(property_name);
}

int ObjectExpression::get_data_size() {
	return sizeof(this->key_names) + sizeof(this->properties);
}

void ObjectExpression::trace() {
	GarbigeCollector::Instance()->FindAndMark(this);
}

vector<string>* ObjectExpression::get_keys() {
	return &this->key_names;
}

BaseValue* ObjectExpression::GetPropertyValue(string name) {
	ObjectExpression* current = this;
	
	while (current != NULL) {
		for (int i = 0; i < current->key_names.size(); i++) {
			if (name == current->key_names.at(i)) {
				EcmaPropertyAttributes property = current->properties.at(i);
				//Сначала проверяем: есть ли геттер?
				if (property.get != NULL) {
					CallExpression* getter_caller = new CallExpression(property.get->GetReference(), NULL);
					getter_caller->SetupThisObject(this);
					return getter_caller->eval();
				}
				//Если нет, то возвращаем свойство
				return current->properties.at(i).value;
			}
		}

		BaseValue* proto = current->GetPropertyValue("__proto__");
		ValueTypes var_type = proto->GetType();
		if (var_type == ValueTypes::OBJECT_TYPE || var_type == ValueTypes::FUNCTION_TYPE) {
			current = proto->GetReference();
		}
		else {
			current = NULL;
		}
	}

	return new UndefinedType();
}

BaseValue* ObjectExpression::SetPropertyValueByDescriptor(string name, EcmaPropertyAttributes* props) {
	bool is_exist = false;
	for (int i = 0; i < this->key_names.size(); i++) {
		if (name == this->key_names.at(i)) {
			EcmaPropertyAttributes* property = &this->properties.at(i);
			is_exist = true;
			if (property->configurable != PropertyDescriptorState::TURNED_OFF) {
				if (props->writable != PropertyDescriptorState::UNSET) property->writable = props->writable;
				if (props->configurable != PropertyDescriptorState::UNSET) property->configurable = props->configurable;
				if (props->enumerable != PropertyDescriptorState::UNSET) property->enumerable = props->enumerable;
				if (props->get != NULL) property->get = props->get;
				if (props->set != NULL) property->set = props->set;
				if (props->value != NULL) property->value = props->value;
			}
		}
	}

	if (!is_exist) {
		this->properties.push_back(*props);
		this->key_names.push_back(name);
	}

	return props->value;
}

BaseValue* ObjectExpression::SetPropertyValue(string name, BaseValue* val) {
	bool is_exist = false;

	for (int i = 0; i < this->key_names.size(); i++) {
		if (name == this->key_names.at(i)) {
			EcmaPropertyAttributes* property = &this->properties.at(i);
			is_exist = true;
			//Проверяю, есть ли сеттер
			if (property->set != NULL) {
				CallExpression* setter_caller = new CallExpression(property->set->GetReference(), NULL);
				setter_caller->add(new EmptyExpression(val));
				setter_caller->SetupThisObject(this);
				property->value = setter_caller->eval();
			}
			else if (property->writable == PropertyDescriptorState::TURNED_ON) {
				//Если сеттера нет, то просто устанавливает значение
				property->value = val;
			}
		}
	}

	if (!is_exist) {
		this->properties.push_back({ val, PropertyDescriptorState::TURNED_ON, PropertyDescriptorState::TURNED_ON, PropertyDescriptorState::TURNED_ON });
		this->key_names.push_back(name);
	}

	return val;
}

BaseValue* ObjectExpression::SetPropertyValue(string name, Expression* exp) {
	BaseValue* val = exp->eval();
	return this->SetPropertyValue(name, val);
}

vector<EcmaPropertyAttributes>* ObjectExpression::get_obj_properties() {
	return &this->properties;
}

int ObjectExpression::get_entity_type() {
	return 3;
}

BaseValue* ObjectExpression::eval() {
	return new BaseValue(this, ValueTypes::OBJECT_TYPE);
}

void ObjectExpression::set_object_proto(bool has_null_proto) {
	//Если has_null_proto = true, то мы выставляем __proto__ как null
	this->properties.push_back({
	has_null_proto ? new NullType() : IdentifierCollection::GetGlobal("Object")->GetReference()->GetPropertyValue("prototype"),
	PropertyDescriptorState::TURNED_ON,
	PropertyDescriptorState::TURNED_ON,
	PropertyDescriptorState::TURNED_ON
		});
	this->key_names.push_back("__proto__");
}