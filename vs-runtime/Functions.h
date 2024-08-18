#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <map>;
#include <string>;
#include <vector>;
#include "Expression.h";
#include "BaseValue.h"

using namespace std;

class Functions
{
public:
	static BaseValue* Get(string name, vector<Expression*>* args);
};

#endif
