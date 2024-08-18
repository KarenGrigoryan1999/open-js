#pragma once
#include <iostream>
#include "Token.h"
#include "LexicalTokenType.h"

using namespace std;

Token::Token(LexicalTokenType type, string value) {
	this->type = type;
	this->value = value;
}