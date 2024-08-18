#include "FunctionDeclaration.h"
#include "string"
#include "Token.h"
#include "Functions.h"

using namespace std;

FunctionDeclaration::FunctionDeclaration(string name, EcmaEnvironment* block, bool is_const, Parser* parser) {
	//Код в конструкторе выполняется на фазе Static Semantic
	this->name = name;
	this->block = block;
	this->is_const = is_const;
	this->function = new AnonumousFuncExpression(block, parser);

	//ClosureStatement. Выполняем инициализацию функции на стадии StaticSemantic
	BaseValue* function = new BaseValue(this->function);
	if (this->block == NULL) {
		IdentifierCollection::SetGlobal(this->name, function, this->is_const);
	}
	else {
		EcmaEnvironment* current = this->block->outer;

		if (current == NULL) {
			IdentifierCollection::SetGlobal(this->name, function, this->is_const);
		}
		else {
			current->records->SetLocal(this->name, function, this->is_const);
		}
	}
}

BaseValue* FunctionDeclaration::eval() {
	//метод eval вызывается на фазе Runtime Semantics
	return 0;
}