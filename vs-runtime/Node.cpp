#include "Node.h"
#include "string"
#include "Token.h"
#include "BaseValue.h"
#include "NumberType.h"

using namespace std;

Node::Node() {
}

BaseValue* Node::eval() {
	return new NumberType(0);
}