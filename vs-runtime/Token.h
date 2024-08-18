#ifndef TOKEN_H
#define TOKEN_H
#include "LexicalTokenType.h"

using namespace std;

class Token
{
public:
	Token(LexicalTokenType type, string value);
	string value;
	LexicalTokenType type;
};

#endif