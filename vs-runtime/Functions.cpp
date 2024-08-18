#include "Functions.h"
#include "BaseValue.h"
#include "NumberType.h"
#include "Expression.h"
#include "UndefinedType.h"
#include "ObjectExpression.h"
#include "EmptyExpression.h"
#include "BoolType.h"
#include "NanType.h"
#include "StringType.h"
#include <iostream>
#include "Errors.h"
#include "JobQueue.h"
#include "CallExpression.h"
#include "EventLoop.h"
#include "EcmaContextStack.h"

using namespace std;

#define BLUE_TEXT "\033[34m"
#define LIGHT_YELLOW_TEXT "\033[93m"
#define ANSI_COLOR_GRAY "\x1b[90m"
#define RESET_COLOR "\033[0m"

BaseValue* print(vector<Expression*>* args) {
	for (int i = 0; i < args->size(); i++) {
		BaseValue* res = args->at(i)->eval();
		switch (res->GetType())
		{
		case ValueTypes::NUMBER_TYPE:
			cout << BLUE_TEXT << res->GetAsNumber() << RESET_COLOR;
			break;
		case ValueTypes::UNDEFINED_TYPE:
			cout << ANSI_COLOR_GRAY << res->GetAsString() << RESET_COLOR;
			break;
		case ValueTypes::NULL_TYPE:
			cout << BLUE_TEXT << res->GetAsString() << RESET_COLOR;
			break;
		case ValueTypes::OBJECT_TYPE:
		{
			cout << ANSI_COLOR_GRAY << "{" << endl;
			ObjectExpression* obj = res->GetReference();
			vector<string>* obj_keys = obj->get_keys();

			BaseValue* primitive_value = obj->get_hidden_property(HiddenProperties::PrimitiveValue);

			if(primitive_value) cout << "   [[PrimitiveValue]]" << ": " << primitive_value->GetAsString() << endl;

			for (int i = 0; i < obj_keys->size(); i++) {
				cout << "   " << obj_keys->at(i) << ": " << obj->GetPropertyValue(obj_keys->at(i))->GetAsString() << endl;
			}
			cout << "}" << RESET_COLOR;
			break;
		}
		default:
			cout << LIGHT_YELLOW_TEXT  << res->GetAsString() << RESET_COLOR;
			break;
		}

		cout << " ";
	}
	cout << endl;

	return new NumberType(0);
}

BaseValue* input(vector<Expression*>* args) {
	string temp;
	if(args->size() > 0) cout << args->at(0)->eval()->GetAsString();
	cin >> temp;

	return new StringType(temp);
}

BaseValue* is_nan(vector<Expression*>* args) {
	string temp;
	if (args->at(0)->eval()->GetType() == ValueTypes::NAN_TYPE) return new BoolType(true);

	return new BoolType(false);
}

BaseValue* parse_int(vector<Expression*>* args) {
	string str = args->at(0)->eval()->GetAsString();
	string buffer = "";
	for (int i = 0; i < str.size(); i++) {
		if (std::isdigit(str[i])) buffer += str[i];
		else break;
	}

	if (buffer == "") return new NanType();

	return new NumberType(stod(buffer));
}

BaseValue* clear_timeout(vector<Expression*>* args) {
	EventLoop::ClearTimer(args->at(0)->eval()->GetAsNumber());

	return new UndefinedType();
}

BaseValue* set_timeout(vector<Expression*>* args, bool repeate) {
	Expression* first_arg = args->at(0);
	int identifier = 0;

	if (dynamic_cast<AnonumousFuncExpression*>(first_arg)) {
		EcmaExecutionContext* cnt = EcmaContextStack::Instance()->top();
		CallExpression* exp = new CallExpression(first_arg, cnt ? cnt->GetEnvironmentRecord() : NULL);

		((AnonumousFuncExpression*)first_arg)->setBlock(new EcmaEnvironment{ NULL, cnt ? cnt->GetEnvironmentRecord() : NULL });

		for (int i = 2; i < args->size(); i++) {
			exp->add(args->at(i));
		}

		identifier = EventLoop::SetupTimer(exp);
	}
	else {
		identifier = EventLoop::SetupTimer(first_arg);
	}

	int delay = args[0].size() > 0 ? args[0].at(1)->eval()->GetAsNumber() : 0;

	EventLoop::StartTimer(delay, repeate);

	return new NumberType(identifier);
}

BaseValue* queue_microtask(vector<Expression*>* args) {
	Expression* first_arg = args->at(0);

	if (dynamic_cast<AnonumousFuncExpression*>(first_arg)) {
		EcmaExecutionContext* cnt = EcmaContextStack::Instance()->top();

		CallExpression* exp = new CallExpression(args->at(0), NULL);
		((AnonumousFuncExpression*)first_arg)->setBlock(new EcmaEnvironment{NULL, cnt ? cnt->GetEnvironmentRecord() : NULL });

		for (int i = 2; i < args->size(); i++) {
			exp->add(args->at(i));
		}

		EventLoop::SetupPromise(exp);
	}
	else {
		EventLoop::SetupPromise(first_arg);
	}

	EventLoop::StartPromise();

	return new UndefinedType();
}

BaseValue* Functions::Get(string name, vector<Expression*>* args) {
	if (name == "print") {
		return print(args);
	}
	if (name == "meow") {
		return input(args);
	}
	if (name == "isNaN") {
		return is_nan(args);
	}
	if (name == "parseInt") {
		return parse_int(args);
	}
	if (name == "setTimeout") {
		return set_timeout(args, false);
	}
	if (name == "clearTimeout" || name == "clearInterval") {
		return clear_timeout(args);
	}
	if (name == "setInterval") {
		return set_timeout(args, true);
	}
	if (name == "queueMicrotask") {
		return queue_microtask(args);
	}

	Errors::ThrowError(ExceptionTypes::ReferenceError, "function is not defined");
}
