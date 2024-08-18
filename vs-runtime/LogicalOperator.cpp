#include "LogicalOperator.h"
#include "Token.h"
#include "NumberType.h"
#include "StringType.h"
#include "BoolType.h"
#include "LexicalTokenType.h"

using namespace std;

LogicalOperator::LogicalOperator(LexicalTokenType op, Expression* exp1, Expression* exp2) {
	this->op = op;
	this->exp1 = exp1;
	this->exp2 = exp2;
}

bool LogicalOperator::IsLooselyEqual(BaseValue* x, BaseValue* y) {
	//https://262.ecma-international.org/#sec-islooselyequal тут в пункте 4 написано что есть изменения функции LooselyEqual в секции B.3.6.2. Но я их не учитывал, по крайней мере не посчитал их сильно нужными пока что
	//Пункты 7, 8 и 13 для нас пока не актуален так как у нас нет BigInt
	ValueTypes type_of_x = x->GetType();
	ValueTypes type_of_y = y->GetType();

	if (type_of_x == y->GetType()) {
		return this->IsStrictlyEqual(x, y);
	}

	if (type_of_x == ValueTypes::NULL_TYPE && type_of_y == ValueTypes::UNDEFINED_TYPE) {
		return true;
	}

	if (type_of_x == ValueTypes::UNDEFINED_TYPE && type_of_y == ValueTypes::NULL_TYPE) {
		return true;
	}

	if (type_of_x == ValueTypes::NUMBER_TYPE && type_of_y == ValueTypes::STRING_TYPE) {
		return x->GetAsNumber() == y->GetAsNumber();
	}

	if (type_of_x == ValueTypes::STRING_TYPE && type_of_y == ValueTypes::NUMBER_TYPE) {
		return x->GetAsNumber() == y->GetAsNumber();
	}

	if (type_of_x == ValueTypes::BOOL_TYPE) {
		return x->GetAsNumber() == y->GetAsNumber();
	}

	if (type_of_y == ValueTypes::BOOL_TYPE) {
		return x->GetAsNumber() == y->GetAsNumber();
	}

	if (type_of_x == ValueTypes::OBJECT_TYPE || type_of_x == ValueTypes::FUNCTION_TYPE) {
		//TODO: тут мы должны вызвать toPrimitive у объектов перед сравнением, надо доделать!
		return false;
	}

	return false;
}

bool LogicalOperator::IsStrictlyEqual(BaseValue* x, BaseValue* y) {
	ValueTypes type_of_x = x->GetType();
	ValueTypes type_of_y = y->GetType();

	if (type_of_x != type_of_y) {
		return false;
	}
	if (type_of_x == ValueTypes::NUMBER_TYPE) {
		return x->GetAsNumber() == y->GetAsNumber();
	}
	if (type_of_x == ValueTypes::STRING_TYPE) {
		if (x->GetAsString() == y->GetAsString()) {
			return true;
		}
	}
	if (type_of_x == ValueTypes::BOOL_TYPE) {
		if (x->GetAsBoolean() == y->GetAsBoolean()) {
			return true;
		}
	}
	if (type_of_x == ValueTypes::OBJECT_TYPE || type_of_x == ValueTypes::FUNCTION_TYPE) {
		if (x->GetReference() == y->GetReference()) {
			return true;
		}
	}
	if (type_of_x == ValueTypes::NULL_TYPE || type_of_y == ValueTypes::UNDEFINED_TYPE) {
		return true;
	}

	return false;
}

bool LogicalOperator::IsLessThan(BaseValue* x, BaseValue* y) {
	//https://262.ecma-international.org/#sec-islessthan
	//Нужно доделать для объектов
	ValueTypes type_of_x = x->GetType();
	ValueTypes type_of_y = y->GetType();

	if (type_of_x == ValueTypes::STRING_TYPE && type_of_y == ValueTypes::STRING_TYPE) {
		string string_x = x->GetAsString();
		string string_y = y->GetAsString();

		for (int i = 0; i < min(string_x.size(), string_y.size()); i++) {
			if (string_y.at(i) > string_x.at(i)) return true;
			if (string_x.at(i) > string_y.at(i)) return false;
		}

		if (x->GetAsString().size() < y->GetAsString().size()) return true;
		else return false;
	}

	if (type_of_x == ValueTypes::NUMBER_TYPE && type_of_y == ValueTypes::STRING_TYPE) {
		return x->GetAsNumber() == y->GetAsNumber();
	}

	if (type_of_y == ValueTypes::NUMBER_TYPE && type_of_x == ValueTypes::STRING_TYPE) {
		return x->GetAsNumber() == y->GetAsNumber();
	}

	return false;
}

bool LogicalOperator::IsLooselyLessThan(BaseValue* x, BaseValue* y) {
	ValueTypes type_of_x = x->GetType();
	ValueTypes type_of_y = y->GetType();

	if (type_of_x == ValueTypes::NULL_TYPE && type_of_y == ValueTypes::UNDEFINED_TYPE && type_of_x == ValueTypes::UNDEFINED_TYPE && type_of_y == ValueTypes::NULL_TYPE) {
		return true;
	}
	return this->IsStrictlyEqual(x, y) && this->IsLessThan(x, y);
}

BaseValue* LogicalOperator::eval() {
	BaseValue* operand1 = exp1->eval();
	BaseValue* operand2 = exp2->eval();

	BoolType* TrueVariant = new BoolType(true);
	BoolType* FalseVariant = new BoolType(false);

	//Операторы && и || описаны в https://262.ecma-international.org/#sec-binary-logical-operators
	if (this->op == LexicalTokenType::LOGICAL_AND_EXPRESSION) {
		if (!operand1->GetAsBoolean()) {
			return operand1;
		}
		return operand2;
	}

	if (this->op == LexicalTokenType::LOGICAL_OR_EXPRESSION) {
		if (operand1->GetAsBoolean()) {
			return operand1;
		}
		return operand2;
	}

	if (this->op == LexicalTokenType::NOT_EQ_CHECKING) {
		return this->IsLooselyEqual(operand1, operand2) ? FalseVariant : TrueVariant;
	}

	if (this->op == LexicalTokenType::NOT_EQ_STRICT_CHECKING) {
		return this->IsStrictlyEqual(operand1, operand2) ? FalseVariant : TrueVariant;
	}

	if (this->op == LexicalTokenType::EQ_CHECKING) {
		return this->IsLooselyEqual(operand1, operand2) ? TrueVariant : FalseVariant;
	}

	if (this->op == LexicalTokenType::EQ_STRICT_CHECKING) {
		return this->IsStrictlyEqual(operand1, operand2) ? TrueVariant : FalseVariant;
	}

	if (this->op == LexicalTokenType::GT) {
		return this->IsLessThan(operand1, operand2) ? FalseVariant : TrueVariant;
	}

	if (this->op == LexicalTokenType::LT) {
		return this->IsLessThan(operand1, operand2) ? TrueVariant : FalseVariant;
	}

	if (this->op == LexicalTokenType::LTE) {
		return this->IsLooselyLessThan(operand1, operand2) ? TrueVariant : FalseVariant;
	}

	if (this->op == LexicalTokenType::GTE) {
		return this->IsLooselyLessThan(operand1, operand2) ? FalseVariant : TrueVariant;
	}
}

//https://262.ecma-international.org/#sec-binary-logical-operators