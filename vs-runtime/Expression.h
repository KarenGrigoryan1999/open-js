#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include "string"
#include "Node.h"

using namespace std;

class Expression: public Node
{
public:
	const char* plain_name = "Expression";
};

#endif