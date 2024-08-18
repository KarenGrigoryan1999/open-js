#ifndef SYNTAX_ERROR_H
#define SYNTAX_ERROR_H

#include <string>
#include "BaseException.h"

using namespace std;

class SyntaxError : public BaseException
{
public:
	SyntaxError(string msg) : BaseException(msg) {};
	const char* plain_name = "SyntaxError";
};

#endif