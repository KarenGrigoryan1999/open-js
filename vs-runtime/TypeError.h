#ifndef TYPE_ERROR_H
#define TYPE_ERROR_H

#include <string>
#include "BaseException.h"

using namespace std;

class TypeError : public BaseException
{
public:
	TypeError(string msg) : BaseException(msg) {};
	const char* plain_name = "TypeError";
};

#endif