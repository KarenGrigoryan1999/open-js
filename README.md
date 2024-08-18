# Кастомный рантайм языка JavaScript на C++

## Базовые понятия

Семантически, конструкции в Ecma-262 поделены на Statement, Expression и Operator.

- Expression - конструкция, которая всегда возвращает значение
- Statement - конструкция, которая всегда возвращает undefined
- Operator - минимальная часть языка, которая работает с некоторым числом операндов (как правило с двумя, но их может быть любое количество) и возвращает новое значение

## Правила именования структур

Все структуры в кодовой базе рантайма, которые связаны со спецификацией именуются начиная с префикса Ecma, Например: EcmaEnvironment, EcmaExecutionContext, EcmaCompletionRecord и т. д. Все остальные структуры и классы являются машинерией рантайма

## Жизненный цикл выражений и стейтментов
В соответствии со спецификацией, существуют три стадии интерпретации кода в js. Мы можем получить доступ к двум стадиям: Static Semantic и Runtime Semantic. Мне больше нравится называть их хуками жизненного цикла. Пример использования данных стадий приведен ниже:

```c
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
```
