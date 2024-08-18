#include "IfStatement.h"

IfStatement::IfStatement(Expression* condition, Node* if_statement, Node* else_statement) {
	//Static semantic
	//�������������
	this->condition_ = condition;
	this->ifStatement_ = if_statement;
	this->elseStatement_ = else_statement;
	//�������� �� ������
	//���� � �������� ifStatement ��� elseStatement ��������� ������� ������� FunctionDeclaration, �� � �� ������� ������ ����� ������ ���� SyntaxError
	//TODO: ��� ������ � �������� �������� ����� - ����������� �������� �� ���� ������
}

BaseValue* IfStatement::eval() {
	//Runtime semantic
	BaseValue* result = this->condition_->eval();//��������� �� ��� ������ �������

	//�������� � boolean � ���� ������� �������, �� ��������� ����� ifStatement. ����� - ��������� elseStatement
	if (result->GetAsBoolean()) {
		this->ifStatement_->eval();
	}
	else if(this->elseStatement_ != NULL) {
		this->elseStatement_->eval();
	}

	return new UndefinedType();//� ����� ������, ���������� undefined
}

//https://262.ecma-international.org/#sec-if-statement