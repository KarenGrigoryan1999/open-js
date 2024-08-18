#ifndef BASE_EXCEPTION_H
#define BASE_EXCEPTION_H

#include <string>

using namespace std;

class BaseException
{
public:
	BaseException(string msg);
	string GetMsg();
	const char* plain_name = "BaseException";
private:
	string msg;
};

#endif