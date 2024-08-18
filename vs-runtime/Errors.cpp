#include "Errors.h"

BaseException* Errors::ThrowError(ExceptionTypes type, string text) {
	switch (type)
	{
	case ExceptionTypes::ReferenceError:
		return new ReferenceError(string("Uncaught ReferenceError: ") + text);
	case ExceptionTypes::SyntaxError:
		return new BaseException(string("Uncaught SyntaxError: ") + text);
	case ExceptionTypes::TypeError:
		return new BaseException(string("Uncaught TypeError: ") + text);
	default:
		return new BaseException("UnknownError");
	}
}