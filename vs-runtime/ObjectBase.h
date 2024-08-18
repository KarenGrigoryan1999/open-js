#ifndef OBJECT_BASE_STATEMENT_H
#define OBJECT_BASE_STATEMENT_H

#include "Expression.h"

using namespace std;

class ObjectBase : public Expression {
	const char* plain_name = "ObjectBase";
};

#endif