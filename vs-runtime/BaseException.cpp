#include "BaseException.h"

BaseException::BaseException(string msg) {
	this->msg = msg;
}

string BaseException::GetMsg() {
	return this->msg;
}