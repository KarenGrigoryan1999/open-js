#ifndef EXECUTION_CONTEXT_H
#define EXECUTION_CONTEXT_H

#include "BaseValue.h"
#include "EcmaEnvironment.h"

using namespace std;

class EcmaExecutionContext
{
public:
	EcmaExecutionContext();
	void SetEnvironmentRecord(EcmaEnvironment* env);
	EcmaEnvironment* GetEnvironmentRecord();
	void SetThis(BaseValue* obj);
	BaseValue* GetThis();
private:
	EcmaEnvironment* env_;
	BaseValue* this_;
};

#endif