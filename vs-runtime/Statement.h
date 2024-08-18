#ifndef STATEMENT_H
#define STATEMENT_H

#include <vector>
#include "string"
#include "Node.h"

using namespace std;

class Statement : public Node
{
	const char* plain_name = "Statement";
};

#endif