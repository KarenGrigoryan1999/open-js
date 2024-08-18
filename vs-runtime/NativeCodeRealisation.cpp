#include "NativeCodeRealisation.h"
#include "string"
#include "StringType.h"
#include "Token.h"
#include "Expression.h"
#include "DotExpression.h"
#include "ThisExpression.h"
#include "UndefinedType.h"
#include "NumberType.h"
#include "EcmaEnvironment.h"
#include "EcmaContextStack.h"
#include "ObjectExpression.h"
#include "Errors.h"

using namespace std;

NativeCodeRealisation::NativeCodeRealisation(native_method_type type) {
	this->type = type;
}

BaseValue* NativeCodeRealisation::eval() {
	if (this->type == OBJECT_DEFINE_PROPERTY) {
		IdentifierCollection* env = EcmaContextStack::Instance()->top()->GetEnvironmentRecord()->records;
		BaseValue* current_obj = env->GetLocal("obj");
		BaseValue* obj_property = env->GetLocal("prop");
		BaseValue* obj_config = env->GetLocal("conf");
		ObjectExpression* obj_config_ref = obj_config->GetReference();

		if (current_obj->GetType() != ValueTypes::FUNCTION_TYPE && current_obj->GetType() != ValueTypes::OBJECT_TYPE) {
			throw Errors::ThrowError(ExceptionTypes::ReferenceError, "Object.defineProperty called on non-object");
		}

		if (obj_property->GetType() != ValueTypes::STRING_TYPE) {
			throw Errors::ThrowError(ExceptionTypes::TypeError, "Invalid property descriptor. Cannot both specify accessors and a value or writable attribute, #<Object>");
		}

		if (obj_config->GetType() != ValueTypes::FUNCTION_TYPE && obj_config->GetType() != ValueTypes::OBJECT_TYPE) {
			throw Errors::ThrowError(ExceptionTypes::TypeError, "Property description must be an object" + obj_config->GetAsString());
		}

		EcmaPropertyAttributes prop;
		BaseValue* w = obj_config_ref->GetPropertyValue("writable");
		BaseValue* c = obj_config_ref->GetPropertyValue("configurable");
		BaseValue* e = obj_config_ref->GetPropertyValue("enumirable");
		BaseValue* value = obj_config_ref->GetPropertyValue("value");
		BaseValue* get = obj_config_ref->GetPropertyValue("get");
		BaseValue* set = obj_config_ref->GetPropertyValue("set");

		prop.writable = this->convert_to_object_property_descriptor_state(w);
		prop.configurable = this->convert_to_object_property_descriptor_state(c);
		prop.enumerable = this->convert_to_object_property_descriptor_state(e);
		if (value->GetType() != ValueTypes::UNDEFINED_TYPE) prop.value = value;
		else prop.value = NULL;
		if (get->GetType() != ValueTypes::UNDEFINED_TYPE) prop.get = get;
		else prop.get = NULL;
		if (set->GetType() != ValueTypes::UNDEFINED_TYPE) prop.set = set;
		else prop.set = NULL;

		if (
			(get->GetType() != ValueTypes::UNDEFINED_TYPE || set->GetType() != ValueTypes::UNDEFINED_TYPE) &&
			(prop.writable != PropertyDescriptorState::UNSET || prop.enumerable != PropertyDescriptorState::UNSET || prop.configurable != PropertyDescriptorState::UNSET)
		) {
			throw Errors::ThrowError(ExceptionTypes::TypeError, "Invalid property descriptor. Cannot both specify accessors and a value or writable attribute, #<Object>");
		}

		BaseValue* val = current_obj->GetReference()->SetPropertyValueByDescriptor(obj_property->GetAsString(), &prop);

		return new UndefinedType();
	}
	string val = (new DotExpression(new ThisExpression(), HiddenProperties::PrimitiveValue))->eval()->GetAsString();
	BaseValue* result = NULL;
	if (this->type == STRING_TO_LOWER_CASE) {
		for (char& c : val) {
			c = tolower(c);
		}

		result = new StringType(val);
	}

	if (this->type == STRING_TO_UPPER_CASE) {
		for (char& c : val) {
			c = toupper(c);
		}

		result = new StringType(val);
	}

	if (this->type == STRING_LENGTH) {
		result = new NumberType(val.length());
	}
	
	throw new EcmaCompletionRecord{ EcmaCompletionType::Return, result };
}

PropertyDescriptorState NativeCodeRealisation::convert_to_object_property_descriptor_state(BaseValue* val) {
	if (val->GetType() != ValueTypes::UNDEFINED_TYPE) {
		return val->GetAsBoolean() ? PropertyDescriptorState::TURNED_ON : PropertyDescriptorState::TURNED_OFF;
	}
	return PropertyDescriptorState::UNSET;
}