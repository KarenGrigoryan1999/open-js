#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include "Token.h"
#include "LexicalTokenType.h"

using namespace std;

class Lexer {
public:
    string code;
    string tokenize_temp;
    vector<Token> token_list;
    char symbol;
    int pos;
    Lexer(string code);
    vector<Token>* tokenize();
private:
    void next();
    char peek();
    void tokenize_bracket_operator(LexicalTokenType type);
    void tokenize_math_operator(LexicalTokenType type);
    void tokenize_numeric(LexicalTokenType type);
    void tokenize_word(LexicalTokenType type);
    void base_tokenize(LexicalTokenType type);
    void equal_operator();
    void tokenize_slash_symbol();
    void tokenize_amp_operator();
    void tokenize_or_operator();
    void tokenize_plus_operator();
    void tokenize_minus_operator();
    void tokenize_inverse_operator();
    void tokenize_relative_operator(LexicalTokenType type);
    void tokenize_plain_text(char quotation_symbol);
};
#endif