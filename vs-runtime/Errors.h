#ifndef EXCEPTION_BASE_H
#define EXCEPTION_BASE_H

#include "string";
#include "exceptionTypes.h"
#include "BaseException.h"
#include "ReferenceError.h"

using namespace std;

class Errors
{
public:
	static BaseException* ThrowError(ExceptionTypes type, string text);
};

#endif