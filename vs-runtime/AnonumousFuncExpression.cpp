#include "AnonumousFuncExpression.h"

using namespace std;

AnonumousFuncExpression::AnonumousFuncExpression(EcmaEnvironment* block) {
	this->block = block;
	this->body = NULL;
	this->token_position = 0;
	this->parser = NULL;
	this->SetupPrototype();
}

AnonumousFuncExpression::AnonumousFuncExpression(EcmaEnvironment* block, Parser* parser) {
	this->block = block;
	this->body = NULL;
	this->token_position = 0;
	this->parser = parser;
	this->SetupPrototype();
}

void AnonumousFuncExpression::SetupPrototype() {
	ObjectExpression* obj = new ObjectExpression();
	this->add_property("prototype", {
		new BaseValue(obj),
		PropertyDescriptorState::TURNED_OFF,
		PropertyDescriptorState::TURNED_OFF,
		PropertyDescriptorState::TURNED_OFF
		});//Сразу задаем prototype, равный пустому объекту
}

void AnonumousFuncExpression::SetBodyTokenPosition(int token_position) {
	this->token_position = token_position;
}

EcmaEnvironment* AnonumousFuncExpression::getBlock() {
	return this->block;
}

void AnonumousFuncExpression::setBlock(EcmaEnvironment* env) {
	this->block = env;
}

void AnonumousFuncExpression::add(string arg) {
	this->args.push_back(arg);
}

vector<string>* AnonumousFuncExpression::get_arg_list() {
	return &this->args;
}

void AnonumousFuncExpression::setBody(FunctionBodyStatement* body) {
	this->body = body;
}

FunctionBodyStatement* AnonumousFuncExpression::getBody() {
	return this->body;
}

BaseValue* AnonumousFuncExpression::eval() {
	if (this->parser) {
		AnonumousFuncExpression* func = new AnonumousFuncExpression(this->block);
		func->args = this->args;
		FunctionBodyStatement* body = this->parser->func_body_statement(this->token_position, this->block);
		func->setBody(body);

		return new BaseValue(func);
	}

	BaseValue* function = new BaseValue(this);
	
	return function;
}
