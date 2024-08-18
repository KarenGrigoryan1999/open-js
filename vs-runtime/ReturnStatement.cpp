#include "ReturnStatement.h"

using namespace std;

ReturnStatement::ReturnStatement(Expression* exp) {
	this->exp = exp;
	this->result_ = NULL;
}

ReturnStatement::ReturnStatement() {
	this->exp = NULL;
	this->result_ = NULL;
}

EcmaCompletionRecord* ReturnStatement::GetResult() {
	return this->result_;
}

BaseValue* ReturnStatement::eval() {
	//Runtime Semantic
	//ѕункт с генераторами не выполнен так как у нас в рантайме их нет
	this->result_ = new EcmaCompletionRecord{ EcmaCompletionType::Normal, this->exp->eval() };//¬ычисл€ем значение и возвращаем структуру типа CompletionRecord
	throw this;
}

//https://262.ecma-international.org/#prod-ReturnStatement