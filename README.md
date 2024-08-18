# Кастомный рантайм языка JavaScript на C++

## Небольшой дисклеймер

Я писал этот рантайм больше чем полгода и не реализовал огромную кучу фич, поэтому просьба не кидаться помидорами. Я реализую только фичи из ES5, более новые стандарты будут взяты в работу позже

## Базовые понятия

Семантически, конструкции в Ecma-262 поделены на Statement, Expression и Operator.

- Expression - конструкция, которая всегда возвращает значение
- Statement - конструкция, которая всегда возвращает undefined
- Operator - минимальная часть языка, которая работает с некоторым числом операндов (как правило с двумя, но их может быть любое количество) и возвращает новое значение

## Правила именования структур

Все структуры в кодовой базе рантайма, которые связаны со спецификацией именуются начиная с префикса Ecma, Например: EcmaEnvironment, EcmaExecutionContext, EcmaCompletionRecord и т. д. Все остальные структуры и классы являются машинерией рантайма

## Жизненный цикл выражений и стейтментов
В соответствии со спецификацией, существуют три стадии интерпретации кода в js. Мы можем получить доступ к двум стадиям: Static Semantic и Runtime Semantic. Мне больше нравится называть их хуками жизненного цикла.
Конструктор Expression - код, исполняющийся на стадии Static Semantic. Метод eval - некоторый код, исполняющийся на стадии Runtime Semantic.
Пример использования данных стадий приведен ниже:

```c
#include "IfStatement.h"

IfStatement::IfStatement(Expression* condition, Node* if_statement, Node* else_statement) {
	//Static semantic
	//Инициализация (не отсносится к спеке)
	this->condition_ = condition;
	this->ifStatement_ = if_statement;
	this->elseStatement_ = else_statement;
	//Проверка на ошибки на стадии Static Semantic
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

## Как написать built-in функцию
В рантайме реализованы некоторы built-in функции (isNan, parseInt, setTimeout, queueMicrotask и так далее). Создать собственную встроенную функцию очень легко! Достаточно добавить её реализацию в класс Functions. Темплейт built-in функции выглядит так:
```c
// реализация функции isNan
//args - вектор, содержащий список переданных в функцию аргументов
BaseValue* is_nan(vector<Expression*>* args) {
	string temp;
	if (args->at(0)->eval()->GetType() == ValueTypes::NAN_TYPE) return new BoolType(true);

	return new BoolType(false);
}
```

## Некоторые особенности рантайма
Сюда я буду выносить некоторые особенности и огрехи, которые есть в рантайме

1. В рантайме нет объекта console ( Пока используйте функцию print
2. Особенности конвертации ссылочных типов
При неявном преобразовании объектов в примитив можно увидеть что рантайм выдает ошибку:
```js
const exp = ({}) + 10;
print(exp);//Uncaught TypeError: Cannot convert object to primitive value
```

В других рантаймах все будет ок и выведится ```[object Object]10```. На самом деле это не противоречит спеке. Дело в том что по спецификации рантайм должен попробовать использовать метод toString или valueOf. Если их нет, то он обязан выдать ошибку типа TypeError. В моем рантайме методы valueOf и toString не определены, но вы можете их определить самостоятельно:
```js
Object.prototype.toString = function() {
  return "[object Object]";
}
const exp = ({}) + 10;
print(exp);//Выведется [object Object]10
```
3. Механизм замыканий
В рантайме работает механизм замыканий, но он вечно ломается. Думаю что в будущем код связанный с ExoticObjectFunction будет пересмотрен и в обязательном порядке будет покрыт тестами
```js
function counter(init) {
  return function() {
    return ++init;
  }
}

const cnt1 = counter(1);
const cnt2 = counter(100);

print("cnt1: ", cnt1());
print("cnt2: ", cnt2());
print("cnt2: ", cnt2());
print("cnt1: ", cnt1());

//Результат будет:
//cnt1:  2
//cnt2:  101
//cnt2:  102
//cnt1:  3
```
4. Прототипное наследование, this и дескрипторы свойств
Вы можете получать доступ к this, писать ООП код через прототипы и делать геттеры и сеттеры!
```js
function Lion(name, age) {
  this.name = name;
  this.age = age;
}

Lion.prototype.sayHello = function() {
  return "Hello, I am " + this.name + ", I am " + this.age + " years old!";
}

const Simba = new Lion("Simba", 5);

Object.defineProperty(Simba, "greeting", {
  get: function() {
    return "Hello, I am " + this.name + "!";
  },
});

Object.defineProperty(Simba, "height", {
  get: function() {
    return "my height is " + this.h;
  },
  set: function(value) {
    this.h = value + " sm";
  }
});

print(Simba.sayHello());

print(Simba.greeting);

Simba.height = 1;

print(Simba.height);

//Выведет:
//Hello, I am Simba, I am 5 years old!
//Hello, I am Simba!
//my height is 1 sm
```
5. В рантайме пока нет массивов (
6. В рантайме пока нет Infinity ((
7. CallExpression реализован не по спецификации
