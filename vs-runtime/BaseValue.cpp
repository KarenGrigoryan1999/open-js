#include "BaseValue.h"
#include <stdexcept>
#include "ObjectExpression.h"
#include "NativeCodeRealisation.h"
#include "GarbigeCollector.h"

BaseValue::BaseValue(string data, ValueTypes type) {
	this->data = data;
	this->type = type;
	this->registerInHeap();
}

BaseValue::BaseValue(ObjectExpression* data) {
	this->data = "";
	this->ref = data;
	this->type = ValueTypes::FUNCTION_TYPE;
	this->registerInHeap();
}

BaseValue::BaseValue(ObjectExpression* data, ValueTypes type) {
	this->data = "";
	this->ref = data;
	this->type = type;
	this->registerInHeap();
}

ValueTypes BaseValue::GetType() {
	return this->type;
}

int BaseValue::get_data_size() {
	return sizeof(this->data);
}

int BaseValue::get_entity_type() {
	if (this->GetType() == ValueTypes::TDZ) {
		return 2;
	}

	return 1;
}

void BaseValue::trace() {
	GarbigeCollector::Instance()->FindAndMark(this);
	if (this->ref) {
		this->ref->trace();
		vector<EcmaPropertyAttributes>* values = this->ref->get_obj_properties();
		for (auto it = values->begin(); it != values->end(); ++it) {
			if (it->value) {
				it->value->trace();
			}
		}
	}
}

ObjectExpression* BaseValue::GetReference() {
	return this->ref;
}

string BaseValue::GetAsString() {
	if (type == ValueTypes::BOOL_TYPE) {
		if (this->data == "") return "false";
		return "true";
	}
	if (type == ValueTypes::NAN_TYPE) {
		return "NaN";
	}
	if (type == ValueTypes::UNDEFINED_TYPE) {
		return "undefined";
	}
	if (type == ValueTypes::NULL_TYPE) {
		return "null";
	}
	if (type == ValueTypes::OBJECT_TYPE) {
		return "object [Object]";
	}
	if (type == ValueTypes::FUNCTION_TYPE) {
		return "[Function]";
	}
	return this->data;
}

double BaseValue::GetAsNumber() {
	if (type == ValueTypes::BOOL_TYPE) {
		if (this->data == "") return 0;
		return 1;
	}
	if (type == ValueTypes::NULL_TYPE) {
		return 0;
	}
	if (type == ValueTypes::UNDEFINED_TYPE) {
		return NAN;
	}
	try {
		return stod(this->data);
	} catch(const std::invalid_argument& e) {
		return NAN;
	}
}

bool BaseValue::GetAsBoolean() {
	if (type == ValueTypes::NULL_TYPE) {
		return false;
	}
	if (type == ValueTypes::FUNCTION_TYPE || type == ValueTypes::OBJECT_TYPE) {
		return true;
	}
	if (this->data == "") return false;
	try {
		if (stod(this->data) > 0) return true;
		else return false;
	}
	catch (const std::invalid_argument& e) {
		return this->data.size() > 0 ? true : false;
	}
}