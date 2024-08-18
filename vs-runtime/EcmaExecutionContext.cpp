#include "EcmaExecutionContext.h"

EcmaExecutionContext::EcmaExecutionContext() {
	this->env_ = NULL;
	this->this_ = NULL;
}

void EcmaExecutionContext::SetEnvironmentRecord(EcmaEnvironment* env) {
	this->env_ = env;
}

EcmaEnvironment* EcmaExecutionContext::GetEnvironmentRecord() {
	return this->env_;
}

void EcmaExecutionContext::SetThis(BaseValue* obj) {
	this->this_ = obj;
}

BaseValue* EcmaExecutionContext::GetThis() {
	return this->this_;
}

//https://262.ecma-international.org/#sec-execution-contexts