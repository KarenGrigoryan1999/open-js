#ifndef REFERENCE_ERROR_H
#define REFERENCE_ERROR_H

#include <string>
#include "BaseException.h"

using namespace std;

class ReferenceError : public BaseException
{
public:
	ReferenceError(string msg) : BaseException(msg) {};
	const char* plain_name = "ReferenceError";
};

#endif