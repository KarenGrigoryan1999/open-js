#include <iostream>
#include <vector>
#include <cctype>

#include "Lexer.h"
#include "LexicalTokenType.h"

using namespace std;

Lexer::Lexer(string code) {
	this->code = code;
	this->token_list = {};
	pos = 0;
	symbol = code[0];
}

vector<Token>* Lexer::tokenize() {
	while (pos < code.size()) {
		
		if (symbol == '+') {
			this->tokenize_plus_operator();
		}
		else if (symbol == '-') {
			this->tokenize_minus_operator();
		}
		else if (symbol == '!') {
			this->tokenize_inverse_operator();
		}
		else if (symbol == '/') {
			this->tokenize_slash_symbol();
		}
		else if (symbol == '*') {
			this->tokenize_math_operator(LexicalTokenType::STAR);
		}
		else if (symbol == ',') {
			this->tokenize_math_operator(LexicalTokenType::COMMA);
		}
		else if (symbol == '(') {
			this->tokenize_math_operator(LexicalTokenType::L_BRACKET);
		}
		else if (symbol == ')') {
			this->tokenize_math_operator(LexicalTokenType::R_BRACKET);
		}
		else if (symbol == ';') {
			this->tokenize_math_operator(LexicalTokenType::SEMICOLON);
		}
		else if (symbol == '{') {
			this->tokenize_math_operator(LexicalTokenType::L_BLOCK);
		}
		else if (symbol == ':') {
			this->tokenize_math_operator(LexicalTokenType::COLON);
		}
		else if (symbol == '[') {
			this->tokenize_math_operator(LexicalTokenType::SQ_BRACKET_L);
		}
		else if (symbol == ']') {
			this->tokenize_math_operator(LexicalTokenType::SQ_BRACKET_R);
		}
		else if (symbol == '}') {
			this->tokenize_math_operator(LexicalTokenType::R_BLOCK);
		}
		else if (symbol == '.') {
			this->tokenize_math_operator(LexicalTokenType::DOT_OP);
		}
		else if (symbol == '=') {
			this->equal_operator();
		}
		else if (symbol == '<') {
			this->tokenize_relative_operator(LexicalTokenType::LT);
		}
		else if (symbol == '>') {
			this->tokenize_relative_operator(LexicalTokenType::GT);
		}
		else if (symbol == '&') {
			this->tokenize_amp_operator();
		}
		else if (symbol == '|') {
			this->tokenize_or_operator();
		}
		else if (symbol == '\'' || symbol == '\"') {
			this->tokenize_plain_text(symbol);
		}
		else if(std::isdigit(symbol)) {
			this->tokenize_numeric(LexicalTokenType::NUMBER);
		}
		else if (isalpha(symbol) || symbol == '_') {
			this->tokenize_word(LexicalTokenType::ONE_WORD);
		}
		else {
			this->next();
		}
	}

	return &this->token_list;
}

void Lexer::next() {
	this->pos += 1;
	symbol = this->peek();
}

char Lexer::peek() {
	if (pos < code.size()) {
		return code[pos];
	}
	else {
		return '\0';
	}
}

void Lexer::tokenize_inverse_operator() {
	this->next();
	if (symbol == '=') {
		this->next();
		if (symbol == '=') {
			this->next();
			this->base_tokenize(LexicalTokenType::NOT_EQ_STRICT_CHECKING);
		}
		else {
			this->base_tokenize(LexicalTokenType::NOT_EQ_CHECKING);
		}
	} else
	this->base_tokenize(LexicalTokenType::INVERSE);
}

void Lexer::tokenize_bracket_operator(LexicalTokenType type) {
	this->base_tokenize(type);
	this->next();
}

void Lexer::tokenize_plus_operator() {
	this->next();
	while(symbol == ' ')this->next();
	if (symbol == '+') {
		this->base_tokenize(LexicalTokenType::INCREMENT);
		this->next();
	} else
	this->base_tokenize(LexicalTokenType::PLUS);
}

void Lexer::tokenize_minus_operator() {
	this->next();
	while (symbol == ' ')this->next();
	if (symbol == '-') {
		this->base_tokenize(LexicalTokenType::DECREMENT);
		this->next();
	}
	else
		this->base_tokenize(LexicalTokenType::MINUS);
}

void Lexer::tokenize_slash_symbol() {
	this->tokenize_temp += symbol;
	this->next();
	this->tokenize_temp += symbol;
	this->next();
	if (this->tokenize_temp == "//") {
		while (symbol != '\n') {
			this->tokenize_temp += symbol;
			this->next();
		}
		this->base_tokenize(LexicalTokenType::SIMPLE_COMMENT);
	}
	else if (this->tokenize_temp == "/*") {
		this->next();
		this->next();
		while (symbol != '/' || code[pos-1] != '*') {
			this->tokenize_temp += symbol;
			this->next();
		}
		this->next();
		this->base_tokenize(LexicalTokenType::MULTILINE_COMMENT);
	}
	else {
		this->tokenize_temp = "/";
		this->base_tokenize(LexicalTokenType::DIV);
	}
	this->tokenize_temp = "";
}

void Lexer::tokenize_relative_operator(LexicalTokenType type) {
	this->tokenize_temp += symbol;
	this->next();
	this->tokenize_temp += symbol;
	this->next();
	if ((this->tokenize_temp == ">=" && type == LexicalTokenType::GT) || (this->tokenize_temp == "<=" && type == LexicalTokenType::LT)) {
		this->base_tokenize(type == LexicalTokenType::GT ? LexicalTokenType::GTE : LexicalTokenType::LTE);
		this->next();
	}
	else {
		this->base_tokenize(type);
	}
	this->tokenize_temp = "";
}

void Lexer::tokenize_amp_operator() {
	this->tokenize_temp += symbol;
	this->next();
	this->tokenize_temp += symbol;
	this->next();
	if (this->tokenize_temp == "&&") {
		this->base_tokenize(LexicalTokenType::LOGICAL_AND_EXPRESSION);
		this->next();
	} else {
		this->base_tokenize(LexicalTokenType::LOGICAL_AND_EXPRESSION);
	}
	this->tokenize_temp = "";
}

void Lexer::tokenize_or_operator() {
	this->tokenize_temp += symbol;
	this->next();
	this->tokenize_temp += symbol;
	this->next();
	if (this->tokenize_temp == "||") {
		this->base_tokenize(LexicalTokenType::LOGICAL_OR_EXPRESSION);
		this->next();
	}
	else {
		this->base_tokenize(LexicalTokenType::LOGICAL_OR_EXPRESSION);
	}
	this->tokenize_temp = "";
}

void Lexer::tokenize_math_operator(LexicalTokenType type) {
	this->base_tokenize(type);
	this->next();
}

void Lexer::equal_operator() {
	while (symbol == '=') {
		this->tokenize_temp += symbol;
		this->next();
	}

	if (this->tokenize_temp.size() == 1) {
		this->base_tokenize(LexicalTokenType::EQUAL);
	}
	if (this->tokenize_temp.size() == 2) {
		this->base_tokenize(LexicalTokenType::EQ_CHECKING);
	}
	if (this->tokenize_temp.size() == 3) {
		this->base_tokenize(LexicalTokenType::EQ_STRICT_CHECKING);
	}
	this->tokenize_temp = "";
}

void Lexer::tokenize_plain_text(char quotation_symbol) {
	this->next();
	while (symbol != quotation_symbol) {
		if (this->pos >= code.size() || symbol == '\n') {
			throw "Uncaught SyntaxError: Invalid or unexpected token";
		}
		this->tokenize_temp += symbol;
		this->next();
	}
	this->next();
	this->base_tokenize(LexicalTokenType::PLAIN_TEXT);
	this->tokenize_temp = "";

}

void Lexer::tokenize_word(LexicalTokenType type) {
	while (std::isdigit(symbol) || isalpha(symbol) || symbol == '_') {
		this->tokenize_temp += symbol;
		this->next();
	}
	if (this->tokenize_temp == "if") {
		while (symbol == ' ') this->next();
		if (symbol == '(') this->base_tokenize(LexicalTokenType::IF_STATEMENT);
		else throw "unexpected token";
	}
	else if (this->tokenize_temp == "for") {
		while (symbol == ' ') this->next();
		if (symbol == '(') this->base_tokenize(LexicalTokenType::FOR_STATEMENT);
		else throw "unexpected token";
	}
	else if (this->tokenize_temp == "while") {
		while (symbol == ' ') this->next();
		if (symbol == '(') this->base_tokenize(LexicalTokenType::WHILE_STATEMENT);
		else throw "unexpected token";
	}
	else if (this->tokenize_temp == "else") {
		this->tokenize_math_operator(LexicalTokenType::ELSE_STATEMENT);
	}
	else if (this->tokenize_temp == "NaN") {
		this->base_tokenize(LexicalTokenType::NAN_EXPRESSION);
	}
	else if (this->tokenize_temp == "function") {
		this->base_tokenize(LexicalTokenType::FUNCTION);
	}
	else if (this->tokenize_temp == "undefined") {
		this->base_tokenize(LexicalTokenType::UNDEFINED_EXPRESSION);
	}
	else if (this->tokenize_temp == "null") {
		this->base_tokenize(LexicalTokenType::NULL_EXPRESSION);
	}
	else if (this->tokenize_temp == "this") {
		this->base_tokenize(LexicalTokenType::THIS_KEYWORD);
	}
	else if (this->tokenize_temp == "new") {
		this->base_tokenize(LexicalTokenType::NEW_OPERATOR);
	}
	else if (this->tokenize_temp == "return") {
		this->tokenize_math_operator(LexicalTokenType::RETURN);
	}
	else if (this->tokenize_temp == "typeof") {
		this->tokenize_math_operator(LexicalTokenType::TYPEOF);
	}
	else if (this->tokenize_temp == "true" || this->tokenize_temp == "false") {
		this->base_tokenize(LexicalTokenType::BOOLEAN_ELEMENT);
	}
	else {
		this->base_tokenize(type);
	}
	this->tokenize_temp = "";
}

void Lexer::tokenize_numeric(LexicalTokenType type) {
	bool was_point_added = false;
	while (std::isdigit(symbol) || symbol == '.') {
		if (symbol == '.') {
			if (was_point_added == true) throw "error";
			was_point_added = true;
		}
		this->tokenize_temp += symbol;
		this->next();
	}
	this->base_tokenize(type);
	this->tokenize_temp = "";
}

void Lexer::base_tokenize(LexicalTokenType type) {
	Token token(type, this->tokenize_temp);
	token_list.push_back(token);
}
