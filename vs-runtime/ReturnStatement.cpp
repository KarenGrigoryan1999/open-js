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
	//����� � ������������ �� �������� ��� ��� � ��� � �������� �� ���
	this->result_ = new EcmaCompletionRecord{ EcmaCompletionType::Normal, this->exp->eval() };//��������� �������� � ���������� ��������� ���� CompletionRecord
	throw this;
}

//https://262.ecma-international.org/#prod-ReturnStatement