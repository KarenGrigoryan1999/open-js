#include "IfStatement.h"

IfStatement::IfStatement(Expression* condition, Node* if_statement, Node* else_statement) {
	//Static semantic
	//Инициализация
	this->condition_ = condition;
	this->ifStatement_ = if_statement;
	this->elseStatement_ = else_statement;
	//Проверка на ошибки
	//Если в качестве ifStatement или elseStatement выступает меченая функция FunctionDeclaration, то в не строгом режиме будет ошибка типа SyntaxError
	//TODO: как только в рантайме появятся метки - реализовать проверку на этой стадии
}

BaseValue* IfStatement::eval() {
	//Runtime semantic
	BaseValue* result = this->condition_->eval();//Вычисляем то что внутри условия

	//Приводим к boolean и если условие истинно, то выполняем ветку ifStatement. Иначе - выполняем elseStatement
	if (result->GetAsBoolean()) {
		this->ifStatement_->eval();
	}
	else if(this->elseStatement_ != NULL) {
		this->elseStatement_->eval();
	}

	return new UndefinedType();//В конце концов, возвращаем undefined
}

//https://262.ecma-international.org/#sec-if-statement