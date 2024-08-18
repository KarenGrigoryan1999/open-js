#include "string";
#include "vector";
#include "IdentifierCollection.h"
#include "BaseValue.h"
#include "NumberType.h"
#include "UndefinedType.h"
#include "ObjectExpression.h"
#include "AnonumousFuncExpression.h"
#include "IfStatement.h"
#include "ThisExpression.h"
#include "CallExpression.h"
#include "DotExpression.h"
#include "StringExpression.h"
#include "IdentifierReference.h"
#include "NumberType.h"
#include "ReturnStatement.h"
#include "NumberExpression.h"
#include "Functions.h"
#include "NativeCodeRealisation.h"
#include "NullType.h"
#include "Errors.h"

using namespace std;

ObjectExpression* global_context_obj = new ObjectExpression(true);

AnonumousFuncExpression* string_proto_obj = new AnonumousFuncExpression(NULL);
AnonumousFuncExpression* object_proto_obj = new AnonumousFuncExpression(NULL);

map<string, IdentifierCollection::IdentifierValue> IdentifierCollection::global_identifiers = {
	{"Object", {new BaseValue(object_proto_obj), true}},
	{"String", {new BaseValue(string_proto_obj), true}},
	{"global", {new BaseValue(global_context_obj), true}},
	{"PI", {new NumberType(3.14), true}},
	{"EXP", {new NumberType(2.7182), true}},
};

void IdentifierCollection::setupDefaultPrototypes() {
	FunctionBodyStatement* constructor = new FunctionBodyStatement(new EcmaEnvironment());
	DotExpression* left = new DotExpression(new ThisExpression(), HiddenProperties::PrimitiveValue);
	left->set_expression_to_write(new IdentifierReference("str", constructor->env));
	constructor->add(new IfStatement(new ThisExpression(), left, NULL));

	string_proto_obj->add("str");
	string_proto_obj->setBody(constructor);

	BaseValue* string_prototype = string_proto_obj->GetPropertyValue("prototype");

	AnonumousFuncExpression* to_lower_case = new AnonumousFuncExpression(NULL);
	FunctionBodyStatement* to_lower_case_body = new FunctionBodyStatement(NULL);
	to_lower_case_body->add(new NativeCodeRealisation(STRING_TO_LOWER_CASE));
	to_lower_case->setBody(to_lower_case_body);
	string_prototype->GetReference()->add_property("toLowerCase", to_lower_case);

	AnonumousFuncExpression* to_upper_case = new AnonumousFuncExpression(NULL);
	FunctionBodyStatement* to_upper_case_body = new FunctionBodyStatement(NULL);
	to_upper_case_body->add(new NativeCodeRealisation(STRING_TO_UPPER_CASE));
	to_upper_case->setBody(to_upper_case_body);
	string_prototype->GetReference()->add_property("toUpperCase", to_upper_case);

	AnonumousFuncExpression* length_getter = new AnonumousFuncExpression(NULL);
	FunctionBodyStatement* length_getter_body = new FunctionBodyStatement(NULL);
	length_getter_body->add(new NativeCodeRealisation(STRING_LENGTH));

	EcmaPropertyAttributes prop = {};
	prop.get = new BaseValue(length_getter);

	length_getter->setBody(length_getter_body);
	string_prototype->GetReference()->add_property("length", prop);

	//Object
	AnonumousFuncExpression* define_property = new AnonumousFuncExpression(NULL);
	define_property->add("obj");
	define_property->add("prop");
	define_property->add("conf");
	FunctionBodyStatement* define_property_body = new FunctionBodyStatement(NULL);
	define_property_body->add(new NativeCodeRealisation(OBJECT_DEFINE_PROPERTY));
	define_property->setBody(define_property_body);
	object_proto_obj->add_property("defineProperty", define_property);
}

BaseValue* IdentifierCollection::GetGlobal(string name) {
	auto it = IdentifierCollection::global_identifiers.find(name);
	if (it != IdentifierCollection::global_identifiers.end()) {
		BaseValue* variable = it->second.value;
		if (variable->GetType() == ValueTypes::TDZ) {
			throw Errors::ThrowError(ExceptionTypes::ReferenceError, string("Cannot access ") + name + string(" before initialization"));
		} else
		return it->second.value;
	}

	throw Errors::ThrowError(ExceptionTypes::ReferenceError, name + string(" is not defined"));
}

BaseValue* IdentifierCollection::GetLocal(string name) {
	auto it = this->identifiers.find(name);
	if (it != this->identifiers.end()) {
		BaseValue* variable = it->second.value;
		if (variable->GetType() == ValueTypes::TDZ) {
			throw Errors::ThrowError(ExceptionTypes::ReferenceError, string("Cannot access ") + name + string("before initialization"));
		} else
		return it->second.value;
	}

	throw Errors::ThrowError(ExceptionTypes::ReferenceError, name + string(" is not defined"));
}

void IdentifierCollection::SetGlobal(string name, BaseValue* data, bool is_const) {
	auto it = IdentifierCollection::global_identifiers.find(name);
	if (it != IdentifierCollection::global_identifiers.end()) {
		if (it->second.is_const == true && it->second.value->GetType() != ValueTypes::TDZ) throw Errors::ThrowError(ExceptionTypes::TypeError, string("Assignment to constant variable."));
	}

	IdentifierCollection::global_identifiers[name] = { data, is_const };
}

void IdentifierCollection::SetLocal(string name, BaseValue* data, bool is_const) {
	auto it = this->identifiers.find(name);
	if (it != this->identifiers.end()) {
		if (it->second.is_const == true && it->second.value->GetType() != ValueTypes::TDZ) throw Errors::ThrowError(ExceptionTypes::TypeError, string("Assignment to constant variable."));
	}

	this->identifiers[name] = { data, is_const };
}
