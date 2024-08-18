#include "TypeofOperator.h"

using namespace std;

TypeofOperator::TypeofOperator(Expression* unary_expression) {
	this->unary_expression_ = unary_expression;
}

BaseValue* TypeofOperator::eval() {
	//Runtime Semantics
	try {
		BaseValue* val = this->unary_expression_->eval();//��������� ���������
		ValueTypes type = val->GetType();//�������� ���

		if (type == ValueTypes::UNDEFINED_TYPE) {
			return new StringType("undefined");
		}
		if (type == ValueTypes::STRING_TYPE) {
			return new StringType("string");
		}
		if (type == ValueTypes::NULL_TYPE || type == ValueTypes::OBJECT_TYPE) {
			return new StringType("object");
		}
		if (type == ValueTypes::FUNCTION_TYPE) {
			return new StringType("function");
		}
		if (type == ValueTypes::BOOL_TYPE) {
			return new StringType("boolean");
		}
		if (type == ValueTypes::NUMBER_TYPE || type == ValueTypes::NAN_TYPE) {
			return new StringType("number");
		}

		return new StringType("object");
	}
	catch (BaseException* err) {
		//���� �� ����� ������ ReferenceError, �� � ��� unresolvable ��������� (�� ���� ��� ����� � ����� ���� Environment Record). � ���� ������ - ���������� Undefined
		if(strcmp(err->plain_name, "ReferenceError")) return new StringType("undefined");

		throw err;
	}
}

//https://262.ecma-international.org/#sec-typeof-operator