#ifndef CONTEXT_STACK_H
#define CONTEXT_STACK_H

#include <map>
#include <string>
#include <stack>
#include "BaseValue.h"
#include "IdentifierCollection.h"
#include "EcmaExecutionContext.h"
#include "BaseException.h"

#define MAXIMUM_CALLSTACK 100

using namespace std;

class EcmaContextStack
{
public:
	EcmaContextStack();
	stack<EcmaExecutionContext*>* GetContextStack();
	void push(EcmaExecutionContext* exec_context);
	void pop();
	EcmaExecutionContext* top();
	int size();
	static EcmaContextStack* Instance();
	static EcmaContextStack* instance_;
private:
	stack<EcmaExecutionContext*>* context_stack_;
};

#endif
