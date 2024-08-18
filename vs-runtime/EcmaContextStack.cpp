#include "EcmaContextStack.h"

EcmaContextStack* EcmaContextStack::instance_ = nullptr;

EcmaContextStack::EcmaContextStack() {
	this->context_stack_ = new stack<EcmaExecutionContext*>();
}

EcmaContextStack* EcmaContextStack::Instance() {
	if (!EcmaContextStack::instance_) {
		EcmaContextStack::instance_ = new EcmaContextStack();
	}

	return EcmaContextStack::instance_;
}

void EcmaContextStack::push(EcmaExecutionContext* exec_context) {
	if (this->context_stack_->size() > MAXIMUM_CALLSTACK) throw new BaseException("Maximum call stack size exceeded");
	this->context_stack_->push(exec_context);
}

stack<EcmaExecutionContext*>* EcmaContextStack::GetContextStack() {
	return this->context_stack_;
}

EcmaExecutionContext* EcmaContextStack::top() {
	if (this->context_stack_->size() == 0)
		return NULL;
	return this->context_stack_->top();
}

int EcmaContextStack::size() {
	return this->context_stack_->size();
}

void EcmaContextStack::pop() {
	this->context_stack_->pop();
}

//https://262.ecma-international.org/#execution-context-stack